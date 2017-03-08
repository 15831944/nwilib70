#include "stdafx.h"
#include <math.h>
#include "bsopt.h"

IMPLEMENT_DYNAMIC(CBSOpt, CObject)

CBSOpt::CBSOpt(CBSOpt &BSOpt)
{
	Setup(BSOpt.GetOptType(), BSOpt.GetPrice(), BSOpt.GetStrike(), BSOpt.GetVolatility(),
			BSOpt.GetRate(), BSOpt.GetYearToExp(), BSOpt.GetDividendYield());
}

void CBSOpt::Setup(LPCTSTR OptType, double Price, double Strike, double Volatility, 
			  double Rate, double YearToExp, double DividendYield)
{
	m_PI = 3.141592653589793238462643;
	
	SetOptType(OptType);
	SetPrice(Price);
	SetStrike(Strike);
	SetVolatility(Volatility);
	SetRate(Rate);
	SetYearToExp(YearToExp);
	SetDividendYield(DividendYield);
}

double CBSOpt::StandardNormalDistribution(double x)
{
	return exp(-.5*x*x)/sqrt(2.0*m_PI);
}

double CBSOpt::CulmulativeStandardNormalDistribution(double x)
{
	double b1 = .31938153, b2 = -.356563782, b3 = 1.781477937;
	double b4 = -1.821255978, b5 = 1.330274429;
	double n, t;

	if(x > 6.0)
		return 1.0;
	if(x < -6.0)
		return 0.0;

	t = 1.0/(1.0 + .2316419*fabs(x));
	n = 1.0 - 1.0/sqrt(2*m_PI)*exp(-x*x/2)*((((b5*t + b4)*t + b3)*t + b2)*t + b1)*t;
	if(x < 0.0)
		n = 1.0 - n;
	
	return n;
}

double CBSOpt::BSD1()
{
	return log(m_Price/m_Strike) + (m_Rate - m_DividendYield + m_Volatility*m_Volatility/
				2*m_YearToExp)/(m_Volatility*sqrt(m_YearToExp));
}

double CBSOpt::BSD2()
{
	return BSD1() - m_Volatility*sqrt(m_YearToExp);
}

double CBSOpt::BSOptPrice(LPCTSTR OptType, double Price, double Strike, double Volatility, 
						double Rate, double YearToExp, double DividendYield)
{
	Setup(OptType, Price, Strike, Volatility, Rate, YearToExp, DividendYield);
	return BSOptPrice();
}

double CBSOpt::BSOptPrice()
{
	if(m_OptType == "CALL")
	{
		if(m_DividendYield > 0)
			return m_Price*exp(-m_DividendYield*m_YearToExp)*
						CulmulativeStandardNormalDistribution(BSD1()) - 
					m_Strike*exp(-m_Rate*m_YearToExp)*
						CulmulativeStandardNormalDistribution(BSD2());
		else
			return m_Price*CulmulativeStandardNormalDistribution(BSD1()) - 
					m_Strike*exp(-m_DividendYield*m_YearToExp)*
						CulmulativeStandardNormalDistribution(BSD2());
	}
	else
	{
		if(m_DividendYield > 0)
			return m_Price*exp(-m_DividendYield*m_YearToExp)*
						(CulmulativeStandardNormalDistribution(BSD1()) - 1) - 
					m_Strike*exp(-m_Rate*m_YearToExp)*
						(CulmulativeStandardNormalDistribution(BSD1()) - 1);
		else
			return m_Price*(CulmulativeStandardNormalDistribution(BSD1()) - 1) - 
					m_Strike*exp(-m_Rate*m_YearToExp)*
						(CulmulativeStandardNormalDistribution(BSD2()) - 1);
	}
}

double CBSOpt::BSDelta()
{
	if(m_OptType == "CALL")
		return exp(-m_DividendYield*m_YearToExp)*CulmulativeStandardNormalDistribution(BSD1());
	else
		return exp(-m_DividendYield*m_YearToExp)*
				(CulmulativeStandardNormalDistribution(BSD1()) - 1);
}

double CBSOpt::BSDelta(LPCTSTR OptType, double Price, double Strike, double Volatility, 
						double Rate, double YearToExp, double DividendYield)
{
	Setup(OptType, Price, Strike, Volatility, Rate, YearToExp, DividendYield);
	return BSDelta();
}

double CBSOpt::BSGamma()
{
	return exp(-m_DividendYield*m_YearToExp)*StandardNormalDistribution(BSD1())/
			(m_Price*m_Volatility*sqrt(m_YearToExp));
}

double CBSOpt::BSGamma(LPCTSTR OptType, double Price, double Strike, double Volatility, 
						double Rate, double YearToExp, double DividendYield)
{
	Setup(OptType, Price, Strike, Volatility, Rate, YearToExp, DividendYield);
	return BSGamma();
}

double CBSOpt::BSVega()
{
	return exp(-m_DividendYield*m_YearToExp)*m_Price*sqrt(m_YearToExp)*
			StandardNormalDistribution(BSD1())/100;
}

double CBSOpt::BSVega(LPCTSTR OptType, double Price, double Strike, double Volatility, 
					double Rate, double YearToExp, double DividendYield)
{
	Setup(OptType, Price, Strike, Volatility, Rate, YearToExp, DividendYield);
	return BSVega();
}

double CBSOpt::BSRho()
{
	double d1, d2, Rho;

	d1 = BSD1();
	d2 = BSD2();

	Rho = (exp(-m_DividendYield*m_YearToExp)*sqrt(m_YearToExp)*m_Price*
				StandardNormalDistribution(d1)/m_Volatility + 
			m_YearToExp*m_Strike*exp(-m_Rate*m_YearToExp)*
				CulmulativeStandardNormalDistribution(d2) - 
			m_Strike*exp(-m_Rate*m_YearToExp)*sqrt(m_YearToExp)*
				StandardNormalDistribution(d2)/m_Volatility)/100;
	
	if(m_OptType == "PUT")
		Rho -= m_YearToExp*m_Strike*exp(-m_Rate*m_YearToExp)/100;
	
	return Rho;
}

double CBSOpt::BSRho(LPCTSTR OptType, double Price, double Strike, double Volatility, 
					double Rate, double YearToExp, double DividendYield)
{
	Setup(OptType, Price, Strike, Volatility, Rate, YearToExp, DividendYield);
	return BSRho();
}

double CBSOpt::BSTheta()
{
	double d1, d2, Theta;

	d1 = BSD1();
	d2 = BSD2();
	
	Theta = (m_DividendYield*exp(-m_DividendYield*m_YearToExp)*m_Price*
				CulmulativeStandardNormalDistribution(d1) -
			m_Rate*exp(-m_Rate*m_YearToExp)*m_Strike*
				CulmulativeStandardNormalDistribution(d2) -
			m_Strike*exp(-m_Rate*m_YearToExp)*m_Volatility*StandardNormalDistribution(d2)/
				(2*sqrt(m_YearToExp)))/365;
	
	if(m_OptType == "PUT")
		Theta += (m_Rate*exp(-m_Rate*m_YearToExp)*m_Strike - 
					m_DividendYield*exp(-m_DividendYield*m_YearToExp)*m_Price)/365;
	return Theta;
}

double CBSOpt::BSTheta(LPCTSTR OptType, double Price, double Strike, double Volatility, 
				double Rate, double YearToExp, double DividendYield)
{
	Setup(OptType, Price, Strike, Volatility, Rate, YearToExp, DividendYield);
	return BSTheta();
}

double CBSOpt::BSImpliedVolatility(double OptPrice)
{
	double Converge = 0.00001, Diff, Vol0, Vol1;
	int i = 0;

	Vol1 = m_Volatility;
	Diff = OptPrice - BSOptPrice();
	for(;;)
	{
		Vol0 = Vol1;
		Vol1 = Vol0 + Diff/BSVega()/100;
		i ++;
		Diff = OptPrice - BSOptPrice(m_OptType, m_Price, m_Strike, Vol1, m_Rate, m_YearToExp, 
									m_DividendYield);
		if(fabs(Vol0 - Vol1) < Converge)
			return Vol1;
		if(i > 100)
			return 0;
	}
}

double CBSOpt::BSImpliedVolatility(LPCTSTR OptType, double Price, double Strike, 
								   double Volatility, double Rate, double YearToExp, 
								   double DividendYield, double OptPrice)
{
	Setup(OptType, Price, Strike, Volatility, Rate, YearToExp, DividendYield);
	return BSImpliedVolatility(OptPrice);
}
