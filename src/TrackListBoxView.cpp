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
#include <aknnavide.h>
#include <aknnavi.h>
#include <eikbtgpc.h>
#include <GPSTracker.rsg>
// ]]] end generated region [Generated System Includes]

// [[[ begin generated region: do not modify [Generated User Includes]

#include "GPSTracker.hrh"
#include "TrackListBoxView.h"
#include "TrackerInfoListBox.hrh"
#include "TrackListBox.hrh"
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
	iNaviDecorator_ = NULL;
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
	if ( iNaviDecorator_ != NULL )
		{
		delete iNaviDecorator_;
		iNaviDecorator_ = NULL;
		}
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
	
		case EAknSoftkeyBack:
			commandHandled = HandleControlPaneRightSoftKeyPressedL( aCommand );
			break;
		case ETrackListBoxViewTrackDetailsMenuItemCommand:
			commandHandled = HandleTrackDetailsMenuItemSelectedL( aCommand );
			break;
		case ETrackListBoxViewDeleteTrackMenuItemCommand:
			commandHandled = HandleDeleteTrackMenuItemSelectedL( aCommand );
			break;
		default:
			break;
		}
	
		
	if ( !commandHandled ) 
		{
	
		if ( aCommand == EAknSoftkeyBack )
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
				
	// set the navi pane content
	TUid naviPaneUid = TUid::Uid( EEikStatusPaneUidNavi );
	CEikStatusPaneBase::TPaneCapabilities subPaneNavi = 
		StatusPane()->PaneCapabilities( naviPaneUid );
	if ( subPaneNavi.IsPresent() && subPaneNavi.IsAppOwned() )
		{
		CAknNavigationControlContainer* naviPane = 
			static_cast< CAknNavigationControlContainer* >( 
				StatusPane()->ControlL( naviPaneUid ) );
		if ( iNaviDecorator_ != NULL )
			{
			delete iNaviDecorator_;
			iNaviDecorator_ = NULL;
			}
				
		HBufC* labelText = StringLoader::LoadLC( R_TRACK_LIST_BOX_NAVI_TEXT );
		iNaviDecorator_ = naviPane->CreateNavigationLabelL( *labelText );
		CleanupStack::PopAndDestroy( labelText );			
				
		naviPane->PushL( *iNaviDecorator_ );
		}
				
	}

// ]]] end generated function

// [[[ begin generated function: do not modify
void CTrackListBoxView::CleanupStatusPane()
	{
	// reset the navi pane 
	TUid naviPaneUid = TUid::Uid( EEikStatusPaneUidNavi );
	CEikStatusPaneBase::TPaneCapabilities subPaneNavi = 
		StatusPane()->PaneCapabilities( naviPaneUid );
	if ( subPaneNavi.IsPresent() && subPaneNavi.IsAppOwned() )
		{
		CAknNavigationControlContainer* naviPane = 
			static_cast< CAknNavigationControlContainer* >( 
				StatusPane()->ControlL( naviPaneUid ) );
		if ( iNaviDecorator_ != NULL )
			{
			naviPane->Pop( iNaviDecorator_ );
			delete iNaviDecorator_;
			iNaviDecorator_ = NULL;
			}
		}
	
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
	CTrackListBox* lb = CTrackListBox::NewL( ClientRect(), NULL, this );
	
	// Add observer for items change
	lb->AddListBoxItemChangeObserverL(this);
	
	return lb;
	}

void CTrackListBoxView::ListBoxItemsChanged(CEikListBox* aListBox)
	{
	TInt count = aListBox->Model()->NumberOfItems();
	
	HBufC* naviText = StringLoader::LoadLC(R_TRACK_LIST_BOX_NAVI_TEXT, count, iEikonEnv);
	SetNaviPaneTextL(*naviText);
	CleanupStack::PopAndDestroy(naviText);
	}

void CTrackListBoxView::SetNaviPaneTextL(const TDesC& aNaviText)
	{
	// ToDo: Is it possible to change pane text without recreate label component?
	
	delete iNaviDecorator_;
	iNaviDecorator_ = NULL;
	
	CAknNavigationControlContainer* naviPane = static_cast<CAknNavigationControlContainer*>(
				StatusPane()->ControlL(TUid::Uid(EEikStatusPaneUidNavi)));
	
	iNaviDecorator_ = naviPane->CreateNavigationLabelL(aNaviText);
	naviPane->PushL(*iNaviDecorator_);
	}

void CTrackListBoxView::SetTrackArrayL(const CDesCArray &aTrackArr)
	{
	if (!iTrackListBox)
		return;
	
	iTrackListBox->SetTrackArrayL(aTrackArr);
	}
/** 
 * Handle the rightSoftKeyPressed event.
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CTrackListBoxView::HandleControlPaneRightSoftKeyPressedL( TInt aCommand )
	{
	// Go back to previous view
	AppUi()->ActivateViewL(iPrevViewId);
	
	return ETrue;
	}
				
/** 
 * Handle the selected event.
 * @param aCommand the command id invoked
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CTrackListBoxView::HandleTrackDetailsMenuItemSelectedL( TInt /*aCommand*/ )
	{
	// Read track file information
	HBufC* fileName = iTrackListBox->GetCurrentListBoxItemTextLC();
	/*if (fileName == NULL)
		return; // No selected item, exit*/
	TFileName fileFullName;
	static_cast<CGPSTrackerAppUi *>(AppUi())->TrackDir(fileFullName);
	fileFullName.Append(*fileName);
	TEntry fileEntry;
	User::LeaveIfError(iEikonEnv->FsSession().Entry(fileFullName, fileEntry));
	
	
	// Prepare strings
	// ToDo: Add points count, distance info
	TBuf<64> tmp;	
	CDesCArrayFlat* strings = new CDesCArrayFlat(3);
	CleanupStack::PushL(strings);
	// File name
	strings->AppendL(fileEntry.iName);
	// File size
	// ToDo: Show size in kb/mb
	tmp.Num(fileEntry.iSize);
	strings->AppendL(tmp);
	// File modify date
	// ToDo: I think, it will be better to get time from first (or last?) <trkpt> tag in GPX
	_LIT(KDateFmt, "%F%Y-%M-%D %H:%T:%S"); // ToDo: Use locale-specific format
	TTime modTime = fileEntry.iModified;
	// Convert UTC to local time
	TLocale locale;
	modTime += locale.UniversalTimeOffset();
	modTime.FormatL(tmp, KDateFmt);
	strings->AppendL(tmp);
	
	
	// Create and show message window
	HBufC* titleBuff = StringLoader::LoadLC(R_TRACK_LIST_BOX_TRACK_DETAILS_TITLE, iEikonEnv);
	HBufC* textBuff = StringLoader::LoadLC(R_TRACK_LIST_BOX_TRACK_DETAILS_TEXT, *strings, iEikonEnv);
	//iEikonEnv->AlertWin(*titleBuff, *textBuff);
	iEikonEnv->InfoWinL(*titleBuff, *textBuff);
	CleanupStack::PopAndDestroy(4, fileName);	
	
	
	return ETrue;
	}
				
/** 
 * Handle the selected event.
 * @param aCommand the command id invoked
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CTrackListBoxView::HandleDeleteTrackMenuItemSelectedL( TInt /*aCommand*/ )
	{
	// ToDo: Allow to mark multiple items for deletion
	// ToDo: Delete with "C" key
	// ToDo: Add "Delete all" in menu
	// ToDo: Add confirmation
	
	CGPSTrackerAppUi* appUi = static_cast<CGPSTrackerAppUi *>(AppUi());
	
	HBufC* fileName = iTrackListBox->GetCurrentListBoxItemTextLC();
	/*if (fileName == NULL)
		return; // No selected item, exit*/	
	TFileName fileFullName;
	appUi->TrackDir(fileFullName);
	fileFullName.Append(*fileName);
	
	TInt r = iCoeEnv->FsSession().Delete(fileFullName);
	if (r == KErrNone)
		{
		TInt idx = iTrackListBox->GetCurrentListBoxItemIdx();
		iTrackListBox->DeleteListBoxItemL(idx);
		}
	else
		{
		if (r == KErrInUse)
			{
			_LIT(KErrMsg, "Can`t delete active track file!");
			appUi->ShowError(KErrMsg);
			}
		else
			{
			_LIT(KErrMsgFmt, "Can`t delete file \"%S\"!");
			RBuf msg;
			msg.CreateL(KErrMsgFmt().Length() + fileName->Length());
			msg.Format(KErrMsgFmt, &(*fileName));
			appUi->ShowError(msg, r);
			msg.Close();
			}
		}
	
	return ETrue;
	}
				
