#ifndef __CAPBOND_H__
#define __CAPBOND_H__

#ifndef __PLUSBOND_H__
#include "plusbond.h"
#endif

class CCapBond : public CPlusBond
{
DECLARE_DYNAMIC(CCapBond)

protected:
	double m_CapRate;
	int m_nPreCapDays;
	int m_nPreCapDays2;
	int m_nCapYearBase;
	int m_nCapYearBase2;
	int m_nCapDays;
	int m_nCapDays2;
	CString m_PreCapDate;
	CString m_CapRateBasis;

public:
	CCapBond() { SetupCapInfo(EMPTYSTRING, EMPTYSTRING, EMPTYSTRING, EMPTYSTRING, 0); }
	CCapBond(CCapBond& CapBond);
	CCapBond(COraLoader &OraLoader, LPCTSTR Type, double NomAmount, double Price, 
			double FxRate, double BondFact = 1, double AmortFact = 1, 
			double PlusAmount = 0, LPCTSTR Dir = P);
	CCapBond(COraLoader &OraLoader, LPCTSTR Type, double NomAmount, double Price, 
			double FxRate, LPCTSTR AssetType = EQUITYTYPE, double AmortFact = 1, 
			double PlusAmount = 0, LPCTSTR Dir = P);
	~CCapBond() {}

	void SetupCapInfo(LPCTSTR PreCapDate, LPCTSTR ValueDate, LPCTSTR Date, 
					LPCTSTR RateBasis, double CapRate = 0);

	void SetCapRate(double CapRate = 0) { m_CapRate = CapRate; }
	void SetCapDays(int CapDays = 0) { m_nCapDays = CapDays; }
	void SetPreCapDays(int PreCapDays = 0) { m_nPreCapDays = PreCapDays; }
	void SetCapYearBase(int CapYearBase = 365) { m_nCapYearBase = CapYearBase; }

	double GetCapRate() { return m_CapRate; }
	CString &GetPreCapDate() { return m_PreCapDate; }
	int GetCapDays() { return m_nCapDays; }
	int GetCapDays2() { return m_nCapDays2; }
	int GetPreCapDays() { return m_nPreCapDays; }
	int GetPreCapDays2() { return m_nPreCapDays2; }
	int GetCapYearBase() { return m_nCapYearBase; }
	CString &GetCapRateBasis() { return m_CapRateBasis; }

	double GetPrePaidAccretion();
	double GetAccretion();
	double GetPrePaidAccretionValue();
	double GetAccretionValue();
};
#endif
