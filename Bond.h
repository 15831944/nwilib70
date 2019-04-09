#ifndef __BOND_H__
#define __BOND_H__

#ifndef __EQUITY_H__
#include "equity.h"
#endif

#ifndef __ORALOADER__
#include "oraloader.h"
#endif

class CBond : public CEquity
{
DECLARE_DYNAMIC(CBond)

protected:
	COraLoader m_OraLoader;
	double m_BondFact;
	double m_Rate;
	BOOL m_bAccruable;
	BOOL m_bPrePaid;
	CString m_PreIntDate;
	CString m_ValueDate;
	CString m_Date;
	CString m_PeriodEndDate;
	CString m_RateBasis;
	int m_nAADays;
	int m_nYearBase;
	int m_nYearBase2;
	int m_nIntDays;
	int m_nIntDays2;
	int m_nPreIntDays;
	int m_nPreIntDays2;

protected:
	virtual double ComputeValue(double NomAmount = -1, double Price = -1);
	virtual double ComputeInterest(double Amount, double Rate, int Days, int YearBase);
	virtual double ComputeInterest(double Amount, double Rate, LPCTSTR StartDate, LPCTSTR EndDate, 
									LPCTSTR RateBasis, int &IntDays, int &intDays2, int AADays = 0);
	int ComputeDays(LPCTSTR StartDate, LPCTSTR EndDate, LPCTSTR RateBasis, int &YearBase, int AADays = 0);
	int ComputeDays(LPCTSTR StartDate, LPCTSTR ValueDate, LPCTSTR EndDate, LPCTSTR RateBasis,
					int &YearBase, int AADays = 0);

public:
	CBond() { SetBondFact(); SetupIntInfo(EMPTYSTRING, EMPTYSTRING, EMPTYSTRING, EMPTYSTRING, 
				EMPTYSTRING, 0, 0, FALSE, FALSE); }
	CBond(CBond& Bond);		
	CBond(COraLoader &OraLoader, LPCTSTR Type, double NomAmount, double Price, 
		double FxRate, double BondFact = 1, LPCTSTR Dir = P);
	CBond(COraLoader &OraLoader, LPCTSTR Type, double NomAmount, double Price, 
		double FxRate, LPCTSTR AssetType = EQUITYTYPE, LPCTSTR Dir = P);
	~CBond() {}

	void SetupIntInfo(LPCTSTR PreIntDate, LPCTSTR ValueDate, LPCTSTR Date, LPCTSTR PeriodEndDate, LPCTSTR RateBasis, 
			double Rate = 0, int AADays = 0, BOOL bAccruble = FALSE, BOOL bPrePaid = FALSE);
	void SetupIntInfo(LPCTSTR PreIntDate, LPCTSTR ValueDate, LPCTSTR Date, LPCTSTR PeriodEndDate, LPCTSTR RateBasis, 
			double Rate = 0, int AADays = 0, LPCTSTR Accrual = N, LPCTSTR PrePaid = N);

	void Setup(COraLoader &OraLoader, LPCTSTR Type, double NomAmount, double Price, double FxRate, 
			double BondFact = 1, LPCTSTR Dir = P);
	void Setup(COraLoader &OraLoader, LPCTSTR Type, double NomAmount, double Price, double FxRate, 
			LPCTSTR AssetType = EQUITYTYPE, LPCTSTR Dir = P);

	COraLoader &GetOraLoader() { return m_OraLoader; }
	double GetBondFact() { return m_BondFact; }
	void SetBondFact(double BondFact = 1) { m_BondFact = BondFact; }

	double GetRate() { return m_Rate; }
	void SetRate(double Rate = 0) { m_Rate = Rate; }

	BOOL GetAccruable() { return m_bAccruable; }
	void SetAccruable(BOOL Accruable = FALSE) { m_bAccruable = Accruable; }

	BOOL GetPrePaid() { return m_bPrePaid; }
	void SetPrePaid(BOOL PrePaid = FALSE) { m_bPrePaid = PrePaid; }

	int GetAADays() { return m_nAADays; }
	void SetAADays(int AADays = 0) { m_nAADays = AADays; }

	int GetYearBase() { return m_nYearBase; }
	int GetYearBase2() { return m_nYearBase2; }
	void SetYearBase(int YearBase = 365) { m_nYearBase = YearBase; }

	int	GetPreIntDays() { return m_nPreIntDays; }
	int	GetPreIntDays2() { return m_nPreIntDays2; }
	void SetPreIntDays(int PreIntDays = 0) { m_nPreIntDays = PreIntDays; }

	int GetIntDays() { return m_nIntDays; }
	int GetIntDays2() { return m_nIntDays2; }
	void SetIntDays(int IntDays = 0) { m_nIntDays = IntDays; }

	CString &GetPreIntDate() { return m_PreIntDate; }
	CString &GetValueDate() { return m_ValueDate; }
	CString &GetDate() { return m_Date; }
	CString &GetPeriodEndDate() { return m_PeriodEndDate; }
	CString &GetRateBasis() { return m_RateBasis; }

	double GetPrePaidInt();
	double GetAccrualInt();

	double GetNetPrice(double GrossPrice, double BrFee, double OtherFee);
};
#endif
