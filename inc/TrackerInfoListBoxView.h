/*
========================================================================
 Name        : TrackerInfoListBoxView.h
 Author      : artem78
 Copyright   : General Public License v3.0
 Description : 
========================================================================
*/
#ifndef TRACKERINFOLISTBOXVIEW_H
#define TRACKERINFOLISTBOXVIEW_H

// [[[ begin generated region: do not modify [Generated Includes]
#include <aknview.h>
// ]]] end generated region [Generated Includes]


// [[[ begin [Event Handler Includes]
// ]]] end [Event Handler Includes]

// [[[ begin generated region: do not modify [Generated Constants]
// ]]] end generated region [Generated Constants]

// [[[ begin generated region: do not modify [Generated Forward Declarations]
class CTrackerInfoListBox;
// ]]] end generated region [Generated Forward Declarations]

/**
 * Avkon view class for TrackerInfoListBoxView. It is register with the view server
 * by the AppUi. It owns the container control.
 * @class	CTrackerInfoListBoxView TrackerInfoListBoxView.h
 */						
			
class CTrackerInfoListBoxView : public CAknView
	{
	
	
	// [[[ begin [Public Section]
public:
	// constructors and destructor
	CTrackerInfoListBoxView();
	static CTrackerInfoListBoxView* NewL();
	static CTrackerInfoListBoxView* NewLC();        
	void ConstructL();
	virtual ~CTrackerInfoListBoxView();
						
	// from base class CAknView
	TUid Id() const;
	void HandleCommandL( TInt aCommand );
	
	// [[[ begin generated region: do not modify [Generated Methods]
	CTrackerInfoListBox* CreateContainerL();
	// ]]] end generated region [Generated Methods]
	
	void SetDataL(TReal aLat, TReal aLon, TReal aAlt, TReal aSpeed, TInt aSatUsed,
			TInt aSatTotal, TInt aSavedPoints, TTimeIntervalMicroSeconds aPosRefreshRate);
	
	// ]]] end [Public Section]
	
	
	// [[[ begin [Protected Section]
protected:
	// from base class CAknView
	void DoActivateL(
		const TVwsViewId& aPrevViewId,
		TUid aCustomMessageId,
		const TDesC8& aCustomMessage );
	void DoDeactivate();
	void HandleStatusPaneSizeChange();
	
	// [[[ begin generated region: do not modify [Overridden Methods]
	// ]]] end generated region [Overridden Methods]
	
	
	// [[[ begin [User Handlers]
	TBool HandlePauseTrackRecordingMenuItemSelectedL( TInt aCommand );
	TBool HandleContinueTrackRecordingMenuItemSelectedL( TInt aCommand );
	TBool HandleTracksListMenuItemSelectedL( TInt aCommand );
	TBool HandleAboutMenuItemSelectedL( TInt aCommand );
	// ]]] end [User Handlers]
	
	// ]]] end [Protected Section]
	
	
	// [[[ begin [Private Section]
private:
	void SetupStatusPaneL();
	void CleanupStatusPane();
	
	// [[[ begin generated region: do not modify [Generated Instance Variables]
	CTrackerInfoListBox* iTrackerInfoListBox;
	// ]]] end generated region [Generated Instance Variables]
	
	// [[[ begin generated region: do not modify [Generated Methods]
	// ]]] end generated region [Generated Methods]
	
	// From MEikMenuObserver
	void DynInitMenuPaneL(TInt aMenuID, CEikMenuPane* aMenuPane);
	
	enum TListBoxItem
		{
		ELatitudeItem = 0,
		ELongitudeItem,
		EAltitudeItem,
		ESpeedItem,
		ESatellitesItem,
		ESavedPointsItem,
		EPositionRefreshRateItem
		};

	// ]]] end [Private Section]
	
	};

#endif // TRACKERINFOLISTBOXVIEW_H
