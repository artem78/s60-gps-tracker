/*
 ============================================================================
 Name		: PhoneInfo.cpp
 Author	  : artem78
 Version	 : 1.0
 Copyright   : 
 Description : PhoneInfo implementation
 ============================================================================
 */

#include "PhoneInfo.h"

#include <e32math.h>

// TVersion2

TVersion2::TVersion2() : iMajor(0), iMinor(0) {}

TVersion2::TVersion2(TInt aMajor, TInt aMinor) : iMajor(aMajor), iMinor(aMinor) {}

bool operator== (const TVersion2 &aV1, const TVersion2 &aV2)
{
	return aV1.iMajor == aV2.iMajor && aV1.iMinor == aV2.iMinor;
}

bool operator!= (const TVersion2 &aV1, const TVersion2 &aV2)
{
	return !(aV1 == aV2);
}

bool operator> (const TVersion2 &aV1, const TVersion2 &aV2)
{
	if (aV1.iMajor > aV2.iMajor)
		return ETrue;
	else if (aV1.iMajor < aV2.iMajor)
		return EFalse;
	else // aV1.iMajor == aV2.iMajor
		return aV1.iMinor > aV2.iMinor;
}
 
bool operator< (const TVersion2 &aV1, const TVersion2 &aV2)
{
    return aV1 != aV2 && !(aV1 > aV2);
}


// CPhoneInfo

CPhoneInfo::CPhoneInfo(RFs &aFs) : iFs(aFs) {}

TBool CPhoneInfo::GetModel(TDes &aModel)
	{
	TBuf8<64> phoneModel8; // 16bit chars
	_LIT(KModelFilePath, "z:\\resource\\versions\\model.txt");
	iFs.ReadFileSection(KModelFilePath, 0, phoneModel8, phoneModel8.MaxLength());
	if (phoneModel8.Length() > 0)
	    {
	    TPtrC16 phoneModelPtr((const TUint16*)phoneModel8.Ptr(),phoneModel8.Size()/2); // Convert 8bit ptr to 16bit
	    aModel.Copy(phoneModelPtr.Mid(1, phoneModelPtr.Length()-1)); // Drop 2 bytes of BOM
	    return ETrue;
	    }
	else
	    return EFalse;
	}

/*TBool CPhoneInfo::GetOS(TDes &anOS)
	{
	anOS.Copy(_L("Symbian 9.2"));
	return ETrue;
	}*/

TBool CPhoneInfo::GetPlatformVersion(TVersion2 &aVer)
	{
	// Theare are will be more than one files with S60 version
	// (ex.: series60v3.0.sis and  series60v3.1.sis), therefore
	// gets latest version.
	
	//_LIT(KSisFilesPath, "z:\\system\\install\\series60v*.*.sis");
	_LIT(KSisFilesPath, "z:\\system\\install\\series60v*.sis");
	
	CDir* files(NULL);
	if (iFs.GetDir(KSisFilesPath, KEntryAttNormal, ESortNone, files) != KErrNone)
		return EFalse;

	if (files != NULL)
		{
		CleanupStack::PushL(files);
		
		TVersion2 maxVer(0, 0);
		
		for (TInt i = 0; i < files->Count(); i++)
			{
			if ((*files)[i].IsDir())
				continue;
			
			TVersion2 ver;
			if (ParseS60Version((*files)[i].iName, ver))
				{
				if (ver > maxVer)
					maxVer = ver;
				}
			}
		
		CleanupStack::PopAndDestroy(files);
		
		if (maxVer != TVersion2(0, 0))
			{
			aVer = maxVer;
			return ETrue;
			}
		
		return EFalse;
		}
	
	return KErrUnknown;
	}

TBool CPhoneInfo::GetPlatform(TDes &aPlatform)
	{
	TVersion2 ver;
	if (GetPlatformVersion(ver))
		{
		_LIT(KS60VerFmt, "s60v%d.%d");
		aPlatform.Format(KS60VerFmt, ver.iMajor, ver.iMinor);
		return ETrue;
		}
	else
		return EFalse;
	}

TBool CPhoneInfo::ParseS60Version(const TDesC &aDes, TVersion2 &aVer)
	{
	_LIT(KStrStart, "series60v");
	_LIT(KStrEnd, ".sis");
	
	// Some checks
	if (aDes.Find(KStrStart) != 0)
		return EFalse;
	if (aDes.Find(KStrEnd) != aDes.Length() - KStrEnd().Length())
		return EFalse;
	
	TInt posStart = KStrStart().Length();
	TInt posEnd = aDes.Length() - KStrEnd().Length() - 1;
	TInt len = posEnd - posStart + 1;
	
	TPtrC numPtr = aDes.Mid(posStart, len);
	TLex lex(numPtr);
	
	// Parse major
	TInt major = 0;
	while ((lex.Peek()).IsDigit())
		major = major * 10 + (TInt)lex.Get() - (TInt)'0';
	
	if (lex.Peek() == '.')
		lex.Inc();
	else
		return EFalse;
	
	// Parse minor
	TInt minor = 0;
	while ((lex.Peek()).IsDigit())
		minor = minor * 10 + (TInt)lex.Get() - (TInt)'0';
	
	if (major == 0 && minor == 0)
		return EFalse;
	
	// Make result
	aVer.iMajor = major;
	aVer.iMinor = minor;
	return ETrue;
	}
