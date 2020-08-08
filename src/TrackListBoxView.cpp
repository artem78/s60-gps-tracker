/*
========================================================================
 Name        : TrackListBoxView.cpp
 Author      : artem78
 Copyright   : 
 Description : 
========================================================================
*/
// [[[ begin generated region: do not modify [Generated System Includes]
#include <aknviewappui.h>
#include <eikmenub.h>
#include <avkon.hrh>
#include <barsread.h>
#include <stringloader.h>
#include <aknlists.h>
#include <eikenv.h>
#include <akniconarray.h>
#include <eikclbd.h>
#include <akncontext.h>
#include <akntitle.h>
#include <eikbtgpc.h>
#include <GPSTracker.rsg>
// ]]] end generated region [Generated System Includes]

// [[[ begin generated region: do not modify [Generated User Includes]

#include "GPSTracker.hrh"
#include "TrackListBoxView.h"
#include "TrackerInfoListBox.hrh"
#include "TrackListBox.h"
// ]]] end generated region [Generated User Includes]

// [[[ begin generated region: do not modify [Generated Constants]
// ]]] end generated region [Generated Constants]

#include "GPSTrackerAppUi.h"

/**
 * First phase of Symbian two-phase construction. Should not contain any
 * code that could leave.
 */
CTrackListBoxView::CTrackListBoxView()
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	iTrackListBox = NULL;
	// ]]] end generated region [Generated Contents]
	
	}

/** 
 * The view's destructor removes the container from the control
 * stack and destroys it.
 */
CTrackListBoxView::~CTrackListBoxView()
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	delete iTrackListBox;
	iTrackListBox = NULL;
	// ]]] end generated region [Generated Contents]
	
	}

/**
 * Symbian two-phase constructor.
 * This creates an instance then calls the second-phase constructor
 * without leaving the instance on the cleanup stack.
 * @return new instance of CTrackListBoxView
 */
CTrackListBoxView* CTrackListBoxView::NewL()
	{
	CTrackListBoxView* self = CTrackListBoxView::NewLC();
	CleanupStack::Pop( self );
	return self;
	}

/**
 * Symbian two-phase constructor.
 * This creates an instance, pushes it on the cleanup stack,
 * then calls the second-phase constructor.
 * @return new instance of CTrackListBoxView
 */
CTrackListBoxView* CTrackListBoxView::NewLC()
	{
	CTrackListBoxView* self = new ( ELeave ) CTrackListBoxView();
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}


/**
 * Second-phase constructor for view.  
 * Initialize contents from resource.
 */ 
void CTrackListBoxView::ConstructL()
	{
	// [[[ begin generated region: do not modify [Generated Code]
	BaseConstructL( R_TRACK_LIST_BOX_TRACK_LIST_BOX_VIEW );
				
	// ]]] end generated region [Generated Code]
	
	// add your own initialization code here
	
	}

/**
 * @return The UID for this view
 */
TUid CTrackListBoxView::Id() const
	{
	return TUid::Uid( ETrackListBoxViewId );
	}

/**
 * Handle a command for this view (override)
 * @param aCommand command id to be handled
 */
void CTrackListBoxView::HandleCommandL( TInt aCommand )
	{
	// [[[ begin generated region: do not modify [Generated Code]
	TBool commandHandled = EFalse;
	switch ( aCommand )
		{	// code to dispatch to the AknView's menu and CBA commands is generated here
		default:
			break;
		}
	
		
	if ( !commandHandled ) 
		{
	
		if ( aCommand == EAknSoftkeyBack )
			{
			// Go back to previous view
			AppUi()->ActivateViewL(iPrevViewId);
			}
	
		}
	// ]]] end generated region [Generated Code]
	
	}

/**
 *	Handles user actions during activation of the view, 
 *	such as initializing the content.
 */
void CTrackListBoxView::DoActivateL( 
		const TVwsViewId& aPrevViewId,
		TUid /*aCustomMessageId*/,
		const TDesC8& /*aCustomMessage*/ )
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	SetupStatusPaneL();
	
				
	if ( iTrackListBox == NULL )
		{
		iTrackListBox = CreateContainerL();
		iTrackListBox->SetMopParent( this );
		AppUi()->AddToStackL( *this, iTrackListBox );
		} 
	// ]]] end generated region [Generated Contents]
	
	iPrevViewId = aPrevViewId;
	
	// Tell AppUi to refresh track list
	static_cast<CGPSTrackerAppUi *>(AppUi())->UpdateTrackListL();
	}

/**
 */
void CTrackListBoxView::DoDeactivate()
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	CleanupStatusPane();
	
	if ( iTrackListBox != NULL )
		{
		AppUi()->RemoveFromViewStack( *this, iTrackListBox );
		delete iTrackListBox;
		iTrackListBox = NULL;
		}
	// ]]] end generated region [Generated Contents]
	}

/** 
 * Handle status pane size change for this view (override)
 */
void CTrackListBoxView::HandleStatusPaneSizeChange()
	{
	CAknView::HandleStatusPaneSizeChange();
	
	// this may fail, but we're not able to propagate exceptions here
	TVwsViewId view;
	AppUi()->GetActiveViewId( view );
	if ( view.iViewUid == Id() )
		{
		TInt result;
		TRAP( result, SetupStatusPaneL() );
		}
	
	// [[[ begin generated region: do not modify [Generated Code]
	// ]]] end generated region [Generated Code]
	
	}

// [[[ begin generated function: do not modify
void CTrackListBoxView::SetupStatusPaneL()
	{
	// reset the context pane
	TUid contextPaneUid = TUid::Uid( EEikStatusPaneUidContext );
	CEikStatusPaneBase::TPaneCapabilities subPaneContext = 
		StatusPane()->PaneCapabilities( contextPaneUid );
	if ( subPaneContext.IsPresent() && subPaneContext.IsAppOwned() )
		{
		CAknContextPane* context = static_cast< CAknContextPane* > ( 
			StatusPane()->ControlL( contextPaneUid ) );
		context->SetPictureToDefaultL();
		}
	
	// setup the title pane
	TUid titlePaneUid = TUid::Uid( EEikStatusPaneUidTitle );
	CEikStatusPaneBase::TPaneCapabilities subPaneTitle = 
		StatusPane()->PaneCapabilities( titlePaneUid );
	if ( subPaneTitle.IsPresent() && subPaneTitle.IsAppOwned() )
		{
		CAknTitlePane* title = static_cast< CAknTitlePane* >( 
			StatusPane()->ControlL( titlePaneUid ) );
		TResourceReader reader;
		iEikonEnv->CreateResourceReaderLC( reader, R_TRACK_LIST_BOX_TITLE_RESOURCE );
		title->SetFromResourceL( reader );
		CleanupStack::PopAndDestroy(); // reader internal state
		}
				
	}

// ]]] end generated function

// [[[ begin generated function: do not modify
void CTrackListBoxView::CleanupStatusPane()
	{
	}

// ]]] end generated function

/**
 *	Creates the top-level container for the view.  You may modify this method's
 *	contents and the CTrackListBox::NewL() signature as needed to initialize the
 *	container, but the signature for this method is fixed.
 *	@return new initialized instance of CTrackListBox
 */
CTrackListBox* CTrackListBoxView::CreateContainerL()
	{
	return CTrackListBox::NewL( ClientRect(), NULL, this );
	}


void CTrackListBoxView::SetTrackArrayL(const CDesCArray &aTrackArr)
	{
	if (!iTrackListBox)
		return;
	
	iTrackListBox->SetTrackArrayL(aTrackArr);
	}
