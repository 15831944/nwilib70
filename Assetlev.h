#ifndef __ASSETLEV_H__
#define __ASSETLEV_H__

#ifndef __CAPBOND_H__
#include "capbond.h"
#endif

#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif

class CAssetLev : public CCapBond
{
DECLARE_DYNAMIC(CAssetLev)

protected:
	double m_LevAmount;
	double m_LevInterest;
	double m_LevRate;
	CString m_LevRateBasis;
	CString m_Formula;
	BOOL m_bLev;
	int m_nLevDays;
	int m_nLevYearBase;

public:
	CAssetLev() { SetupLevInfo(EMPTYSTRING, EMPTYSTRING, EMPTYSTRING, 0, NULL); }
	CAssetLev(CAssetLev &AssetLev);
	CAssetLev(COraLoader &OraLoader, LPCTSTR Type, double NomAmount, double Price, 
			double FxRate, double BondFact = 1, double AmortFact = 1, double PlusAmount = 0, 
			LPCTSTR Dir = P) : CCapBond(OraLoader, Type, NomAmount, Price, FxRate, 
			BondFact, AmortFact, PlusAmount, Dir)
			{ SetupLevInfo(EMPTYSTRING, EMPTYSTRING, EMPTYSTRING, 0, NULL); }
	CAssetLev(COraLoader &OraLoader, LPCTSTR Type, double NomAmount, double Price, 
			double FxRate, LPCTSTR AssetType = EQUITYTYPE, double AmortFact = 1, 
			double PlusAmount = 0, LPCTSTR Dir = P) : CCapBond(OraLoader, Type, 
			NomAmount, Price, FxRate, AssetType, AmortFact, PlusAmount, Dir)
			{ SetupLevInfo(EMPTYSTRING, EMPTYSTRING, EMPTYSTRING, 0, NULL); }
	~CAssetLev() {}

protected:
	void SetLevDays(int Days = 0) { m_nLevDays = Days; }
	void AddLevDays(int Days) { m_nLevDays += Days; }
	void SetLevInterest(double Interest = 0) { m_LevInterest = Interest; }
	void AddLevInterest(double Interest) { m_LevInterest += Interest; }
	double ComputeLevInterest();
	virtual double ComputeValue(double NomAmount = -1, double Price = -1);

public:
	void SetupLevInfo(LPCTSTR ValueDate, LPCTSTR Date, LPCTSTR LevRateBasis, 
					double LevRate, LPCTSTR Formula);

	void SetLevRate(double LevRate = 0) { m_LevRate = LevRate; }
	double GetLevRate() { return !m_bLev ? 0 : m_LevRate; }
	CString &GetFormula() { return m_Formula; }
	CString &GetLevRateBasis() { return m_LevRateBasis; }
	int GetLevDays() { return m_nLevDays; }
	int GetLevYearBase() { return m_nLevYearBase; }

	double GetLevInterest() { return m_LevInterest; }
	double GetLevInterest(LPCTSTR ValueDate, LPCTSTR Date, double LevRate);
	double GetLevInterest(double Amount, LPCTSTR RateBasis, LPCTSTR ValueDate, 
						LPCTSTR Date, double LevRate);
	double GetDailyLevInterest(LPCTSTR FromDate, LPCTSTR ToDate, double LevRate = 0);
	double GetDailyLevInterest(double Amount, LPCTSTR FromDate, LPCTSTR ToDate, double LevRate);
	double GetDailyLevInterest(COraLoader &OraLoader, LPCTSTR Portfolio, LPCTSTR TransNum, 
								LPCTSTR InvNum);
	double GetDailyLevInterest(COraLoader &OraLoader, LPCTSTR Portfolio, LPCTSTR TransNum, 
								LPCTSTR FromDate, LPCTSTR ToDate);
	double GetDailyLevInterest(COraLoader &OraLoader, double Amount, LPCTSTR Portfolio, 
								LPCTSTR TransNum, LPCTSTR InvNum);
	double GetDailyLevInterest(COraLoader &OraLoader, double Amount, LPCTSTR Portfolio, 
								LPCTSTR TransNum, LPCTSTR FromDate, LPCTSTR ToDate);

	void SetLevAmount(double LevAmount) { m_LevAmount = LevAmount; }
	double GetLevAmount() { return m_LevAmount; }
	double GetLevRepayAmount();

	virtual int GetnDir(LPCTSTR Dir = NULL);
	double ComputeLevAmount(BOOL bRefresh = FALSE);
};
#endif
