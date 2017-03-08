#ifndef __BLMDATA_H__
#define __BLMDATA_H__

#ifndef __NWIVAR_H__
#include "nwivar.h"
#endif

#ifndef __KEYDATA_H__
#include "KeyData.H"
#endif

class CBlmSec : public CKeyData
{
DECLARE_DYNAMIC(CBlmSec)

protected:
	CString m_AssetClass;
	CString m_Ticker;
	CString m_IDType;
	CString m_CountryCode;
	CString m_BlmKey;
	CString m_BlmCurr;
	double m_Price;
	double m_PrevPrice;
	double m_Fxrate;
	double m_WtFactor;
	int m_nFxIdx;
	BOOL m_bUSDPrice;
	BOOL m_bRev;
	BOOL m_bModified;

public:
	CBlmSec() {}
	CBlmSec(CBlmSec &BlmSec) { Copy(BlmSec); }
	~CBlmSec() {}

	CBlmSec operator=(CBlmSec &Other) { Copy(Other); return *this; }

	CString &GetAssetClass() { return m_AssetClass; }
	CString &GetTicker() { return m_Ticker; }
	CString &GetIDType() { return m_IDType; }
	CString &GetCountryCode() { return m_CountryCode; }
	CString &GetBlmKey() { return m_BlmKey; }
	CString &GetBlmCurr() { return m_BlmCurr; }
	double &GetPrevPrice() { return m_PrevPrice; }
	double &GetPrice() { return m_Price; }
	double &GetFxrate() { return m_Fxrate; }
	int &GetFxIdx() { return m_nFxIdx; }
	BOOL &IsUSDPrice() { return m_bUSDPrice; }
	BOOL &IsRev() { return m_bRev; }
	BOOL &IsModified() { return m_bModified; }
	
	void Copy(CBlmSec &BlmSec);	
};


class CBlmSecArray : public CKeyDataArray
{
DECLARE_DYNAMIC(CBlmSecArray)

protected:
	CStringArray m_Fields;

protected:	

public:
	CBlmSecArray() { }
	~CBlmSecArray() { RemoveAll(); }

	CStringArray &GetFields() { return m_Fields; }

	int	Add(CBlmSec *pSecData);
	int Add(CBlmSec &SecData) { return Add(new CBlmSec(SecData)); }
	void SetAt(int Index, CBlmSec *pSecData);
	void SetAt(int Index, CBlmSec &SecData) { SetAt(Index, new CBlmSec(SecData)); }
	void InsertAt(int Index, CBlmSec *pSecData);
	void InsertAt(int Index, CBlmSec &SecData) { InsertAt(Index, new CBlmSec(SecData)); }
	void RemoveAt(int Index);
	void RemoveAll();
	CBlmSec *GetAt(int Index);
	void Copy(CBlmSecArray &BlmSecArray);
	int Find(const CString Key);

	BOOL IsModified();
	BOOL IsFullyUpdated();

	const CBlmSecArray &operator=(CBlmSecArray &BlmSecArray) { Copy(BlmSecArray); return *this; }

	void ResetData();

protected:
	BOOL FreePointer(int Index);
};
#endif
