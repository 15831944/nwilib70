#ifndef __ASSETVAL_H__
#define __ASSETVAL_H__

#ifndef __ASSETELEV_H__
#include "assetlev.h"
#endif

#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif

class CAssetVal : public CAssetLev
{
DECLARE_DYNAMIC(CAssetVal)

public:
	CAssetVal() {}

	void Setup(COraLoader &OraLoader, LPCTSTR Date, LPCTSTR Portfolio, LPCTSTR TransNum);
	void Setup(COraLoader &OraLoader, LPCTSTR TransType, LPCTSTR Dir, LPCTSTR Asset, 
			LPCTSTR ValueDate, LPCTSTR Date, double NomAmount, double Price, double Fxrate, 
			LPCTSTR LevRateBasis = "A/360", double LevRate = 0, LPCTSTR Formula = NULL);
	void Setup(COraLoader &OraLoader, LPCTSTR TransType, LPCTSTR Dir, LPCTSTR Asset, 
			LPCTSTR ValueDate, LPCTSTR Date, LPCTSTR NomAmount, LPCTSTR Price, LPCTSTR Fxrate, 
			LPCTSTR LevRateBasis = "A/360", LPCTSTR LevRate = "0", LPCTSTR Formula = NULL);
};
#endif
