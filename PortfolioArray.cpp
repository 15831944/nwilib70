#include "stdafx.h"
#include "portfolioarray.h"

IMPLEMENT_DYNAMIC(CPortfolioArray, CDBRecArray)
CPortfolioArray::CPortfolioArray(CPortfolioArray &PortfolioArray)
{
	int i;

	RemoveAll();
	for(i = 0; i < PortfolioArray.GetSize(); i ++)
		Add(*PortfolioArray.GetAt(i));
	
	SetPortfolio(PortfolioArray.GetPortfolio());
}

void CPortfolioArray::SetPortfolio(const char* Portfolio)
{	
	if(!Portfolio || strlen(Portfolio) == 0)
	{
		m_Portfolio.Empty();
		m_Custodian.Empty();
		m_Account.Empty();
	}
	else
	{
		int i;

		m_Portfolio = Portfolio;
		i = Find(Portfolio, 0);
		if(i >= 0)
		{
			m_Custodian = GetAt(i)->GetAt(1);
			m_Account = GetAt(i)->GetAt(2);
		}
	}
}
