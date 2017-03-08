#ifndef __PLUSBOND_H__
#define __PLUSBOND_H__

#ifndef __AMORTBOND_H__
#include "amortbond.h"
#endif

class CPlusBond : public CAmortBond
{
DECLARE_DYNAMIC(CPlusBond)

protected:
	double m_PlusAmount;

public:
	CPlusBond() { SetPlusAmountFactor(); }
	CPlusBond(CPlusBond& PlusBond) : CAmortBond(PlusBond) 
			{ SetPlusAmountFactor(PlusBond.GetPlusAmountFactor()); }
	CPlusBond(COraLoader &OraLoader, LPCTSTR Type, double NomAmount, double Price, 
			double FxRate, double BondFact = 1, double AmortFact = 1, 
			double PlusAmount = 0, LPCTSTR Dir = P) 
			: CAmortBond(OraLoader, Type, NomAmount, Price, FxRate, BondFact, AmortFact, Dir) 
			{ SetPlusAmountFactor(PlusAmount); }
	CPlusBond(COraLoader &OraLoader, LPCTSTR Type, double NomAmount, double Price, 
			double FxRate, LPCTSTR AssetType = EQUITYTYPE, double AmortFact = 1, 
			double PlusAmount = 0, LPCTSTR Dir = P) 
			: CAmortBond(OraLoader, Type, NomAmount, Price, FxRate, AssetType, AmortFact, Dir) 
			{ SetPlusAmountFactor(PlusAmount); } 
	~CPlusBond() {}

	void Setup(COraLoader &OraLoader, LPCTSTR Type, double NomAmount, double Price, 
			double FxRate, double BondFact = 1, double AmortFact = 1, 
			double PlusAmount = 0, LPCTSTR Dir = P)
			{ CAmortBond::Setup(OraLoader, Type, NomAmount, Price, FxRate, BondFact, 
					AmortFact, Dir); 
					SetPlusAmountFactor(PlusAmount); }
	void Setup(COraLoader &OraLoader, LPCTSTR Type, double NomAmount, double Price, 
			double FxRate, LPCTSTR AssetType = EQUITYTYPE, double AmortFact = 1, 
			double PlusAmount = 0, LPCTSTR Dir = P)
			{ CAmortBond::Setup(OraLoader, Type, NomAmount, Price, FxRate, AssetType, 
					AmortFact, Dir); 
					SetPlusAmountFactor(PlusAmount); }

	void SetPlusAmountFactor(double PlusAmount = 0) { m_PlusAmount = PlusAmount; }
	double GetPlusAmountFactor() { return m_PlusAmount; }
	double GetPlusAmount();
	double GetPrePaidInt() { return CAmortBond::GetPrePaidInt() + GetPlusAmount(); }
};
#endif