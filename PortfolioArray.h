#ifndef __PORTFOLIOARRAY_H__
#define __PORTFOLIOARRAY_H__

#ifndef __DBRECARRAY_H__
#include "dbrecarray.h"
#endif


class CPortfolioArray : public CDBRecArray
{
DECLARE_DYNAMIC(CPortfolioArray)
protected:
	CString m_Portfolio;
	CString m_Custodian;
	CString m_Account;

public:
	CPortfolioArray() { m_Portfolio.Empty(); }
	CPortfolioArray(CPortfolioArray &PortfolioArray);
	~CPortfolioArray() {}

	void SetPortfolio(LPCTSTR Portfolio);
	CString &GetPortfolio() { return m_Portfolio; }
	CString &GetCustodian() { return m_Custodian; }
	CString &GetAccount() { return m_Account; }
};
#endif // __PORTFOLIOARRAY_H__