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
	iFileMan = CFileMan::NewL(aFs, this);
	
	CActiveScheduler::Add(this); // Add to scheduler
	}

CAsyncFileMan::~CAsyncFileMan()
	{
	Cancel(); // Cancel any request, if outstanding
	//iTimer.Close(); // Destroy the RTimer object
	// Delete instance variables if any
	
	delete iFileMan;
	}

void CAsyncFileMan::DoCancel()
	{
	//iTimer.Cancel();
	DEBUG(_L("Operation goes to cancell"));
	iCancelOperation = ETrue;
	}

//void CAsyncFileMan::StartL()
//	{
//	Cancel(); // Cancel any request, just to be sure
//	//iState = EUninitialized;
//	//iTimer.After(iStatus, aDelay); // Set for later
//	SetActive(); // Tell scheduler a request is active
//	}

void CAsyncFileMan::RunL()
	{
	DEBUG(_L("RunL status=%d"), iStatus.Int());
	
	iObserver->OnFileManFinished(iStatus.Int());
	
//	if (iState == EUninitialized)
//		{
//		// Do something the first time RunL() is called
//		iState = EInitialized;
//		}
//	else if (iState != EError)
//		{
//		// Do something
//		}
//	iTimer.After(iStatus, 1000000); // Set for 1 sec later
//	SetActive(); // Tell scheduler a request is active
	}

TInt CAsyncFileMan::RunError(TInt aError)
	{
	if (aError != KErrNone)
		ERROR(_L("Error, code=%d"), aError);
	
	return aError;
	}

MFileManObserver::TControl CAsyncFileMan::NotifyFileManStarted()
	{
	if (iCancelOperation)
		{
		DEBUG(_L("Operation cancelled"));
		//iObserver->OnFileManFinished(KErrCancel);
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
		//iObserver->OnFileManFinished(KErrCancel);
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
		//iObserver->OnFileManFinished(KErrCancel);
		return MFileManObserver::EAbort;
		}
	
	DEBUG(_L("NotifyFileManEnded"));
	return iObserver->OnFileManEnded();
	}



/*MFileManObserver::TControl CAsyncFileMan::CheckCancell()
	{
	if (iCancelOperation)
		return MFileManObserver::EAbort;
	else
		return MFileManObserver::EContinue;
	}*/

TInt CAsyncFileMan::Delete(const TDesC& aName, TUint aSwitch)
	{
	Cancel();
	TInt r = iFileMan->Delete(aName, aSwitch, iStatus);
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
