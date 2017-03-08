#ifndef __BSOPT_H__
#define __BSOPT_H__

#ifndef __NWIVAR_H__
#include "nwivar.h"
#endif

#ifndef __DBBASE_H__
#include "dbbase.h"
#endif

#ifndef __QDATA_H__
#include "qdata.h"
#endif

class CBSOpt : public CObject
{
DECLARE_DYNAMIC(CBSOpt)
protected:
	CString m_OptType;
	double m_Price;
	double m_Strike;
	double m_Volatility;
	double m_Rate;
	double m_YearToExp;
	double m_DividendYield;
	double m_PI;

protected:
	double BSD1();
	double BSD2();

public:
	CBSOpt() { Setup(NULL, 0, 0, 0, 0, 0, 0); }
	CBSOpt(CBSOpt& BSOpt);
	CBSOpt(LPCTSTR OptType, double Price, double Strike, double Volatility, double Rate, 
			double YearToExp, double DividendYield)
			{ Setup(OptType, Price, Strike, Volatility, Rate, YearToExp, DividendYield); }
	~CBSOpt() {}

	void Setup(LPCTSTR OptType, double Price, double Strike, double Volatility, double Rate,
				double YearToExp, double DividendYield);
	double StandardNormalDistribution(double x);
	double CulmulativeStandardNormalDistribution(double x);
	double BSOptPrice();
	double BSOptPrice(LPCTSTR OptType, double Price, double Strike, double Volatility, 
						double Rate, double YearToExp, double DividendYield);
	double BSDelta();
	double BSDelta(LPCTSTR OptType, double Price, double Strike, double Volatility, 
						double Rate, double YearToExp, double DividendYield);
	double BSGamma();
	double BSGamma(LPCTSTR OptType, double Price, double Strike, double Volatility, 
						double Rate, double YearToExp, double DividendYield);
	double BSVega();
	double BSVega(LPCTSTR OptType, double Price, double Strike, double Volatility, 
						double Rate, double YearToExp, double DividendYield);
	double BSRho();
	double BSRho(LPCTSTR OptType, double Price, double Strike, double Volatility, 
						double Rate, double YearToExp, double DividendYield);
	double BSTheta();
	double BSTheta(LPCTSTR OptType, double Price, double Strike, double Volatility, 
						double Rate, double YearToExp, double DividendYield);
	double BSImpliedVolatility(double OptPrice);
	double BSImpliedVolatility(LPCTSTR OptType, double Price, double Strike, 
								double Volatility, double Rate, double YearToExp, 
								double DividendYield, double OptPrice);

	LPCTSTR GetOptType() { return (LPCTSTR) m_OptType; }
	void SetOptType(LPCTSTR OptType) { m_OptType = OptType; }

	double GetPrice() { return m_Price; }
	void SetPrice(double Price) { m_Price = Price; }

	double GetStrike() { return m_Strike; }
	void SetStrike(double Strike) { m_Strike = Strike; }

	double GetVolatility() { return m_Volatility; }
	void SetVolatility(double Volatility) { m_Volatility = Volatility; }

	double GetRate() { return m_Rate; }
	void SetRate(double Rate) { m_Rate = Rate; }

	double GetYearToExp() { return m_YearToExp; }
	void SetYearToExp(double YearToExp) { m_YearToExp = YearToExp; }
	
	double GetDividendYield() { return m_DividendYield; }
	void SetDividendYield(double DividendYield) { m_DividendYield = DividendYield; }
};
#endif