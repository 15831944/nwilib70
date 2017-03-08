#ifndef __DPBASE_H__
#define __DPBASE_H__

#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif

#ifndef __QDATA_H__
#include "qdata.h"
#endif

class CDPBase : public CObject
{
DECLARE_DYNAMIC(CDPBase)

protected:
	COraLoader m_OraLoader;
	COraLoader m_OraLoader2;
	CStringArray m_FileSets;
	CString m_Portfolios;
	CString m_Conditions;
	CString m_PrevDate;
	CString m_StartDate;
	CString m_Date;
	BOOL m_bReady;

protected:
	void SetQueryDate(CString &Date, LPCTSTR SrcDate) { CQData QData; Date = QData.GetQueryDate(SrcDate); }
	BOOL SetFileSets();
	BOOL IsAll(LPCTSTR InStmt);
	void ToInStmt(CString &InStmt, CStringArray &List);
	double GetFactor(OValue &Value, double Default = 1);
	char GetCharVal(OValue &Value, char Default = ' ');
	BOOL GetBOOLVal(OValue &Value, LPCTSTR YesStr, BOOL Default = FALSE);
	virtual double GetFxrate(double &Fxrate, LPCTSTR Currency, LPCTSTR Date);

public:
	CDPBase() { m_bReady = FALSE; }
	CDPBase(COraLoader &OraLoader, CStringArray &Portfolios, CStringArray &Conditions,
			LPCTSTR PrevDate, LPCTSTR StartDate, LPCTSTR Date);
	~CDPBase() { }

	void Setup(COraLoader &OraLoader, CStringArray &Portfolios, CStringArray &Conditions,
			LPCTSTR PrevDate, LPCTSTR StartDate, LPCTSTR Date);

	void Setup(COraLoader &OraLoader, LPCTSTR Portfolios, LPCTSTR Conditions,
			LPCTSTR PrevDate, LPCTSTR StartDate, LPCTSTR Date);

	COraLoader &GetOraLoader() { return m_OraLoader; }
	COraLoader &GetOraLoader2() { return m_OraLoader2; }

	CString &GetPrevDate() { return m_PrevDate; }
	CString &GetStartDate() { return m_StartDate; }
	CString &GetDate() { return m_Date; }
	CString &GetPortfolios() { return m_Portfolios; }
	CString &GetConditions() { return m_Conditions; }
	CStringArray &GetFileSets() { return m_FileSets; }

	void SetOraLoader(COraLoader &OraLoader);
	void SetPrevDate(LPCTSTR PrevDate) { SetQueryDate(m_PrevDate, PrevDate); }
	void SetStartDate(LPCTSTR StartDate) { SetQueryDate(m_StartDate, StartDate); }
	void SetDate(LPCTSTR Date) { SetQueryDate(m_Date, Date); }
	void SetPortfolios(LPCTSTR Portfolios) { m_Portfolios = Portfolios; }
	void SetPortfolios(CStringArray &Portfolios) { ToInStmt(m_Portfolios, Portfolios); }
	void SetConditions(LPCTSTR Conditions) { m_Conditions = Conditions; }
	void SetConditions(CStringArray &Conditions) { ToInStmt(m_Conditions, Conditions); }

	virtual BOOL DeleteDataFromTable() { return TRUE; }
	virtual BOOL PreProcess() { return TRUE; }
	virtual BOOL SetupSql() { return TRUE; }
	virtual BOOL AddDataToTable() { return TRUE; }
	virtual BOOL DoWork();
	virtual BOOL PrepareData() { return TRUE; }
	virtual BOOL ComputeData() { return TRUE; }
};
#endif