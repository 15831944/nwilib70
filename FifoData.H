#ifndef __FIFODATA_H__
#define __FIFODATA_H__

class CFifoData : public CObject
{
DECLARE_DYNAMIC(CFifoData)
protected:	
	CString m_TradeDate;
	int m_TransNum;
	CString m_Dir;
	double m_Price;
	double m_Fxrate;
	double m_Cost;
	double m_Profit;
	double m_Amount;
	double m_Remain;
	double m_AmortFact;
	int m_Holding;
	BOOL m_bAddable;
   
public:
	CFifoData();
	CFifoData(CFifoData &FifoData) { Copy(FifoData); }
	virtual ~CFifoData() {}
    	
	void Copy(CFifoData &FifoData);
	const CFifoData &operator=(CFifoData &FifoData) { Copy(FifoData); return *this; }

	LPCTSTR GetTradeDate() { return (LPCTSTR) m_TradeDate; }
	void SetTradeDate(LPCTSTR TradeDate) { m_TradeDate = TradeDate; }
	
	LPCTSTR GetDir() { return (LPCTSTR) m_Dir; }
	void SetDir(LPCTSTR Dir) { m_Dir = Dir; }
	
	int GetTransNum() { return m_TransNum; }
	void SetTransNum(int TransNum) { m_TransNum = TransNum; }

	double GetPrice() { return m_Price; }
	void SetPrice(double Price) { m_Price = Price; }

	double GetFxrate() { return m_Fxrate; }
	void SetFxrate(double Fxrate) { m_Fxrate = Fxrate; }
	
	double GetCost() { return m_Cost; }
	void SetCost(double Cost) { m_Cost = Cost; }
	
	double GetProfit() { return m_Profit; }
	void SetProfit(double Profit) { m_Profit = Profit; }

	double GetAmount() { return m_Amount; }
	void SetAmount(double Amount) { m_Amount = Amount; }
	
	double GetRemain() { return m_Remain; }
	void SetRemain(double Remain) { m_Remain = Remain; }

	double GetAmortFact() { return m_AmortFact; }
	void SetAmortFact(double AmortFact) { m_AmortFact = AmortFact; }

	int GetHolding() { return m_Holding; }
	void SetHolding(int Holding) { m_Holding = Holding; }

	BOOL GetAddable() { return m_bAddable; }
	void SetAddable(BOOL bAddable = TRUE) { m_bAddable = bAddable; }
};


class CFifoDataArray : public CObArray
{
DECLARE_DYNAMIC(CFifoDataArray)

public:
	CFifoDataArray();
	~CFifoDataArray();

	int	Add(CFifoData *Rec);
	void SetAt(int Index, CFifoData *Rec);
	void InsertAt(int Index, CFifoData *Rec);
	int Add(CFifoData &Rec);
	void SetAt(int Index, CFifoData &Rec);
	void InsertAt(int Index, CFifoData &Rec);
	void RemoveAt(int Index);
	void RemoveAll();
	void Copy(CFifoDataArray &FifoDataArray);
	CFifoData *GetAt(int Index);
		
	const CFifoDataArray &operator=(CFifoDataArray &FifoDataArray) { Copy(FifoDataArray); return *this; }
};
#endif
