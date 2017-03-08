#ifndef __ORADATE_H__
#define __ORADATE_H__

#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif

class COraDate : public CObject
{
DECLARE_DYNAMIC(COraDate)
protected:
	COraLoader m_OraLoader;
	CString m_RateBasis;
	CString m_Date;
	CString m_PeriodEndDate;
	int m_nMonthBase;
	int m_nYearBase;
	int m_nYearBase2;
	int	m_nDay;
	int	m_nMonth;
	int	m_nYear;
	int	m_nAADays;
	int m_nIntDays;
	int m_nIntDays2;
	int m_nDays;

protected:
	int GetDayMonthYear();
	int GetAAAYearBase();
	int GetDaysInYear();

public:	
	COraDate(COraLoader &OraLoader, LPCTSTR Date, LPCTSTR PeriodEndDate, LPCTSTR RateBasis = NULL, int AADays = 0) 
			{ Setup(OraLoader, Date, PeriodEndDate, RateBasis, AADays); }
	COraDate(LPCTSTR Date, LPCTSTR PeriodEndDate, LPCTSTR RateBasis = NULL, int AADays = 0);
	COraDate(COraDate &Date);
	COraDate() {};
	
	int Setup(LPCTSTR Date, LPCTSTR PeriodEndDate = NULL, LPCTSTR RateBasis = NULL, int AADays = 0);
	int	Setup(COraLoader &OraLoader, LPCTSTR Date, LPCTSTR PeriodEndDate, LPCTSTR RateBasis = NULL, int AADays = 0);

	CString &GetRateBasis() {return m_RateBasis; }
	CString &GetDate() { return m_Date; }
	CString &GetPeriodEndDate() { return m_PeriodEndDate; }
	int IsLeapYear();	
    int CompareDate(LPCTSTR Date);
	COraLoader &GetOraLoader() { return m_OraLoader; }
    int GetDays(LPCTSTR Date, LPCTSTR Sql = 0);
	int GetYearBase() { return m_nYearBase; }
	int GetYearBase2() { return m_nYearBase2; }
    int GetMonthBase() { return m_nMonthBase; }
	int GetDay() { return m_nDay; }
	int GetAADays() { return m_nAADays; }
	int GetMonth() { return m_nMonth; }
	int GetYear() { return m_nYear; }
	int GetDays() { return m_nDays; }
    int GetActualDays();    
	int GetLastDayofMonth();
	int GetIntDays() { return m_nIntDays; }
	int GetIntDays2() { return m_nIntDays2; }
	void SetYearBase(int nYearBase) { m_nYearBase = nYearBase; }
	void SetIntDays(int nIntDays) { m_nIntDays = nIntDays; }
	void SetIntDays2(int nIntDays2) { m_nIntDays2 = nIntDays2; }
	void SetDays(int nDays) { m_nDays = nDays; }
};

#endif __ORADATE_H__