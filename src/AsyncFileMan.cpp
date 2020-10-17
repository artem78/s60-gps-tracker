/*
 ============================================================================
 Name		: AsyncFileMan.cpp
 Author	  : artem78
 Version	 : 1.0
 Copyright   : 
 Description : CAsyncFileMan implementation
 ============================================================================
 */

#include "AsyncFileMan.h"
#include "Logger.h"


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
