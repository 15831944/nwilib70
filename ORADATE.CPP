#include "stdafx.h"
#include "oradate.h"
#include "nwivar.h"
#include "qdata.h"

extern ODatabase theDB;

IMPLEMENT_DYNAMIC(COraDate, CObject)
/********** OraDate Class ****************/
COraDate::COraDate(COraDate& Date)
{
	Setup(Date.GetOraLoader(), Date.GetDate(), Date.GetPeriodEndDate(), Date.GetRateBasis(), Date.GetAADays());
}

COraDate::COraDate(LPCTSTR Date, LPCTSTR PeriodEndDate, LPCTSTR RateBasis, int AADays)
{
	Setup(Date, PeriodEndDate, RateBasis, AADays);
}

int COraDate::Setup(LPCTSTR Date, LPCTSTR PeriodEndDate, LPCTSTR RateBasis, int AADays)
{
	if(!Date || strlen(Date) <= 0)
		return 0;
	
	CQData QData;
	
	m_OraLoader.SetDB(&theDB);
	m_Date = Date;
	m_Date.MakeUpper();
	
	m_nAADays = AADays;
	m_nMonthBase = 31;
    m_nYearBase = 365;
	m_RateBasis = "A/365";
	if(PeriodEndDate && strlen(PeriodEndDate) > 0)
		m_PeriodEndDate = PeriodEndDate;

	if(!GetDayMonthYear())
		return 0;

	if(RateBasis && strlen(RateBasis) > 0)
	{
		m_RateBasis = RateBasis;
		m_RateBasis.MakeUpper();
		
		if(m_RateBasis.Find("360") >= 0)
			m_nYearBase = 360;
        else
			if(m_RateBasis.Find("A/A (R)") >= 0 || m_RateBasis.Find("A/A (AS)") >= 0)
			{
				if(GetPeriodEndDate().IsEmpty())
					m_nYearBase = 365 + IsLeapYear();
				else
					m_nYearBase = GetDaysInYear();
			}
			else
				if(m_RateBasis.Find("A/364") >= 0)
					m_nYearBase = 364;
				else
					if(m_RateBasis.Find("A/A (A)") >= 0)
						m_nYearBase = GetAAAYearBase();
					else
   						if(m_RateBasis.Find("A/A") >= 0)
						{
							double Days = 365;
							Days /= m_nAADays;
							m_nYearBase = !m_nAADays ? 364 : m_nAADays* ((int) QData.Round(Days, 0));
						}
						else
							if(m_RateBasis.Find("AB/252") >= 0)
								m_nYearBase = 252;

    	if(m_RateBasis.Find("30") != -1)
			m_nMonthBase = 30;
	}

	return 1;
}

int COraDate::Setup(COraLoader &OraLoader, LPCTSTR Date, LPCTSTR PeriodEndDate, LPCTSTR RateBasis, int AADays)
{
	m_OraLoader = OraLoader;
	return Setup(Date, PeriodEndDate, RateBasis, AADays);
}

int COraDate::GetDayMonthYear()
{
	int pos;
   
	m_nDay = 0;
	m_nMonth = 0;
	m_nYear = 0;
   
	if(m_Date.IsEmpty())
		return 0;
	
	m_nMonth = atoi(m_Date);
	pos = m_Date.Find(SLASH);	
	m_nDay = atoi((LPCTSTR ) m_Date + pos + 1);
	m_nYear = atoi((LPCTSTR ) m_Date + pos + 4);
			
	return (m_nDay && m_nMonth && m_nYear) ? 1 : 0;
}

int COraDate::IsLeapYear()
{
	return (m_nYear % 400 == 0 || (m_nYear % 4 == 0 && m_nYear % 100 != 0)) ? 1 : 0;
}

int COraDate::CompareDate(LPCTSTR Date)
{
	if(!GetOraLoader().GetDB())
		return -10000;

	COraDate Date2(GetOraLoader(), Date, GetPeriodEndDate(), GetRateBasis(), m_nAADays);

	if(m_nYear > Date2.m_nYear) 
		return 1;
	else
		if(m_nYear == Date2.m_nYear && m_nMonth > Date2.m_nMonth) 
			return 1;
		else
			if(m_nYear == Date2.m_nYear && m_nMonth == Date2.m_nMonth &&
				m_nDay > Date2.m_nDay)
				return 1;
			else
				if(m_nYear == Date2.m_nYear && m_nMonth == Date2.m_nMonth &&
					m_nDay == Date2.m_nDay)
					return 0;
	return -1;
}

int COraDate::GetActualDays()
{
	int Mp, Yp, T;
	CString Data;

	m_nDays = 1;
	if(m_nMonth <= 2)
	{
		Mp = 0;
		Yp = m_nYear - 1;
	}
	else
	{
		Data.Format("%lf", 0.4*m_nMonth + 2.3);
		Mp = atoi(Data);
		Yp = m_nYear;
	}

	T = Yp/4 - Yp/100 + Yp/400;
	m_nDays = m_nDays*365*m_nYear + 31*(m_nMonth - 1) + m_nDay + T - Mp;
   
	return m_nDays;
}

int COraDate::GetLastDayofMonth()
{
	int DayOfMonth[] = {0, 31, 28, 31, 30, 31, 30, 
						   31, 31, 30, 31, 30, 31};
	
	if(GetMonth() == 2)
		return DayOfMonth[GetMonth()] + IsLeapYear();
	else
		return DayOfMonth[GetMonth()];
}

int COraDate::GetAAAYearBase()
{
	CString sDate;

	if(GetDate().Find("02/29", 0) >= 0)
		sDate.Format("02/28/%d", atoi(GetDate().Right(2)) + 1);
	else
		sDate.Format("%s%02d", (LPCTSTR) GetDate().Left(6), atoi(GetDate().Right(2)) + 1);
	GetOraLoader().GetSql().Format("SELECT TO_DATE('%s') - TO_DATE('%s') FROM DUAL ",
									(LPCTSTR) sDate, (LPCTSTR) GetDate());

	m_nYearBase = GetOraLoader().GetCount(GetOraLoader().GetSql());

	return m_nYearBase;
}

int COraDate::GetDaysInYear()
{
	CString Data;
	CQData QData;

	Data = QData.GetQueryText(GetPeriodEndDate());
	GetOraLoader().GetSql().Format("SELECT TO_DATE(%s) - ADD_MONTHS(TO_DATE(%s), -12) FROM DUAL ", 
									(LPCTSTR) Data, (LPCTSTR) Data);
	m_nYearBase = GetOraLoader().GetCount(GetOraLoader().GetSql());

	return m_nYearBase;
}

int COraDate::GetDays(LPCTSTR Date, LPCTSTR Sql)
{
	int D1, D2, M1, M2, Y1, Y2;
	CQData QData;

	m_nIntDays2 = 0;
	m_nYearBase2 = 0;
	
	if(!Date || strlen(Date) == 0)
		return 0;

	if(CompareDate(Date) >= 0)
		return 0;

	if(!GetOraLoader().GetDB())
		return 0;

	COraDate Date2(GetOraLoader(), Date, GetPeriodEndDate(), GetRateBasis(), m_nAADays);

	if(strstr(GetRateBasis(), "AB/252") || strstr(GetRateBasis(), "A/A (A)"))
	{
		LPCTSTR pFromDate;
		CString sDate;

		sDate = QData.GetQueryDate(Date);
		pFromDate = QData.GetQueryDate(GetDate());
		if(strstr(GetRateBasis(), "AB/252"))
		{
			GetOraLoader().GetSql().Format("SELECT SUM(DECODE(TO_CHAR(TO_DATE(%s) + INC, 'D'), '7', 0, '1', 0, 1)) "
								"FROM SEMAM.MYSEQ "
								"WHERE TO_DATE(%s) + INC <= TO_DATE(%s) "
								"AND INC > 0 ", pFromDate, pFromDate, (LPCTSTR) sDate);

			m_nIntDays = GetOraLoader().GetCount(GetOraLoader().GetSql());
			m_nIntDays -= GetOraLoader().GetHolidays(GetDate(), Date, "BRL");
			if(m_nIntDays < 0)
				m_nIntDays = 0;
			return m_nIntDays;
		}
		else
			if(strstr(GetRateBasis(), "A/A (A)"))
			{
				if(Sql && strlen(Sql) > 0)
				{
					GetOraLoader().GetSql() = Sql;
					m_nYearBase = GetOraLoader().GetCount(GetOraLoader().GetSql());
				}
			}
	}

	if(strncmp(GetRateBasis(), "30", 2) == 0 && strstr(GetRateBasis(), "/360")) /* 30/360 over */
	{
		D1 = m_nDay;
		D2 = Date2.m_nDay;
		M1 = GetMonth();
		M2 = Date2.GetMonth();
		Y1 = GetYear();
		Y2 = Date2.GetYear();
      
		if(strstr(GetRateBasis(), "30/360"))
		{
			if(D2 == Date2.GetLastDayofMonth() && Date2.GetMonth() == 2 &&
				D1 == GetLastDayofMonth() && GetMonth() == 2)
				D2 = 30;   
      
			if(D1 == GetLastDayofMonth() && GetMonth() == 2)
				D1 = 30;
      
			if(D2 == 31)		// if(D2 == 31 && D1 >= 30)
				D2 = 30;		//    D2 = 30;
      
			if(D1 == 31)
				D1 = 30;
		}
		else
			if(strcmp(GetRateBasis(), "30E/360") == 0)
			{
				if(D1 == 31)
					D1 = 30;

				if(D2 == 31)
					D2 = 30;
			}
			else
				if(strcmp(GetRateBasis(), "30EI/360") == 0)
				{
					if(D1 == GetLastDayofMonth())
						D1 = 30;
	
					if(D2 == Date2.GetLastDayofMonth() && Date2.GetMonth() != 2)
						D2 = 30;
				}
				else
					if(strcmp(GetRateBasis(), "30E+/360") == 0)
					{
						if(D1 == 31)
							D1 = 30;
				
						if(D2 == 31)
						{
							D2 = 1;
							M2 += 1;
							if(M2 > 12)
							{
								Y2 += 1;
								M2 = 1;
							}
						}
					}
		
		m_nIntDays = (D2 - D1) + ((M2 - M1) + (Y2 - Y1)*12)*30;
		return m_nIntDays;
	}

	/* Actual Over */
	if(strstr(GetRateBasis(), "A/A (I)")) // day1/365 + day2/366 = (day1 + day2 * 365/366)/365
	{
		if(GetYear() != Date2.GetYear())
		{
			Y1 = 365 + IsLeapYear();
			Y2 = 365 + Date2.IsLeapYear();
		}
		
		if(GetYear() == Date2.GetYear() || Y1 == Y2)
			SetIntDays(Date2.GetActualDays() - GetActualDays());
		else
		{
			COraDate Date3(*this);
			Date3.m_nMonth = 12;
			Date3.m_nDay = 31;
			D1 = Date3.GetActualDays() - GetActualDays() + 1;
			D2 = Date2.GetActualDays() - Date3.GetActualDays();
			m_nIntDays = D1;
			m_nYearBase = Y1;
			m_nIntDays2 = D2;
			m_nYearBase2 = Y2;
		}
	}
	else
		m_nIntDays = Date2.GetActualDays() - GetActualDays();

	return m_nIntDays;
}
