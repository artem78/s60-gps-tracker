/*
 ============================================================================
 Name		: AsyncFileMan.h
 Author	  : artem78
 Version	 : 1.0
 Copyright   : 
 Description : CAsyncFileMan declaration
 ============================================================================
 */

#ifndef ASYNCFILEMAN_H
#define ASYNCFILEMAN_H

// Includes

#include <e32base.h>	// For CActive, link against: euser.lib
//#include <e32std.h>
#include <f32file.h>

// Forward declarations

class MAsyncFileManObserver;

// Classes

class CAsyncFileMan : public CActive, public MFileManObserver
	{
public:
	// Cancel and destroy
	~CAsyncFileMan();

	// Two-phased constructor.
	static CAsyncFileMan* NewL(RFs &aFs, MAsyncFileManObserver* aObserver);

	// Two-phased constructor.
	static CAsyncFileMan* NewLC(RFs &aFs, MAsyncFileManObserver* aObserver);

public:
	// New functions
	// Function for making the initial request
//	void StartL();

private:
	// C++ constructor
	CAsyncFileMan(MAsyncFileManObserver* aObserver);

	// Second-phase constructor
	void ConstructL(RFs &aFs);

private:
	// From CActive
	// Handle completion
	void RunL();

	// How to cancel me
	void DoCancel();

	// Override to handle leaves from RunL(). Default implementation causes
	// the active scheduler to panic.
	TInt RunError(TInt aError);
	
// from MFileManObserver
public:
	MFileManObserver::TControl NotifyFileManStarted();
	MFileManObserver::TControl NotifyFileManOperation();
	MFileManObserver::TControl NotifyFileManEnded();

private:
	/*enum TAsyncFileManState
		{
		EUninitialized, // Uninitialized
		EInitialized, // Initalized
		EError
		// Error condition
		};*/

private:
	//TInt iState; // State of the active object
	CFileMan* iFileMan;
	MAsyncFileManObserver* iObserver;
	TBool iCancelOperation; // Used for cancel current operation in file manager
	//TInt iFManProcessedFiles;
	//TInt iFManTotalFiles;
//	TRequestStatus iFManOperationReqStatus;

	//MFileManObserver::TControl CheckCancell();
	
public:
	// ToDo: Add other operations (rename, copy, etc...)
	TInt Delete(const TDesC& aName, TUint aSwitch=0);
	
	};


class MAsyncFileManObserver
	{
public:
	virtual MFileManObserver::TControl OnFileManStarted();
	virtual MFileManObserver::TControl OnFileManOperation();
	virtual MFileManObserver::TControl OnFileManEnded();
	virtual void OnFileManFinished(TInt aStatus);
	};

#endif // ASYNCFILEMAN_H
