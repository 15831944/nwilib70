#ifndef __QDATA_H__
#define __QDATA_H__

#ifndef __NWIVAR_H__
#include "nwivar.h"
#endif

#ifndef __DBRECARRAY_H__
#include "dbrecarray.h"
#endif

#ifndef __COMBOX_H__
#include "combox.h"
#endif

#ifndef __HLBOX_H__
#include "hlbox.h"
#endif

#ifndef __KEYDBLIST_H__
#include "keydblist.h"
#endif

class CQData
{
protected:
	CString m_DataBuf;

public:
	CQData() {}
	~CQData() {}

public: // Data Interfaces
	void SetDataBuf(LPCTSTR DataBuf = NULL) { m_DataBuf = DataBuf ? DataBuf : EMPTYSTRING; }
	LPCTSTR GetDataBuf() { return (LPCTSTR ) m_DataBuf; }
	void GetCSVToken(CString &ScrData, CString &Data);

public: // Data Conversion
	LPCTSTR GetQueryText(const char Src);
	LPCTSTR GetQueryText(LPCTSTR Src, LPCTSTR Extra = NULL);
	LPCTSTR GetQueryText(CString &Src, LPCTSTR Extra = NULL);
	LPCTSTR GetQueryDate(CString &SrcDate);
	LPCTSTR GetQueryDate(LPCTSTR SrcDate);
	LPCTSTR GetQueryNumber(LPCTSTR SrcNum);
	LPCTSTR GetQueryNumber(CString &SrcNum);

public: // Data Utilities
	LPCTSTR GetRecAt(CStringArray &QRec, int Index);
	BOOL IsValid(LPCTSTR Str, LPCTSTR ValidString);
	LPCTSTR FormatedDate(LPCTSTR Date);
	int YYToYYYY(int yy);
	void SetString(CString &Str, LPCTSTR pStr);
	BOOL SetSqlStmt(CString &Sql, CString &Value, LPCTSTR Field, BOOL bAnd);
	BOOL SetSqlStmt(CString &Sql, CEdit &Edit, LPCTSTR Field, BOOL bAnd);
	BOOL SetSqlStmt(CString &Sql, COptComboBox *Combo, LPCTSTR Field, BOOL bAnd);
	BOOL SetSqlStmt(CString &Sql, CHLBox *HLBox, LPCTSTR Field, BOOL bAnd);

public: // Numeric Function
	LPCTSTR RemoveComma(LPCTSTR SrcNum);
	LPCTSTR AddComma(LPCTSTR SrcNum);
	LPCTSTR Abs(LPCTSTR Num);
	LPCTSTR Abs(CString &Num);
	LPCTSTR	GetCSVString(LPCTSTR String);
	LPCTSTR	GetCSVNumber(LPCTSTR SrcNum);
	double Round(double Number, int Precision);
	double Trunc(double Number, int Precision);
	LPCTSTR WriteNumber(int Number, BOOL bComma = TRUE);
	LPCTSTR WriteNumber(double Number, BOOL bComma = TRUE, int Precision = -1);
	LPCTSTR WriteNumber(CString &Number, BOOL bComma = TRUE, int Precision = -1);
	LPCTSTR WriteNumber(LPCTSTR Number, BOOL bComma = TRUE, int Precision = -1);
	LPCTSTR WriteNumber(CEdit &Ctrl, BOOL bComma = TRUE, int Precision = -1);
	LPCTSTR TrimZero(CString &Num);
	LPCTSTR AddZero(CString &Num, int Precision);

	LPCTSTR RemoveString(CString &String, LPCTSTR RemoveString);

	int CopyDBRecArrayToComboBox(CDBRecArray &DBRecArray, COptComboBox &ComboBox, int Col = 0, BOOL bSetModified = TRUE);
	int CopyKeyDBListKeyToComboBox(CKeyDBList &KeyDBList, COptComboBox &ComboBox, BOOL bSetModified = TRUE);
	int CopyKeyDBListDataToComboBox(CKeyDBList &KeyDBList, LPCTSTR Key, COptComboBox &ComboBox, BOOL bSetModified = TRUE);

public: // FileName
	BOOL GetFileName(CWinApp *theApp, CString &Path, LPCTSTR Section, LPCTSTR Entry, LPCTSTR DefExt);
	BOOL IsBloombergAvailbe();

protected:
	void ReWriteFraction(CString &Num, BOOL bComma, int Precision);
};
#endif // __QData_H__