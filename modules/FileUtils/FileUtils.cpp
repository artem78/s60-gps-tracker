/*
 ============================================================================
 Name		: FileUtils.cpp
 Author	  : artem78
 Version	 : 1.0
 Copyright   : 
 Description :
 ============================================================================
 */

#include "FileUtils.h"
#include "Logger.h"


// FileUtils

void FileUtils::FileSizeToReadableString(/*TUint64*/ TInt aBytes, TDes &aDes)
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


// 	CFileManExtended

CFileManExtended* CFileManExtended::NewL(RFs& aFs)
	{
	// Just change class of returned pointer from parent
	return static_cast<CFileManExtended*>(CFileMan::NewL(aFs));
	}

CFileManExtended* CFileManExtended::NewL(RFs& aFs,MFileManObserver* anObserver)
	{
	// Just change class of returned pointer from parent
	return static_cast<CFileManExtended*>(CFileMan::NewL(aFs, anObserver));
	}


// CAsyncFileMan

CAsyncFileMan::CAsyncFileMan(MAsyncFileManObserver* aObserver) :
	CActive(EPriorityStandard), // Standard priority
	iObserver(aObserver)
	{
	}

CAsyncFileMan* CAsyncFileMan::NewLC(RFs &aFs, MAsyncFileManObserver* aObserver)
	{
	CAsyncFileMan* self = new (ELeave) CAsyncFileMan(aObserver);
	CleanupStack::PushL(self);
	self->ConstructL(aFs);
	return self;
	}

CAsyncFileMan* CAsyncFileMan::NewL(RFs &aFs, MAsyncFileManObserver* aObserver)
	{
	CAsyncFileMan* self = CAsyncFileMan::NewLC(aFs, aObserver);
	CleanupStack::Pop(); // self;
	return self;
	}

void CAsyncFileMan::ConstructL(RFs &aFs)
	{	
	iFileMan = CFileManExtended::NewL(aFs, this);
	
	CActiveScheduler::Add(this); // Add to scheduler
	}

CAsyncFileMan::~CAsyncFileMan()
	{
	Cancel(); // Cancel any request, if outstanding
	
	delete iFileMan;
	}

void CAsyncFileMan::DoCancel()
	{
	DEBUG(_L("Operation goes to cancell"));
	iCancelOperation = ETrue;
	
	// When cancelling, RunL won`t be called later,
	// therefore call observer`s method here 
	iObserver->OnFileManFinished(KErrCancel);
	}

void CAsyncFileMan::RunL()
	{
	DEBUG(_L("RunL status=%d"), iStatus.Int());
	
	iObserver->OnFileManFinished(iStatus.Int());
	}

TInt CAsyncFileMan::RunError(TInt aError)
	{
	if (aError != KErrNone)
		ERROR(_L("Error, code=%d"), aError);
	
	return /*aError*/ KErrNone;
	}

MFileManObserver::TControl CAsyncFileMan::NotifyFileManStarted()
	{
	if (iCancelOperation)
		{
		DEBUG(_L("Operation cancelled"));
		return MFileManObserver::EAbort;
		}
	
	DEBUG(_L("NotifyFileManStarted"));
	return iObserver->OnFileManStarted();
	}

MFileManObserver::TControl CAsyncFileMan::NotifyFileManOperation()
	{
	if (iCancelOperation)
		{
		DEBUG(_L("Operation cancelled"));
		return MFileManObserver::EAbort;
		}
	
	DEBUG(_L("NotifyFileManOperation"));
	return iObserver->OnFileManOperation(); 
	}

MFileManObserver::TControl CAsyncFileMan::NotifyFileManEnded()
	{
	if (iCancelOperation)
		{
		DEBUG(_L("Operation cancelled"));
		return MFileManObserver::EAbort;
		}
	
	DEBUG(_L("NotifyFileManEnded"));
	return iObserver->OnFileManEnded();
	}

TInt CAsyncFileMan::Delete(const TDesC& aName, TUint aSwitch)
	{
	//Cancel();
	if (IsActive())
		return KErrInUse;
	iCancelOperation = EFalse;
	TInt r = iFileMan->Delete(aName, aSwitch, iStatus); // ToDo: Check r
	SetActive();
	INFO(_L("Delete operation started"));
	return r;
	}


// MAsyncFileManObserver

MFileManObserver::TControl MAsyncFileManObserver::OnFileManStarted()
	{
	return MFileManObserver::EContinue;
	}

MFileManObserver::TControl MAsyncFileManObserver::OnFileManOperation()
	{
	return MFileManObserver::EContinue;
	}

MFileManObserver::TControl MAsyncFileManObserver::OnFileManEnded()
	{
	return MFileManObserver::EContinue;
	}

void MAsyncFileManObserver::OnFileManFinished(TInt /*aStatus*/)
	{
	
	}
