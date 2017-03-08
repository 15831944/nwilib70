#ifndef __AMORTBOND_H__
#define __AMORTBOND_H__

#ifndef __BOND_H__
#include "bond.h"
#endif

class CAmortBond : public CBond
{
DECLARE_DYNAMIC(CAmortBond)

protected:
	double m_AmortFact;

protected:
	virtual double ComputeValue(double NomAmount = -1, double Price = -1);
	virtual double ComputeInterest(double Amount, double Rate, int Days, int YearBase);
	virtual double ComputeInterest(double Amount, double Rate, LPCTSTR StartDate, 
						LPCTSTR EndDate, LPCTSTR RateBasis, int *IntDays, int *IntDays2);

public:
	CAmortBond() { SetAmortFact(); }
	CAmortBond(COraLoader &OraLoader, CAmortBond &AmortBond) : 
			CBond(AmortBond) { SetAmortFact(AmortBond.GetAmortFact()); }
	CAmortBond(COraLoader &OraLoader, LPCTSTR Type, double NomAmount, double Price, 
			double FxRate, double BondFact = 1, double AmortFact = 1, LPCTSTR Dir = P) : 
			CBond(OraLoader, Type, NomAmount, Price, FxRate, BondFact, Dir) { SetAmortFact(AmortFact); }
	CAmortBond(COraLoader &OraLoader, LPCTSTR Type, double NomAmount, double Price, 
			double FxRate, LPCTSTR AssetType = EQUITYTYPE, double AmortFact = 1, 
			LPCTSTR Dir = P) :
			CBond(OraLoader, Type, NomAmount, Price, FxRate, AssetType, Dir) { SetAmortFact(AmortFact); }
	~CAmortBond() {}

	void Setup(COraLoader &OraLoader, LPCTSTR Type, double NomAmount, double Price, 
			double FxRate, double BondFact = 1, double AmortFact = 1, LPCTSTR Dir = P)
			{ CBond::Setup(OraLoader, Type, NomAmount, Price, FxRate, BondFact, Dir); 
				SetAmortFact(AmortFact); }
	void Setup(COraLoader &OraLoader, LPCTSTR Type, double NomAmount, double Price, 
			double FxRate, LPCTSTR AssetType = EQUITYTYPE, double AmortFact = 1, 
			LPCTSTR Dir = P)
			{ CBond::Setup(OraLoader, Type, NomAmount, Price, FxRate, AssetType, Dir); 
				SetAmortFact(AmortFact); }

	void SetAmortFact(double AmortFact = 1) { m_AmortFact = AmortFact; }
	double GetAmortFact() { return m_AmortFact; }
};
#endif