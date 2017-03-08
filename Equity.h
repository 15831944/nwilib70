#ifndef __EQUITY_H__
#define __EQUITY_H__

#ifndef __NWIVAR_H__
#include "nwivar.h"
#endif

#ifndef __DBBASE_H__
#include "dbbase.h"
#endif

#ifndef __QDATA_H__
#include "qdata.h"
#endif

extern double SECFEERATE;
extern double ORFEERATE;

class CEquity : public CObject
{
public:
	static double m_SecFeeRate;
	static double m_OrFeeRate;

DECLARE_DYNAMIC(CEquity)
protected:
	double m_NomAmount;
	double m_Price;
	double m_CalPrice;
	double m_FxRate;
	int	m_OutPrec;
	int	m_nDir;
	CString	m_Dir;
	CString	m_Type;

protected:
	double GetCalPrice() { return m_CalPrice; }
	virtual double ComputeValue(double NomAmount = -1, double Price = -1);

public:
	CEquity() { Setup(NULL, 0, 0, 1); }
	CEquity(CEquity& Eq);
	CEquity(LPCTSTR Type, double NomAmount, double Price, double FxRate, LPCTSTR Dir = P)
			{ Setup(Type, NomAmount, Price, FxRate, Dir); }
	~CEquity() {}

	void Setup(LPCTSTR Type, double NomAmount, double Price, double FxRate, LPCTSTR Dir = P);

	double GetNomAmount() { return m_NomAmount; }
	void SetNomAmount(double NomAmount) { m_NomAmount = NomAmount; }

	double GetPrice() { return m_Price; }
	void SetPrice(double Price);

	double GetFxRate() { return m_FxRate; }
	void SetFxRate(double FxRate);

	CString &GetDir() { return m_Dir; }
	void SetDir(LPCTSTR Dir = N);

	CString &GetType() { return m_Type; }
	virtual int GetnDir(LPCTSTR Dir = NULL);
	
	LPCTSTR DefineTransType(LPCTSTR TransType, LPCTSTR Dir);
	
	int GetOutPrec() { return m_OutPrec; }
	void SetOutPrec(int Prec = 2) { m_OutPrec = 2; }

	double GetOutput(double OutVal);
	double GetValue(BOOL bLocal = TRUE);

	double GetSecFees(double GrossPrice, BOOL bSec);
	double GetOrFees(double Par, BOOL bOr);
};
#endif