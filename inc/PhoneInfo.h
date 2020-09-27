/*
 ============================================================================
 Name		: PhoneInfo.h
 Author	  : artem78
 Version	 : 1.0
 Copyright   : 
 Description : Phone and system version information
 ============================================================================
 */

#ifndef PHONEINFO_H
#define PHONEINFO_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <f32file.h>

// CLASS DECLARATION

/* Version class with only Major and Minor values (without Build) */
class TVersion2
	{
public:
	TInt iMajor;
	TInt iMinor;
	
	TVersion2();
	TVersion2(TInt aMajor, TInt aMinor);
	
	friend bool operator== (const TVersion2 &aV1, const TVersion2 &aV2);
	friend bool operator!= (const TVersion2 &aV1, const TVersion2 &aV2);
    friend bool operator> (const TVersion2 &aV1, const TVersion2 &aV2);
    friend bool operator< (const TVersion2 &aV1, const TVersion2 &aV2);
	};

/**
 *  PhoneInfo
 * 
 */
class CPhoneInfo : public CBase
	{
private:
	RFs iFs;
	
	TBool ParseS60Version(const TDesC &aDes, TVersion2 &aVer);
	
public:
	CPhoneInfo(RFs &aFs);
	TBool GetModel(TDes &aModel);
	//TBool GetOS(TDes &anOS);
	TBool GetPlatformVersion(TVersion2 &aVer);
	TBool GetPlatform(TDes &aPlatform);
	};

#endif // PHONEINFO_H
