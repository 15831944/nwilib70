#ifndef __ORFEEDATA_H__
#define __ORFEEDATA_H__

#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif


class COrFeeData : public CObject
{
DECLARE_DYNAMIC(COrFeeData)
protected:
	CString m_PB;
	CString m_CP;
	double m_LowLimit;
	double m_HighLimit;
	double m_OrFee;
	double m_OCCFee;

public:
	COrFeeData() { }
	COrFeeData(COrFeeData& OrFeeData) { Copy(OrFeeData); }
	~COrFeeData() {}

	void Copy(COrFeeData &OrFeeData);

	const COrFeeData &operator = (COrFeeData &OrFeeData) { Copy(OrFeeData); return *this; }
	CString &GetPB() { return m_PB; }
	CString &GetCP() { return m_CP; }
	double &GetLowLimit() { return m_LowLimit; }
	double &GetHighLimit() { return m_HighLimit; }
	double &GetOrFee() { return m_OrFee; }
	double &GetOCCFee() { return m_OCCFee; }
};

class COrFeeDataArray : public CObArray
{
DECLARE_DYNAMIC(COrFeeDataArray)
public:
	COrFeeDataArray() { };
	COrFeeDataArray(COrFeeDataArray &RecArray) { Copy(RecArray); }
	~COrFeeDataArray() { RemoveAll(); }

	int	Add(COrFeeData *Rec);
	void SetAt(int Index, COrFeeData *Rec);
	void InsertAt(int Index, COrFeeData *Rec);
	int Add(COrFeeData &Rec);
	void SetAt(int Index, COrFeeData &Rec);
	void InsertAt(int Index, COrFeeData &Rec);
	void RemoveAt(int Index);
	void RemoveAll();
	void Copy(COrFeeDataArray &OrFeeDataArray);

	COrFeeData *GetAt(int Index);
	int Find(const CString PB, const CString CP, double Contracts);
	int LoadData(COraLoader &OraLoader);
	const COrFeeDataArray &operator = (COrFeeDataArray &OrFeeDataArray) { Copy(OrFeeDataArray); return *this; }
};
#endif