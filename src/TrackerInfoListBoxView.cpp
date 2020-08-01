/*
========================================================================
 Name        : TrackerInfoListBoxView.cpp
 Author      : artem78
 Copyright   : General Public License v3.0
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
#include "TrackerInfoListBoxView.h"
#include "TrackerInfoListBox.hrh"
#include "TrackerInfoListBox.h"
// ]]] end generated region [Generated User Includes]

#include "GPSTrackerAppUi.h"
#include "Logger.h"

// [[[ begin generated region: do not modify [Generated Constants]
// ]]] end generated region [Generated Constants]

/**
 * First phase of Symbian two-phase construction. Should not contain any
 * code that could leave.
 */
CTrackerInfoListBoxView::CTrackerInfoListBoxView()
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	iTrackerInfoListBox = NULL;
	// ]]] end generated region [Generated Contents]
	
	}

/** 
 * The view's destructor removes the container from the control
 * stack and destroys it.
 */
CTrackerInfoListBoxView::~CTrackerInfoListBoxView()
	{
	DEBUG(_L("InfoListBoxView destructor start"));
	
	// [[[ begin generated region: do not modify [Generated Contents]
	delete iTrackerInfoListBox;
	iTrackerInfoListBox = NULL;
	// ]]] end generated region [Generated Contents]
	
	DEBUG(_L("InfoListBoxView destructor end"));
	}

/**
 * Symbian two-phase constructor.
 * This creates an instance then calls the second-phase constructor
 * without leaving the instance on the cleanup stack.
 * @return new instance of CTrackerInfoListBoxView
 */
CTrackerInfoListBoxView* CTrackerInfoListBoxView::NewL()
	{
	CTrackerInfoListBoxView* self = CTrackerInfoListBoxView::NewLC();
	CleanupStack::Pop( self );
	return self;
	}

/**
 * Symbian two-phase constructor.
 * This creates an instance, pushes it on the cleanup stack,
 * then calls the second-phase constructor.
 * @return new instance of CTrackerInfoListBoxView
 */
CTrackerInfoListBoxView* CTrackerInfoListBoxView::NewLC()
	{
	CTrackerInfoListBoxView* self = new ( ELeave ) CTrackerInfoListBoxView();
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}


/**
 * Second-phase constructor for view.  
 * Initialize contents from resource.
 */ 
void CTrackerInfoListBoxView::ConstructL()
	{
	// [[[ begin generated region: do not modify [Generated Code]
	BaseConstructL( R_TRACKER_INFO_LIST_BOX_TRACKER_INFO_LIST_BOX_VIEW );
				
	// ]]] end generated region [Generated Code]
	
	// add your own initialization code here
	
	}

/**
 * @return The UID for this view
 */
TUid CTrackerInfoListBoxView::Id() const
	{
	return TUid::Uid( ETrackerInfoListBoxViewId );
	}

/**
 * Handle a command for this view (override)
 * @param aCommand command id to be handled
 */
void CTrackerInfoListBoxView::HandleCommandL( TInt aCommand )
	{
	// [[[ begin generated region: do not modify [Generated Code]
	TBool commandHandled = EFalse;
	switch ( aCommand )
		{	// code to dispatch to the AknView's menu and CBA commands is generated here
		case ETrackerInfoListBoxViewPauseTrackRecordingMenuItemCommand:
			commandHandled = HandlePauseTrackRecordingMenuItemSelectedL( aCommand );
			break;
		case ETrackerInfoListBoxViewContinueTrackRecordingMenuItemCommand:
			commandHandled = HandleContinueTrackRecordingMenuItemSelectedL( aCommand );
			break;
		default:
			break;
		}
	
		
	if ( !commandHandled ) 
		{
	
		if ( aCommand == EAknSoftkeyExit )
			{
			AppUi()->HandleCommandL( EEikCmdExit );
			}
	
		}
	// ]]] end generated region [Generated Code]
	
	}

/**
 *	Handles user actions during activation of the view, 
 *	such as initializing the content.
 */
void CTrackerInfoListBoxView::DoActivateL( 
		const TVwsViewId& /*aPrevViewId*/,
		TUid /*aCustomMessageId*/,
		const TDesC8& /*aCustomMessage*/ )
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	SetupStatusPaneL();
	
				
	if ( iTrackerInfoListBox == NULL )
		{
		iTrackerInfoListBox = CreateContainerL();
		iTrackerInfoListBox->SetMopParent( this );
		AppUi()->AddToStackL( *this, iTrackerInfoListBox );
		} 
	// ]]] end generated region [Generated Contents]
	
	}

/**
 */
void CTrackerInfoListBoxView::DoDeactivate()
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	CleanupStatusPane();
	
	if ( iTrackerInfoListBox != NULL )
		{
		AppUi()->RemoveFromViewStack( *this, iTrackerInfoListBox );
		delete iTrackerInfoListBox;
		iTrackerInfoListBox = NULL;
		}
	// ]]] end generated region [Generated Contents]
	
	}

/** 
 * Handle status pane size change for this view (override)
 */
void CTrackerInfoListBoxView::HandleStatusPaneSizeChange()
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
void CTrackerInfoListBoxView::SetupStatusPaneL()
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
		iEikonEnv->CreateResourceReaderLC( reader, R_TRACKER_INFO_LIST_BOX_TITLE_RESOURCE );
		title->SetFromResourceL( reader );
		CleanupStack::PopAndDestroy(); // reader internal state
		}
				
	}

// ]]] end generated function

// [[[ begin generated function: do not modify
void CTrackerInfoListBoxView::CleanupStatusPane()
	{
	}

// ]]] end generated function

/**
 *	Creates the top-level container for the view.  You may modify this method's
 *	contents and the CTrackerInfoListBox::NewL() signature as needed to initialize the
 *	container, but the signature for this method is fixed.
 *	@return new initialized instance of CTrackerInfoListBox
 */
CTrackerInfoListBox* CTrackerInfoListBoxView::CreateContainerL()
	{
	return CTrackerInfoListBox::NewL( ClientRect(), NULL, this );
	}

/** 
 * Handle the selected event.
 * @param aCommand the command id invoked
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CTrackerInfoListBoxView::HandlePauseTrackRecordingMenuItemSelectedL( TInt aCommand )
	{
	static_cast<CGPSTrackerAppUi *>(AppUi())->StopTracking();
	
	return ETrue;
	}
				
/** 
 * Handle the selected event.
 * @param aCommand the command id invoked
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CTrackerInfoListBoxView::HandleContinueTrackRecordingMenuItemSelectedL( TInt aCommand )
	{
	static_cast<CGPSTrackerAppUi *>(AppUi())->StartTracking();
	
	return ETrue;
	}

void CTrackerInfoListBoxView::DynInitMenuPaneL(TInt aMenuID, CEikMenuPane* aMenuPane)
	{
	if (aMenuID == R_TRACKER_INFO_LIST_BOX_MENU_PANE1_MENU_PANE)
		{
		if (!static_cast<CGPSTrackerAppUi *>(AppUi())->IsTrackingRunning())
			{
			aMenuPane->SetItemDimmed(ETrackerInfoListBoxViewPauseTrackRecordingMenuItemCommand, ETrue);
			aMenuPane->SetItemDimmed(ETrackerInfoListBoxViewContinueTrackRecordingMenuItemCommand, EFalse);
			}
		else
			{
			aMenuPane->SetItemDimmed(ETrackerInfoListBoxViewContinueTrackRecordingMenuItemCommand, ETrue);
			aMenuPane->SetItemDimmed(ETrackerInfoListBoxViewPauseTrackRecordingMenuItemCommand, EFalse);
			}
		}
	else
		{
		AppUi()->DynInitMenuPaneL(aMenuID, aMenuPane);
		}
	}

void CTrackerInfoListBoxView::SetDataL(TReal aLat, TReal aLon, TReal aAlt, TReal aSpeed,
		TInt aSatUsed, TInt aSatTotal, TInt aSavedPoints, TTimeIntervalMicroSeconds aPosRefreshRate)
	{
	if (!iTrackerInfoListBox) // Check that list box created
		return;
	
	// Constants
	const TChar KForwardSlash = TChar(0x2F);
	const TChar KSpace = TChar(0x20);
	_LIT(KTextNoValue, "-----");
	_LIT(KMetersUnit, "m");
	_LIT(KKilometersPerHourUnit, "km/h");
	_LIT(KSecondsUnit, "s");
	const TChar KDegree = TChar(0xB0);
	const TInt KMicroSecondsPerSecond = 1000000;

	const TRealFormat KShortRealFmt = TRealFormat(10, 1);
	const TRealFormat KLongRealFmt = TRealFormat(10, 5);


	// Variables
	TBuf<100> valBuff;

	
	// Latitude
	if (!Math::IsNaN(aLat))
		{
		valBuff.Zero();
		valBuff.AppendNum(aLat, KLongRealFmt);
		valBuff.Append(KDegree);
		}
	else
		valBuff.Copy(KTextNoValue);
	iTrackerInfoListBox->SetItemValueL(ELatitudeItem, valBuff);

	// Longitude
	if (!Math::IsNaN(aLon))
		{
		valBuff.Zero();
		valBuff.AppendNum(aLon, KLongRealFmt);
		valBuff.Append(KDegree);
		}
	else
		valBuff.Copy(KTextNoValue);
	iTrackerInfoListBox->SetItemValueL(ELongitudeItem, valBuff);
	
	// Altitude
	if (!Math::IsNaN(aAlt))
		{
		valBuff.Zero();
		valBuff.AppendNum(aAlt, KShortRealFmt);
		valBuff.Append(KSpace);
		valBuff.Append(KMetersUnit);
		}
	else
		valBuff.Copy(KTextNoValue);
	iTrackerInfoListBox->SetItemValueL(EAltitudeItem, valBuff);
		
	// Speed
	if (!Math::IsNaN(aSpeed))
		{
		valBuff.Zero();
		valBuff.AppendNum(aSpeed, KShortRealFmt);
		valBuff.Append(KSpace);
		valBuff.Append(KKilometersPerHourUnit);
		}
	else
		valBuff.Copy(KTextNoValue);
	iTrackerInfoListBox->SetItemValueL(ESpeedItem, valBuff);
	
	// Satellites
	valBuff.Zero();
	valBuff.AppendNum(aSatUsed);
	valBuff.Append(KSpace);
	valBuff.Append(KForwardSlash);
	valBuff.Append(KSpace);
	valBuff.AppendNum(aSatTotal);
	iTrackerInfoListBox->SetItemValueL(ESatellitesItem, valBuff);
	
	// Saved points
	valBuff.Zero();
	valBuff.AppendNum(aSavedPoints);
	iTrackerInfoListBox->SetItemValueL(ESavedPointsItem, valBuff);
	
	// Position update interval
	valBuff.Zero();
	valBuff.AppendNum(aPosRefreshRate.Int64() / TReal(KMicroSecondsPerSecond), KShortRealFmt);
	valBuff.Append(KSpace);
	valBuff.Append(KSecondsUnit);
	iTrackerInfoListBox->SetItemValueL(EPositionRefreshRateItem, valBuff);
	
	
	// Redraw listbox component
	iTrackerInfoListBox->DrawNow();
	
	}

				
