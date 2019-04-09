#include "stdafx.h"
#include "bond.h"
#include "math.h"
#include "oradate.h"


IMPLEMENT_DYNAMIC(CBond, CEquity)
CBond::CBond(COraLoader &OraLoader, LPCTSTR Type, double NomAmount, double Price, 
			double FxRate, double BondFact, LPCTSTR Dir) : 
			CEquity(Type, NomAmount, Price, FxRate, Dir) 
{ 
	m_OraLoader = OraLoader;
	SetBondFact(BondFact);
	SetupIntInfo(EMPTYSTRING, EMPTYSTRING, EMPTYSTRING, EMPTYSTRING, 0, 
				0, FALSE, FALSE);
}

CBond::CBond(COraLoader &OraLoader, LPCTSTR Type, double NomAmount, double Price, 
			 double FxRate, LPCTSTR AssetType, LPCTSTR Dir) : 
		CEquity(Type, NomAmount, Price, FxRate, Dir) 
{	
	m_OraLoader = OraLoader;
	if(AssetType && strcmp(AssetType, BONDTYPE) == 0)
		SetBondFact(0.01);
	else
		SetBondFact();

	SetupIntInfo(EMPTYSTRING, EMPTYSTRING, EMPTYSTRING, EMPTYSTRING, 0, 0, FALSE, FALSE); 
}

CBond::CBond(CBond &Bond) : CEquity(Bond)
{
	m_OraLoader = Bond.GetOraLoader();
	SetBondFact(Bond.GetBondFact());

	SetupIntInfo(Bond.GetPreIntDate(), Bond.GetValueDate(), Bond.GetDate(), 
				Bond.GetPeriodEndDate(), Bond.GetRateBasis(), Bond.GetRate(), Bond.GetAADays(), 
				Bond.GetAccruable(), Bond.GetPrePaid());
}
	
void CBond::Setup(COraLoader &OraLoader, LPCTSTR Type, double NomAmount, 
				  double Price, double FxRate, double BondFact, LPCTSTR Dir)
{
	m_OraLoader = OraLoader;
	CEquity::Setup(Type, NomAmount, Price, FxRate, Dir);
	SetBondFact(BondFact);
}

void CBond::Setup(COraLoader &OraLoader, LPCTSTR Type, double NomAmount, 
				  double Price, double FxRate, LPCTSTR AssetType, LPCTSTR Dir)
{
	double BondFact;
	
	if(AssetType && strcmp(AssetType, BONDTYPE) == 0)
		BondFact = 0.01;
	else
		BondFact = 1;
	Setup(OraLoader, Type, NomAmount, Price, FxRate, BondFact, Dir);
}

void CBond::SetupIntInfo(LPCTSTR PreIntDate, LPCTSTR ValueDate, LPCTSTR Date, LPCTSTR PeriodEndDate, 
				LPCTSTR RateBasis, double Rate, int AADays, BOOL bAccruable, BOOL bPrePaid)
{
	if(!bAccruable)
	{
		SetRate();
		SetAADays();
		SetYearBase();
		SetIntDays();
		SetPreIntDays();
	}
	else
	{
		CQData QData;

		SetRate(Rate);
		SetAccruable(bAccruable);
		SetPrePaid(bPrePaid);
		QData.SetString(m_PreIntDate, PreIntDate);
		QData.SetString(m_ValueDate, ValueDate);
		QData.SetString(m_Date, Date);
		QData.SetString(m_PeriodEndDate, PeriodEndDate);
		QData.SetString(m_RateBasis, RateBasis);
		SetAADays(AADays);
		if(bPrePaid)
			m_nPreIntDays = ComputeDays(PreIntDate, ValueDate, RateBasis, m_nYearBase, AADays);

		if(bAccruable)
			m_nIntDays = ComputeDays(PreIntDate, ValueDate, Date, RateBasis, m_nYearBase, AADays);
	}
}

void CBond::SetupIntInfo(LPCTSTR PreIntDate, LPCTSTR ValueDate, LPCTSTR Date, LPCTSTR PeriodEndDate, 
				LPCTSTR RateBasis, double Rate, int AADays, LPCTSTR Accrual, LPCTSTR PrePaid)
{
	BOOL bAccrual, bPrePaid;

	if(Accrual && strcmp(Accrual, Y) == 0)
		bAccrual = TRUE;
	else
		bAccrual = FALSE;

	if(PrePaid && strcmp(PrePaid, Y) == 0)
		bPrePaid = TRUE;
	else
		bPrePaid = FALSE;

	SetupIntInfo(PreIntDate, ValueDate, Date, PeriodEndDate, RateBasis, Rate, AADays, bAccrual, bPrePaid);
}

int CBond::ComputeDays(LPCTSTR StartDate, LPCTSTR EndDate, LPCTSTR RateBasis, int &YearBase, int AADays)
{
	int Days;

	COraDate ODate(GetOraLoader(), StartDate, GetPeriodEndDate(), RateBasis, AADays);

	YearBase = ODate.GetYearBase();
	if(strlen(StartDate) <= 0)
		return 0;

	Days = ODate.GetDays(EndDate);
	if(Days <= 0)
		Days = 0;
	return Days;
}

int CBond::ComputeDays(LPCTSTR StartDate, LPCTSTR ValueDate, LPCTSTR EndDate, 
					   LPCTSTR RateBasis, int &YearBase, int AADays)
{
	int Days;

	COraDate ODate(GetOraLoader(), StartDate, GetPeriodEndDate(), RateBasis, AADays);

	YearBase = ODate.GetYearBase();
	if(strlen(StartDate) <= 0)
		return 0;

	ODate.Setup(GetOraLoader(), ValueDate, GetPeriodEndDate(), RateBasis, AADays);
	Days = ODate.GetDays(EndDate);

	if(Days <= 0)
		Days = 0;
	return Days;
}

double CBond::ComputeInterest(double Amount, double Rate, int Days, int YearBase)
// Note: that Rate/100.
{
	CQData QData;

	if(m_RateBasis == "A/A (AS)")
		return Amount/1000*QData.Round(1000*Rate/100*Days/(YearBase == 0 ? 1 : YearBase), 2);
	else
		if(m_RateBasis == "A/365I")
		{
			double d, x, y;
			d = Days;
			x = QData.Round((100*pow(1 + Rate/100, d/365) - 1), 5);
			y = QData.Round(x*d/365, 5);
			return Amount/100*y;
		}
		else
			if(m_RateBasis == "AB/252")
				return Amount*(pow(1.0 + Rate/100, ((double) Days)/(YearBase == 0 ? 1 : YearBase)) - 1);
			else
				return Amount*Rate/100*Days/(YearBase == 0 ? 1 : YearBase);
}

double CBond::ComputeInterest(double Amount, double Rate, LPCTSTR StartDate, 
						LPCTSTR EndDate, LPCTSTR RateBasis, int &IntDays, int &IntDays2, int AADays)
{
	double Interest = 0;

	COraDate ODate(GetOraLoader(), StartDate, GetPeriodEndDate(), RateBasis, AADays);
	
	if(strlen(StartDate) <= 0)
		return 0;
	
	IntDays = ODate.GetDays(EndDate);
	if(IntDays <= 0)
		IntDays = 0;
	
	if(ODate.GetYearBase2() > 0)
		IntDays2 = ODate.GetIntDays2();

	Interest = ComputeInterest(Amount, Rate, IntDays, ODate.GetYearBase());

	if(strcmp(RateBasis, "A/A (I)") == 0)		
		Interest += ComputeInterest(Amount, Rate, IntDays2, ODate.GetYearBase2());
	return Interest;
}

double CBond::GetPrePaidInt()
{
	if((m_Type == SECURITIES || m_Type == CDS || m_Type == REPO || m_Type == LEVERAGE) && GetPrePaid())
		return ComputeInterest(GetNomAmount(), GetRate(), GetPreIntDate(), GetValueDate(), 
							GetRateBasis(), m_nPreIntDays, m_nPreIntDays2, GetAADays());

	return 0;
}

double CBond::GetAccrualInt()
{
	if((m_Type == SECURITIES || m_Type == CDS || m_Type == REPO || m_Type == LEVERAGE || m_Type == INTSWAP) && GetAccruable())
		return ComputeInterest(GetNomAmount(), GetRate(), GetPreIntDate(), GetDate(), 
							GetRateBasis(), m_nIntDays, m_nIntDays2, GetAADays());
	
	return 0;
}

double CBond::ComputeValue(double NomAmount, double Price)
{
	return CEquity::ComputeValue(NomAmount, Price)*GetBondFact();
}

double CBond::GetNetPrice(double GrossPrice, double BrFee, double OtherFee)
{
	double Fees;
	double Amount;
	
	Amount = GetNomAmount();
	if(Amount <= .001)
		Amount = 1;
	Fees = (BrFee + OtherFee)/Amount/GetBondFact();
	
	return GrossPrice + (strcmp(GetDir(), S) == 0 ? -1*Fees : Fees);
}
