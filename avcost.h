#ifndef __AVCOST_H__
#define __AVCOST_H__

#ifndef __NWIVAR_H__
#include "nwivar.h"
#endif

#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif

extern const int AVCOST_NEW;

class CAvCost : public CObject
{
DECLARE_DYNAMIC(CAvCost)
protected:
	COraLoader m_OraLoader;
	CStringArray m_IDRec;
	CString m_Dir;
	int m_nDir;
	double m_NomAmount;
	double m_Price;
	double m_FxRate;
	double m_HostPrice;
	double m_AmortFact;
	double m_BondFact;
	CString m_TransType;
	BOOL m_bFuture;

	CStringArray m_RunIDRec;
	CString m_RunDir;
	int m_nRunDir;
	double m_RunNomAmount;
	double m_RunPrice;
	double m_RunFxRate;
	double m_RunHostPrice;
	double m_RunAmortFact;
	double m_RunBondFact;
	double m_NetFxProfit;
	double m_NetCapProfit;
	double m_CapProfit;
	double m_FxProfit;

protected:
	double PriceAdjust(double &Price, LPCTSTR Dir, double OptPrice, LPCTSTR OptDir);
	int CompareDir();
	void AmortFactAdjust();
	int GetnDir(LPCTSTR Dir);

public:
	CAvCost();
	CAvCost(CAvCost &AvCost);
	CAvCost(COraLoader &OraLoader, CStringArray &IDRec);
	~CAvCost() {}

	COraLoader &GetOraLoader() { return m_OraLoader; }
    CStringArray& GetIDRec() { return m_IDRec; }
	LPCTSTR GetIDRecAt(CStringArray &IDRec, int Index);
	CString &GetDir() { return m_Dir; }
	int GetnDir() { return m_nDir; }
	double GetNomAmount() { return m_NomAmount; }
	double GetPrice() { return m_Price; }
	double GetHostPrice() { return m_HostPrice; }
	double GetFxRate() { return m_FxRate; }
	double GetBondFact() { return m_BondFact; }
	double GetAmortFact() { return m_AmortFact; }

	void SetDir(LPCTSTR Dir) { m_Dir = Dir; m_nDir = GetnDir(Dir); }
	void SetNomAmount(double NomAmount) { m_NomAmount = NomAmount; }
	void SetPrice(double Price) { m_Price = Price; }
	void SetHostPrice(double HostPrice) { m_HostPrice = HostPrice; }
	void SetFxRate(double FxRate) { m_FxRate = FxRate; }
	void SetBondFact(double BondFact) { m_BondFact = BondFact; }
	void SetAmortFact(double AmortFact) { m_AmortFact = AmortFact; }

    CStringArray& GetRunIDRec() { return m_RunIDRec; }
	CString &GetRunDir() { return m_RunDir; }
	int GetnRunDir() { return m_nRunDir; }
	double GetRunNomAmount() { return m_RunNomAmount; }
	double GetRunPrice() { return m_RunPrice; }
	double GetRunHostPrice() { return m_RunHostPrice; }
	double GetRunFxRate() { return m_RunFxRate; }
	double GetRunBondFact() { return m_RunBondFact; }
	double GetRunAmortFact() { return m_RunAmortFact; }

	void SetRunDir(LPCTSTR RunDir) { m_RunDir = RunDir; m_nRunDir = GetnDir(RunDir); }
	void SetRunNomAmount(double RunNomAmount) { m_RunNomAmount = RunNomAmount; }
	void SetRunPrice(double RunPrice) { m_RunPrice = RunPrice; }
	void SetRunHostPrice(double RunHostPrice) { m_RunHostPrice = RunHostPrice; }
	void SetRunFxRate(double RunFxRate) { m_RunFxRate = RunFxRate; }
	void SetRunBondFact(double RunBondFact) { m_RunBondFact = RunBondFact; }
	void SetRunAmortFact(double RunAmortFact) { m_RunAmortFact = RunAmortFact; }

	void SetNetCapProfit(double NetCapProfit = 0) { m_NetCapProfit = NetCapProfit; }
	double GetNetCapProfit() { return m_NetCapProfit; }

	void SetNetFxProfit(double NetFxProfit = 0) { m_NetFxProfit = NetFxProfit; }
	double GetNetFxProfit() { return m_NetFxProfit; }

	void SetCapProfit(double CapProfit = 0) { m_CapProfit = CapProfit; }
	double GetCapProfit() { return m_CapProfit; }

	void SetFxProfit(double FxProfit = 0) { m_FxProfit = FxProfit; }
	double GetFxProfit() { return m_FxProfit; }

	void Assign(CStringArray &IDRec, LPCTSTR Type, double NomAmount, double Price, 
				double FxRate, LPCTSTR Dir = P, double OptPrice = 0, LPCTSTR OptDir = "P", 
				double BondFact = 1, double AmortFact = 1, BOOL bFuture = FALSE);
	int Update();
	int Compute();

	int IsDiff();
};
#endif