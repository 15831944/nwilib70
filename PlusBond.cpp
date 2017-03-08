#include "stdafx.h"
#include "plusbond.h"


IMPLEMENT_DYNAMIC(CPlusBond, CAmortBond)
double CPlusBond::GetPlusAmount()
{
	if((m_Type == SECURITIES || m_Type == CDS || m_Type == REPO || m_Type == LEVERAGE) && GetPrePaid())
		return GetAmortFact()*GetNomAmount()*GetPlusAmountFactor()/100;

	return 0;
}
