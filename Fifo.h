#ifndef __FIFO_H__
#define __FIFO_H__

#ifndef __FIFODATA_H__
#include "fifodata.h"
#endif

#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif

#ifndef __REPORTWRITER_H__
#include "reportwriter.h"
#endif

class CFifo : public CObject
{
DECLARE_DYNAMIC(CFifo)
protected:
	CStdioFile m_File;
	CFifoDataArray m_Array;
	COraLoader m_OraLoader;
	COraLoader m_OraLoader2;
	CString m_Date;	
	CString m_Portfolio;
	CString m_Asset;
	CString m_DealType;
	CString m_TradeDate;
	CString m_Ticket;
	CString m_ClDate;
	CString m_TransType;
	CString m_TrType;
	CString m_Currency;
	CString m_Dir;
	CString m_ODir;
	CString m_AssetType;
	CString m_OriCurrency;
	CString m_OriTradeDate;
	BOOL m_bFuture;
	int m_TransNum;
	int m_OptNum;
	double m_Price;
	double m_Fxrate;
	double m_OptPrice;
	double m_OptFxrate;
	double m_Amount;
	double m_AmortFact;
	int m_BreakDay;
	int m_BreakMonth;
	int m_BreakYear;
	int m_Pages;
	int m_Lines;
	BOOL m_bChanged;

public:
	CFifo() {};
	CFifo(COraLoader &OraLoader) { SetOraLoader(OraLoader); }
	~CFifo() { m_Array.RemoveAll(); }

protected:	
	void AssignEntry(CFifoData &Data);
	int AddEntry(CFifoData &Data);
	int CountEntry();
	int FindEntry();
	void ComputeEntryPL(CFifoData &Data, double usd_price, double bond_fact);
	void AdjustAmortFact(double amort_fact, double bond_fact);
	int SetupPreSql(CString &Portfolios, CString &Conditon);
	BOOL SetupSql(CString &Portfolios, CString &Condition);
	void DoFetch();
	BOOL IsAll(CString &String);
	double GetFxrate(LPCTSTR Date);
	double GetOptFxrate();
	void SaveResult(CFifoData &Data);
	double PriceAdjust();

	void SetChanged(BOOL bChanged = TRUE) { m_bChanged = bChanged; }
	BOOL GetChanged() { return m_bChanged; }

	void SetBreakDate(LPCTSTR BreakDate);
	int CompareBreakDate(LPCTSTR Date);
	int CompareDate(LPCTSTR Date1, LPCTSTR Date2);
	int DateHolding(LPCTSTR Date1, LPCTSTR Date2, int IsShort);
	void GetDayMonthYear(LPCTSTR Date, int &Day, int &Month, int &Year);
	int GetJulianDays(LPCTSTR date);

	BOOL WritePage();
	void WritePageHeading(LPCTSTR Portfolio);	
	void WriteBreakLine(LPCTSTR Portfolio, double Profit);
	void WriteLine(LPCTSTR Portfolio, CFifoData &Data);
	void WriteSTotalLine(LPCTSTR Portfolio, double Profit, double InProfit);
	void WriteTotalLine(LPCTSTR Portfolio, double Profit, double InProfit);

public:
	void SetOraLoader(COraLoader &OraLoader) { m_OraLoader.Copy(OraLoader); m_OraLoader2.Copy(OraLoader); }
	int RunFifo(LPCTSTR Date, CString &Portfolios, CString &Condition, LPCTSTR BreakDate = NULL);
	int RunFifo(COraLoader &OraLoader, LPCTSTR Date, CString &Portfolios, 
				CString &Condition, LPCTSTR BreakDate = NULL);
};
#endif 

