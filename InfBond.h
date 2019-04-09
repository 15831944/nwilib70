#ifndef __INFBOND_H__
#define __INFBOND_H__

#ifndef __AMORTBOND_H__
#include "amortbond.h"
#endif

class CInfBond : public CAmortBond
{
DECLARE_DYNAMIC(CInfBond)

protected:
	
protected:
	virtual double ComputeInterest(double Amount, double Rate, int Days, int YearBase);
	virtual double ComputeInterest(double Amount, double Rate, LPCTSTR StartDate, LPCTSTR EndDate, 
									LPCTSTR RateBasis, int *IntDays, int *IntDays2);

public:
	CInfBond() { SetFormula(); }
	CInfBond(COraLoader &OraLoader, CInfBond &InfBond) : 
			CAmortBond(InfBond) { SetFormula(InfBond.GetFormula()); }
	CInfBond(COraLoader &OraLoader, LPCTSTR Type, double NomAmount, double Price, double FxRate, double BondFact = 1, double AmortFact = 1, LPCTSTR Dir = P) : 
			CBond(OraLoader, Type, NomAmount, Price, FxRate, BondFact, Dir) { SetAmortFact(AmortFact); }
	CAmortBond(COraLoader &OraLoader, LPCTSTR Type, double NomAmount, double Price, double FxRate, LPCTSTR AssetType = EQUITYTYPE, double AmortFact = 1, 
			LPCTSTR Dir = P) :
			CBond(OraLoader, Type, NomAmount, Price, FxRate, AssetType, Dir) { SetAmortFact(AmortFact); }
	~CAmortBond() {}

	void Setup(COraLoader &OraLoader, LPCTSTR Type, double NomAmount, double Price, double FxRate, double BondFact = 1, double AmortFact = 1, LPCTSTR Dir = P)
			{ CBond::Setup(OraLoader, Type, NomAmount, Price, FxRate, BondFact, Dir); 
				SetAmortFact(AmortFact); }
	void Setup(COraLoader &OraLoader, LPCTSTR Type, double NomAmount, double Price, double FxRate, LPCTSTR AssetType = EQUITYTYPE, double AmortFact = 1, 
			LPCTSTR Dir = P)
			{ CBond::Setup(OraLoader, Type, NomAmount, Price, FxRate, AssetType, Dir); 
				SetAmortFact(AmortFact); }

	void SetAmortFact(double AmortFact = 1) { m_AmortFact = AmortFact; }
	double GetAmortFact() { return m_AmortFact; }
};
#endif