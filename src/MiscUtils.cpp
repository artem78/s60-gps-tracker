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
#include "GPSTracker.RSG"
#include <stringloader.h>
//#include <eikenv.h>
//#include <eikdef.h>

void MiscUtils::DistanceToDesL(TReal aMeters, TDes &aDes)
	{
	const /*TInt*/ TReal KMetersInKilometer = 1000.0;
	
	if (aMeters < KMetersInKilometer) // For <1km show distance in tens of meters
		{
		HBufC* metersUnit = StringLoader::LoadL(R_TRACKER_INFO_LIST_BOX_METERS_UNIT_TEXT/*, iEikonEnv*/);
		
		aDes.Num((TInt) /*(*/ aMeters /*+ 5)*/ / 10 * 10);
		aDes.Append(' ');
		aDes.Append(*metersUnit);
		
		delete metersUnit;
		}
	else // Show distance in kilometers
		{
		HBufC* kilometersUnit = StringLoader::LoadL(R_TRACKER_INFO_LIST_BOX_KILOMETERS_UNIT_TEXT/*,*/ /*iEikonEnv*/ /*CCoeEnv::static()*/);
		const TRealFormat KShortRealFmt = TRealFormat(10, 1);
		
		aDes.Num(aMeters / KMetersInKilometer, KShortRealFmt);
		aDes.Append(' ');
		aDes.Append(*kilometersUnit);
		
		delete kilometersUnit;
		}
	}
