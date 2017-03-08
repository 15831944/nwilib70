#ifndef __QBASE_H__
#define __QBASE_H__

#ifndef __AVCOST_H__
#include "avcost.h"
#endif

#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif

#ifndef __QDATA_H__
#include "qdata.h"
#endif

class CQBase : public CAvCost
{
DECLARE_DYNAMIC(CQBase)

protected:
	COraLoader m_OraLoader;
	COraLoader m_OraLoader2;
	CString m_Portfolios;
	CString m_Conditions;
	CString m_FileSet;
	CString m_PrevDate;
	CString m_StartDate;
	CString m_Date;
	BOOL m_bAll;

protected:
	void ToInStmt(CString &InStmt, CStringArray &List);
	double GetFactor(OValue &Value, double Default = 1);
	char GetCharVal(char &Val, OValue &Value, char Default = ' ');
	BOOL GetBOOLVal(BOOL &Flag, OValue &Value, LPCTSTR YesStr, BOOL Default = FALSE);
	virtual double GetFxrate(double &Fxrate, LPCTSTR Currency, LPCTSTR Date);

	virtual BOOL UpdateTable() = 0;
	virtual BOOL ClearTable() = 0;
	virtual BOOL PreProcess() = 0;
	virtual BOOL SetupSql() = 0;
	virtual BOOL DoWork() = 0;

public:
	CQBase() {};
	CQBase(COraLoader &OraLoader, LPCTSTR FileSet, LPCTSTR PrevDate, LPCTSTR StartDate, 
			LPCTSTR Date, CStringArray &Portfolios, CStringArray &Conditions, BOOL bAll = FALSE);
	virtual ~CQBase() {}

	void Setup(COraLoader &OraLoader, LPCTSTR FileSet, LPCTSTR PrevDate, LPCTSTR StartDate, 
				LPCTSTR Date, CStringArray &Portfolios, CStringArray &Conditions, BOOL bAll = FALSE);

	COraLoader &GetOraLoader() { return m_OraLoader; }
	COraLoader &GetOraLoader2() { return m_OraLoader2; }
	BOOL GetAll() { return m_bAll; }
	CString &GetFileSet() { return m_FileSet; }
	CString &GetPrevDate() { return m_PrevDate; }
	CString &GetStartDate() { return m_StartDate; }
	CString &GetDate() { return m_Date; }
	CString &GetPortfolios() { return m_Portfolios; }
	CString &GetConditions() { return m_Conditions; }

	void SetOraLoader(COraLoader &OraLoader) { m_OraLoader = OraLoader; m_OraLoader2 = OraLoader; }
	void SetAll(BOOL bAll = FALSE) { m_bAll = bAll; }
	void SetFileSet(LPCTSTR FileSet) { CQData QData; QData.SetString(m_FileSet, FileSet); }
	void SetPrevDate(LPCTSTR PrevDate) { CQData QData; QData.SetString(m_PrevDate, PrevDate); }
	void SetStartDate(LPCTSTR StartDate) { CQData QData; QData.SetString(m_StartDate, StartDate); }
	void SetDate(LPCTSTR Date) { CQData QData; QData.SetString(m_Date, Date); }
	void SetPortfolios(CStringArray &Portfolios) { ToInStmt(m_Portfolios, Portfolios); }
	void SetConditions(CStringArray &Conditions) { ToInStmt(m_Conditions, Conditions); }

	virtual BOOL Process();
};
#endif
