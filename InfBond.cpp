#include "stdafx.h"
#include "amortbond.h"

IMPLEMENT_DYNAMIC(CAmortBond, CBond)
double CAmortBond::ComputeValue(double NomAmount, double Price)
{
	return CBond::ComputeValue(NomAmount, Price)*GetAmortFact();
}

double CAmortBond::ComputeInterest(double Amount, double Rate, int Days, int YearBase)
{
	return CBond::ComputeInterest(Amount, Rate, Days, YearBase)*GetAmortFact();
}

double CAmortBond::ComputeInterest(double Amount, double Rate, LPCTSTR StartDate, 
						LPCTSTR EndDate, LPCTSTR RateBasis, int *IntDays, int *IntDays2)
{
	return CBond::ComputeInterest(Amount, Rate, StartDate, EndDate, RateBasis, IntDays, IntDays2)*GetAmortFact();
}
