#include "stdafx.h"
#include "capbond.h"

IMPLEMENT_DYNAMIC(CCapBond, CPlusBond)
CCapBond::CCapBond(COraLoader &OraLoader, LPCTSTR Type, double NomAmount, 
				double Price, double FxRate, double BondFact, double AmortFact, 
				double PlusAmount, LPCTSTR Dir) :
			CPlusBond(OraLoader, Type, NomAmount, Price, FxRate, BondFact, 
					AmortFact, PlusAmount, Dir)
{ 
	SetupCapInfo(EMPTYSTRING, EMPTYSTRING, EMPTYSTRING, EMPTYSTRING, 0); 
}

CCapBond::CCapBond(COraLoader &OraLoader, LPCTSTR Type, double NomAmount, 
				double Price, double FxRate, LPCTSTR AssetType, double AmortFact, 
				double PlusAmount, LPCTSTR Dir) : 
			CPlusBond(OraLoader, Type, NomAmount, Price, FxRate, AssetType, AmortFact, 
					PlusAmount, Dir) 
{
	SetupCapInfo(EMPTYSTRING, EMPTYSTRING, EMPTYSTRING, EMPTYSTRING, 0);
}

CCapBond::CCapBond(CCapBond &CapBond) : CPlusBond(CapBond)
{
	SetupCapInfo(CapBond.GetPreCapDate(), CapBond.GetValueDate(), CapBond.GetDate(), 
				CapBond.GetCapRateBasis(), CapBond.GetCapRate());
}

void CCapBond::SetupCapInfo(LPCTSTR PreCapDate, LPCTSTR ValueDate, LPCTSTR Date, 
							LPCTSTR CapRateBasis, double CapRate)
{
	if(CapRate != 0)
	{
		CQData QData;

		SetCapRate(CapRate);
		QData.SetString(m_PreCapDate, PreCapDate);
		QData.SetString(m_CapRateBasis, CapRateBasis);
		m_nPreCapDays = ComputeDays(PreCapDate, ValueDate, CapRateBasis, m_nCapYearBase);
		m_nCapDays = ComputeDays(ValueDate, Date, CapRateBasis, m_nCapYearBase);
	}
	else
	{		
		SetCapRate();
		SetCapYearBase();
		SetPreCapDays();
		SetCapDays();
	}
}

double CCapBond::GetPrePaidAccretion()
{
	if((m_Type == SECURITIES || m_Type == CDS || m_Type == REPO || m_Type == LEVERAGE) && GetCapRate() != 0)
	{
		return CBond::ComputeInterest(GetNomAmount(), GetCapRate(), GetPreIntDate(), 
							GetValueDate(), GetRateBasis(), &m_nPreCapDays, &m_nPreCapDays2);
	}
	return 0;
}

double CCapBond::GetAccretion()
{
	if((m_Type == SECURITIES || m_Type == CDS || m_Type == REPO || m_Type == LEVERAGE) && GetCapRate() != 0)
		return CBond::ComputeInterest(GetNomAmount(), GetCapRate(), GetPreIntDate(), 
							GetDate(), GetRateBasis(), &m_nCapDays, &m_nCapDays2);

	return 0;
}

double CCapBond::GetPrePaidAccretionValue()
{
	return GetPrePaidAccretion()*GetCalPrice()*GetBondFact();
}

double CCapBond::GetAccretionValue()
{
	return GetAccretion()*GetCalPrice()*GetBondFact();
} 
                                                                                                                                                                        
