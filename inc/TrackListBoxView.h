/*
========================================================================
 Name        : TrackListBoxView.h
 Author      : artem78
 Copyright   : 
 Description : 
========================================================================
*/
#ifndef TRACKLISTBOXVIEW_H
#define TRACKLISTBOXVIEW_H

// [[[ begin generated region: do not modify [Generated Includes]
#include <aknview.h>
// ]]] end generated region [Generated Includes]


// [[[ begin [Event Handler Includes]
// ]]] end [Event Handler Includes]

#include <eiklbx.h>


// [[[ begin generated region: do not modify [Generated Constants]
// ]]] end generated region [Generated Constants]

// [[[ begin generated region: do not modify [Generated Forward Declarations]
class CAknNavigationDecorator;
class CTrackListBox;
// ]]] end generated region [Generated Forward Declarations]

/**
 * Avkon view class for TrackListBoxView. It is register with the view server
 * by the AppUi. It owns the container control.
 * @class	CTrackListBoxView TrackListBoxView.h
 */						
			
class CTrackListBoxView : public CAknView, public MListBoxItemChangeObserver
	{
	
	
	// [[[ begin [Public Section]
public:
	// constructors and destructor
	CTrackListBoxView();
	static CTrackListBoxView* NewL();
	static CTrackListBoxView* NewLC();        
	void ConstructL();
	virtual ~CTrackListBoxView();
						
	// from base class CAknView
	TUid Id() const;
	void HandleCommandL( TInt aCommand );
	
	// [[[ begin generated region: do not modify [Generated Methods]
	CTrackListBox* CreateContainerL();
	// ]]] end generated region [Generated Methods]
	
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
	// ]]] end [User Handlers]
	
	// ]]] end [Protected Section]
	
	
	// [[[ begin [Private Section]
private:
	void SetupStatusPaneL();
	void CleanupStatusPane();
	
	// [[[ begin generated region: do not modify [Generated Instance Variables]
	// any current navi decorator
	CAknNavigationDecorator* iNaviDecorator_;
	CTrackListBox* iTrackListBox;
	// ]]] end generated region [Generated Instance Variables]
	
	// [[[ begin generated region: do not modify [Generated Methods]
	// ]]] end generated region [Generated Methods]
	
	// ]]] end [Private Section]
	
	
	// From MListBoxItemChangeObserver
public:
	void ListBoxItemsChanged(CEikListBox* aListBox);
	
	
private:
	TVwsViewId iPrevViewId;
	
public:
	void SetNaviPaneTextL(const TDesC& aNaviText);
	void SetTrackArrayL(const CDesCArray &aTrackArr);
	
	};

#endif // TRACKLISTBOXVIEW_H
