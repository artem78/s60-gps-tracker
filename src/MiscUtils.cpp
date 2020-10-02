/*
 ============================================================================
 Name		: MiscUtils.cpp
 Author	  : artem78
 Version	 : 1.0
 Copyright   : 
 Description : MiscUtils implementation
 ============================================================================
 */

#include "MiscUtils.h"

void MiscUtils::FileSizeToReadableString(/*TUint64*/ TInt aBytes, TDes &aDes)
	{
	_LIT(KBytesUnit, "B");
	_LIT(KKiloBytesUnit, "KB");
	_LIT(KMegaBytesUnit, "MB");
	_LIT(KGigaBytesUnit, "GB");
	const TInt KGiga = 1024 * KMega;
	
	TReal size;
	TPtrC unit;
	TRealFormat realFmt;
	realFmt.iType = KRealFormatFixed | KDoNotUseTriads;
	//realFmt.iPoint = '.';
	realFmt.iPlaces = /*1*/ 2;
	realFmt.iTriLen = 0;
	realFmt.iWidth = KDefaultRealWidth;
	
	if (aBytes < KKilo)
		{ // Bytes
		size = aBytes;
		unit.Set(KBytesUnit);
		realFmt.iPlaces = 0;
		}
	else if (aBytes < KMega)
		{ // Kilobytes
		size = (TReal)aBytes / KKilo;
		unit.Set(KKiloBytesUnit);
		}
	else if (aBytes < KGiga)
		{ // Megabytes
		size = (TReal)aBytes / KMega;
		unit.Set(KMegaBytesUnit);
		}
	else
		{ // Gigabytes
		size = (TReal)aBytes / KGiga;
		unit.Set(KGigaBytesUnit);
		}
	
	aDes.Zero();
	aDes.Num(size, realFmt);
	aDes.Append(' ');
	aDes.Append(unit);	
	}
