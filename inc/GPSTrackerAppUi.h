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

// Constants

_LIT(KTimeFormat, "%H:%T:%S");

// [[[ begin generated region: do not modify [Generated Forward Declarations]
class CTrackerInfoListBoxView;
// ]]] end generated region [Generated Forward Declarations]

/**
 * @class	CGPSTrackerAppUi GPSTrackerAppUi.h
 * @brief The AppUi class handles application-wide aspects of the user interface, including
 *        view management and the default menu, control pane, and status pane.
 */
class CGPSTrackerAppUi : public CAknViewAppUi, public MPositionListener
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
	
	/**
	 * @return Symbian OS error code or KErrNone
	 */
	TInt MakeDir(const TDesC &aDir);
	
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
	
	void ShowError(const TDesC aMsg, TInt anErrCode = KErrNone);
	
public:	
	void StartTracking();
	void StopTracking();
	inline TBool IsTrackingRunning()
		{ return iPosRequestor->IsRunning(); }
	
	// Events
	void OnPositionUpdated();
	void OnPositionPartialUpdated();
	void OnPositionRestored();
	void OnPositionLost();
	void OnPositionError(TInt aErrCode);
	void OnPauseTracking();
	void OnResumeTracking();

	
	};

#endif // GPSTRACKERAPPUI_H			
