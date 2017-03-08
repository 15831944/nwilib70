#ifndef __ORALOADER_H__
#define __ORALOADER_H__

#ifndef __ORADSET_H__
#include "oradset.h"
#endif

#ifndef __DBREC_H__
#include "dbrec.h"
#endif

#ifndef __KEYDBLIST_H__
#include "keydblist.h"
#endif

#ifndef __ROWCTRL_H__
#include "rowctrl.h"
#endif

class COraLoader : public COraDset
{
protected:
	CString	m_DataRec;
	CString m_DataBuf;
	CString	m_Separator;
	int	m_nCase;

public:
	COraLoader(ODatabase *pDB);
	COraLoader(COraDset &OraDset);
	COraLoader(COraLoader &OraLoader) { Copy(OraLoader); }
	COraLoader();
	~COraLoader() {}

protected:
	void AdjustCharCase();
	int RecordToLine();
	BOOL GetTPlusNDate(CString &Date, LPCTSTR SrcDate, int PlusDays, LPCTSTR Currency, BOOL bCDS = FALSE);

public: // Data Interfaces
	void Copy(COraLoader &OraLoader);
	const COraLoader &operator=(COraLoader &OraLoader) { Copy(OraLoader); return *this; }

	void SetCase(int Case = CASESENSITIVE) { m_nCase = Case; }
	void SetDataBuf(LPCTSTR DataBuf = NULL) { m_DataBuf = DataBuf ? DataBuf : EMPTYSTRING; }
	void SetDataRec(LPCTSTR DataRec = NULL) { m_DataRec = DataRec ? DataRec : EMPTYSTRING; }
	void SetSeparator(LPCTSTR Separator = TAB) { m_Separator = Separator ? Separator : EMPTYSTRING; }

	int	GetCase() { return m_nCase; }
	CString &GetDataBuf() { return m_DataBuf; }
	CString &GetDataRec() { return m_DataRec; }
	CString &GetSeparator() { return m_Separator; }

public: // Data Conversion
	CString &GetDBString(int Index);

public: // Interfaces
	CString &DateStamp(CString &Date);
	int GetHolidays(LPCTSTR FromDate, LPCTSTR ToDate, LPCTSTR Currency = "USD");
	int GetCount(LPCTSTR Sql = NULL);
	CString &Today(CString &Date);
	CString &TPlusN(CString &Date, LPCTSTR SrcDate, int Days, LPCTSTR Currency = "USD", BOOL bCDS = FALSE);
	CString &TPlus1(CString &Date, LPCTSTR SrcDate, LPCTSTR Currency = "USD", BOOL bCDS = FALSE);
	CString &TPlus2(CString &Date, LPCTSTR SrcDate, LPCTSTR Currency = "USD", BOOL bCDS = FALSE);
	CString &TPlus3(CString &Date, LPCTSTR SrcDate, LPCTSTR Currency = "USD", BOOL bCDS = FALSE);
	CString &TMinus1(CString &Date, LPCTSTR SrcDate, LPCTSTR Currency = "USD", BOOL bCDS = FALSE);
	CString &TMinus3(CString &Date, LPCTSTR SrcDate, LPCTSTR Currency = "USD", BOOL bCDS = FALSE);
	CString &GetValueDate(LPCTSTR SrcDate, const CString TransType, BOOL bListed, BOOL bFuture = FALSE);
	CString &OptExpDate(CString &Date, LPCTSTR SrcDate);
	int LoadComboBox(COptComboBox &CBox);
	int LoadMCComboBox(CMCComboBox &MCComboBox, int KeyCol = 0);
	int LoadListBox(CHLBox &ListBox);
	int LoadStringArray(CStringArray &StrArray);
	int LoadDBRecArray(CDBRecArray &DBRecArray);
	int LoadSheet(CWorkSheet &SS, int StartCol = 1, int StartRow = 1, BOOL Locked = FALSE);
	int LoadKeyDBList(CKeyDBList &KeyDBList);
	int LoadDBSheet(CDBSheet &SS, BOOL Locked = FALSE);
	int LoadDBSheetData(CDBSheet &SS, BOOL Locked = FALSE);	
	int LoadRowCtrl(CRowCtrl &RowCtrl, BOOL Locked = FALSE, BOOL withKey = FALSE);
	int LoadDBRec(CDBRec &DBRec);
	int LoadText(CWnd &Ctrl);
	int LoadTextString(CString &String);
	BOOL InCurrentMonth(LPCTSTR Date);
	
public: // Record Interfaces
	BOOL RecToQueryRec(CStringArray &QRec, CStringArray &DBRec, CFieldArray *FieldArray = NULL, 
						LPCTSTR Extra = NULL);
	BOOL RecToQueryRec(CStringArray &QRec, CDBRec &DBRec, CFieldArray *FieldArray = NULL,
						LPCTSTR Extra = NULL);
	BOOL RecToQueryRec(CDBSheet &SS, CDBRec &DBRec);
	BOOL SetSqlSearchInfo(CString &Sql, CStringArray &QRec, CFieldArray *FieldArray = NULL, 
							BOOL bAllAnd = FALSE, BOOL bFindExact = FALSE);
	LPCTSTR ToDateValue(CString &Date, OValue &Value);

	BOOL UpdateRecord(CDBRec &DBRec, BOOL bNew = FALSE);

public: // Database Operation	
	int	Compute();

public:
	virtual int	SetupData() { return 1; };
	virtual	int ProcessData() { return 1; };
	virtual int RetrieveData() { return 1; };
	double GenerateUniqueID(LPCTSTR IDCode, CString &IDValue);
};
#endif // __ORALOADER_H__