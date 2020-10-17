/*
========================================================================
 Name        : GPSTrackerAppUi.h
 Author      : artem78
 Copyright   : General Public License v3.0
 Description : 
========================================================================
*/
#ifndef GPSTRACKERAPPUI_H
#define GPSTRACKERAPPUI_H

// [[[ begin generated region: do not modify [Generated Includes]
#include <aknviewappui.h>
// ]]] end generated region [Generated Includes]

#include <LoggingDefs.h>
#include "Logger.h"
#include "Positioning.h"
#include "TrackWriter.h"
#include "AsyncFileMan.h"

// Constants

_LIT(KTimeFormat, "%H:%T:%S");

// [[[ begin generated region: do not modify [Generated Forward Declarations]
class CTrackerInfoListBoxView;
class CTrackListBoxView;
// ]]] end generated region [Generated Forward Declarations]

/**
 * @class	CGPSTrackerAppUi GPSTrackerAppUi.h
 * @brief The AppUi class handles application-wide aspects of the user interface, including
 *        view management and the default menu, control pane, and status pane.
 */
class CGPSTrackerAppUi : public CAknViewAppUi, public MPositionListener,
		public MAsyncFileManObserver
	{
public: 
	// constructor and destructor
	CGPSTrackerAppUi();
	virtual ~CGPSTrackerAppUi();
	void ConstructL();

public:
	// from CCoeAppUi
	TKeyResponse HandleKeyEventL(
				const TKeyEvent& aKeyEvent,
				TEventCode aType );

	// from CEikAppUi
	void HandleCommandL( TInt aCommand );
	void HandleResourceChangeL( TInt aType );

	// from CAknAppUi
	void HandleViewDeactivation( 
			const TVwsViewId& aViewIdToBeDeactivated, 
			const TVwsViewId& aNewlyActivatedViewId );

private:
	void InitializeContainersL();
	// [[[ begin generated region: do not modify [Generated Methods]
public: 
	// ]]] end generated region [Generated Methods]
	
	// [[[ begin generated region: do not modify [Generated Instance Variables]
private: 
	CTrackerInfoListBoxView* iTrackerInfoListBoxView;
	CTrackListBoxView* iTrackListBoxView;
	// ]]] end generated region [Generated Instance Variables]
	
	
	// [[[ begin [User Handlers]
protected: 
	// ]]] end [User Handlers]

	
private:
	CDynamicPositionRequestor* iPosRequestor;
	CGPXTrackWriter* iTrackWriter;
	TUint iTotalPointsCount;
	TReal iTotalDistance;
	TBool iIsAfterConnectionRestored;	
	TBool iIsGoingToExit;
	CAsyncFileMan* iAsyncFileMan;
//	CFileMan* iFileMan;
//	TBool iCancelFManOperation; // Used for cancel current operation in file manager
//	TInt iFManProcessedFiles;
//	TInt iFManTotalFiles;
//	TRequestStatus iFManOperationReqStatus;
	
	/**
	 * @return Symbian OS error code or KErrNone
	 */
	TInt CurrentDateTime(TDes &aDes, const TDesC &aFormat);
	
#if LOGGING_ENABLED
	RFile iLogFile;
	CLogger* iLogger;
	void InitializeLoggingL();
#endif
	RFile iTrackFile;
	void InitializeTrackL();
	void ShowDataL();
	void /*Get*/ProgramDataDir(TDes &aDir);
	
public:
	void ShowError(const TDesC &aMsg, TInt anErrCode = KErrNone);
	void ShowMsgL(const TDesC &aTitle, const TDesC &aMsg);
	
public:	
	void StartTracking();
	void StopTracking();
	inline TBool IsTrackingRunning()
		{ return iPosRequestor->IsRunning(); }
	void GetTracksArrayL(CDesCArray &aTrackArr);
	void UpdateTrackListL();
	void ShowTrackListL();
	void TrackDir(TDes &aDes);
	void LogDir(TDes &aDes);
	void DeleteAllTracks/*L*/();
	inline void CancelCurrentFManOperation()
		{ iAsyncFileMan->Cancel(); }
	const TDesC& ProgramName() const;
	const TDesC& ProgramVersion() const;
	
	// Events
	void OnPositionUpdated();
	void OnPositionPartialUpdated();
	void OnPositionRestored();
	void OnPositionLost();
	void OnPositionError(TInt aErrCode);
	void OnPauseTracking();
	void OnResumeTracking();

	
	// from MFileManObserver
//public:
//	MFileManObserver::TControl NotifyFileManStarted();
//	MFileManObserver::TControl NotifyFileManOperation();
//	MFileManObserver::TControl NotifyFileManEnded();
	
//	void NotifyFileManTotallyEnded();
	
	
	// From MAsyncFileManObserver
public:
//	MFileManObserver::TControl OnFileManStarted();
//	MFileManObserver::TControl OnFileManOperation();
	MFileManObserver::TControl OnFileManEnded();
	void OnFileManFinished(TInt aStatus);
	
	
	
	friend CTrackListBoxView;
	
	};

#endif // GPSTRACKERAPPUI_H			
