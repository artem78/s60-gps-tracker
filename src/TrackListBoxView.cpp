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
#include <gdi.h>
#include <eikedwin.h>
#include <aknquerydialog.h>
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

#include "Logger.h"
#include "MiscUtils.h"
#include <eikprogi.h> // For CEikProgressInfo
#include <sendui.h>
#include <cmessagedata.h>

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
	
	TRAP_IGNORE(RemoveDeletionProgressDlgL());
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
		case ETrackListBoxViewSendMenuItemCommand:
			commandHandled = HandleSendTrackMenuItemSelectedL( aCommand );
			break;
		case ETrackListBoxViewRenameTrackMenuItemCommand:
			commandHandled = HandleRenameTrackMenuItemSelectedL( aCommand );
			break;
		case ETrackListBoxViewDeleteTrackMenuItemCommand:
			commandHandled = HandleDeleteTrackMenuItemSelectedL( aCommand );
			break;
		case ETrackListBoxViewDeleteAllTracksMenuItemCommand:
			commandHandled = HandleDeleteAllTracksMenuItemSelectedL( aCommand );
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
	CGPSTrackerAppUi* appUi = static_cast<CGPSTrackerAppUi *>(AppUi());
	
	// Read track file information
	HBufC* fileName = iTrackListBox->GetCurrentListBoxItemTextLC();
	/*if (fileName == NULL)
		return; // No selected item, exit*/
	TFileName fileFullName;
	appUi->TrackDir(fileFullName);
	fileFullName.Append(*fileName);
	TEntry fileEntry;
	User::LeaveIfError(iEikonEnv->FsSession().Entry(fileFullName, fileEntry));
	
	
	// Prepare strings
	TBuf<64> tmp;	
	CDesCArrayFlat* strings = new (ELeave) CDesCArrayFlat(4);
	CleanupStack::PushL(strings);
	CArrayFixFlat<TInt>* nums = new (ELeave) CArrayFixFlat<TInt>(1);
	CleanupStack::PushL(nums);
	// File name
	strings->AppendL(fileEntry.iName);
	// File size
	FileUtils::FileSizeToReadableString(fileEntry.iSize, tmp);
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
	
	// Points count and distance
	RFile file;
	User::LeaveIfError(file.Open(iCoeEnv->FsSession(), fileFullName, EFileRead));
	CleanupClosePushL(file);
	/*TFileText fileText;
	fileText.Set(file);
	fileText.Seek(ESeekStart);
	TBuf<256> buf;
	_LIT(KSearch, "<trkpt ");
	TInt points = 0;
	while (fileText.Read(buf) != KErrEof)
	{
		if (buf.Find(KSearch) != KErrNotFound)
			{
				points++;
			}
	}*/
	TInt points = 0;
	TReal32 distance = 0;
	HBufC8* bufc = NULL;
	TInt len = 0;
	TCoordinate coord, prevCoord;
	const TReal KNaN = 0.0 / 0.0;
	coord.SetCoordinate(KNaN, KNaN);
	prevCoord.SetCoordinate(KNaN, KNaN);
	if (file.Size(len) == KErrNone && len > 0)
	{
		bufc = HBufC8::NewL(len);
		TPtr8 buf(bufc->Des());
		if (file.Read(0, buf) == KErrNone)
		{
			_LIT8(KSearchTrkpt, "<trkpt ");
			_LIT8(KSearchLat, "lat=\"");
			_LIT8(KSearchLon, "lon=\"");
			TPtrC8 ptr(bufc->Des());
			while (true)
			{
				if (ptr.Length() < 1)
				{
					break;
				}
				
				TInt pos = ptr.Find(KSearchTrkpt);
				if (pos == KErrNotFound)
				{
					break;
				}
				else
				{
					TInt latPos = ptr.Find(KSearchLat);
					TInt lonPos = ptr.Find(KSearchLon);
					if (latPos != KErrNotFound and lonPos != KErrNotFound)
						{
						latPos += KSearchLat().Length();
						lonPos += KSearchLon().Length();
						TPtrC8 latPtr = ptr.Mid(latPos, 16);
						TPtrC8 lonPtr = ptr.Mid(lonPos, 16);
						
						TInt latQuotePos = latPtr.Locate('"');
						TInt lonQuotePos = lonPtr.Locate('"');
						if (latQuotePos != KErrNotFound && lonQuotePos != KErrNotFound)
							{
							latPtr.Set(latPtr.Left(latQuotePos));
							lonPtr.Set(lonPtr.Left(lonQuotePos));
							
							TReal64 lat = /*0*/ KNaN;
							TReal64 lon = /*0*/ KNaN;
							TLex8 lex;
							lex.Assign(latPtr);
							if (lex.Val(lat) == KErrNone)
								{
								
								}
							else
								{
								// something wrong...
								}
							
							lex.Assign(lonPtr);
							if (lex.Val(lon) == KErrNone)
								{
								
								}
							else
								{
								// something wrong...
								}
							
							coord.SetCoordinate(lat, lon);

							
							}
						else
							{
							// something wrong...
							}
						
						}
					else
						{
						// something wrong...
						}
						
					
					if (points > 0)
						{
						TReal32 chunkDist = 0;
						if (coord.Distance(prevCoord, chunkDist) == KErrNone)
							{
							distance += chunkDist;
							}
						else
							{
							// something wrong...
							}
						}
					
					ptr.Set(ptr.Right(ptr.Length() - pos - KSearchTrkpt().Length()));
					
					points++;
					prevCoord = coord;
				}
			}
		}
		else
		{
			// something wrong...
		}
		delete bufc;
	}
	else
	{
		// something wrong...
	}
	CleanupStack::PopAndDestroy(&file);
	nums->AppendL(points);
	tmp.Num((TInt) distance);
	tmp.Append(' ');
	tmp.Append('m');
	strings->AppendL(tmp);
	
	
	// Create and show message window
	HBufC* titleBuff = StringLoader::LoadLC(R_TRACK_LIST_BOX_TRACK_DETAILS_TITLE, iEikonEnv);
	HBufC* textBuff = StringLoader::LoadLC(R_TRACK_LIST_BOX_TRACK_DETAILS_TEXT, *strings, *nums, iEikonEnv);
	appUi->ShowMsgL(*titleBuff, *textBuff);
	CleanupStack::PopAndDestroy(5, fileName);	
	
	
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
	
	// Confirmation dialog
	if (RunDeleteConfQueryL(NULL) != EAknSoftkeyYes)
		return ETrue;
	
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
			HBufC* errMsg = StringLoader::LoadL(R_TRACK_LIST_BOX_ACTIVE_FILE_DELETION_ERROR_TEXT, iEikonEnv);
			appUi->ShowError(*errMsg);
			delete errMsg;
			}
		else
			{
			HBufC* errMsg = StringLoader::LoadL(R_TRACK_LIST_BOX_FILE_DELETION_ERROR_TEXT, *fileName, iEikonEnv);
			appUi->ShowError(*errMsg, r);
			delete errMsg;
			}
		}
	
	CleanupStack::PopAndDestroy(fileName);
	
	return ETrue;
	}
				
/** 
 * Handle the selected event.
 * @param aCommand the command id invoked
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CTrackListBoxView::HandleRenameTrackMenuItemSelectedL( TInt /*aCommand*/ )
	{
	CGPSTrackerAppUi* appUi = static_cast<CGPSTrackerAppUi *>(AppUi());
	RFs fs = iEikonEnv->FsSession();
	
	HBufC* oldFileName = iTrackListBox->GetCurrentListBoxItemTextLC();
	/*if (fileName == NULL)
		return; // No selected item, exit*/	
	
	TFileName newFileName;
	newFileName.Copy(*oldFileName);
	
	
	if (RunRenameQueryL(newFileName, /*ETrue*/ EFalse, NULL) == EAknSoftkeyOk)
		{
		TFileName oldFileFullName;
		appUi->TrackDir(oldFileFullName);
		oldFileFullName.Append(*oldFileName);
		
		TFileName newFileFullName;
		appUi->TrackDir(newFileFullName);
		newFileFullName.Append(newFileName);
		
		// Check if filename is correct
		if (!fs.IsValidName(/*newFileFullName*/ newFileName))
			{
			HBufC* errMsg = StringLoader::LoadL(R_TRACK_LIST_BOX_INCORRECT_FILE_NAME_ERROR_TEXT, iEikonEnv);
			appUi->ShowError(*errMsg);
			delete errMsg;
			}
		else
			{
			// Try to rename file
			TInt r = fs.Rename(oldFileFullName, newFileFullName);
			if (r == KErrNone)
				{
				appUi->UpdateTrackListL();
				}
			else
				{
				if (r == KErrInUse)
					{
					HBufC* errMsg = StringLoader::LoadL(R_TRACK_LIST_BOX_ACTIVE_FILE_RENAMING_ERROR_TEXT, iEikonEnv);
					appUi->ShowError(*errMsg);
					delete errMsg;
					}
				else
					{
					HBufC* errMsg = StringLoader::LoadL(R_TRACK_LIST_BOX_FILE_RENAMING_ERROR_TEXT, *oldFileName, iEikonEnv);
					appUi->ShowError(*errMsg, r);
					delete errMsg;
					}
				}
			}
		}
	
	CleanupStack::PopAndDestroy(oldFileName);
	
	return ETrue;
	}
				
// [[[ begin generated function: do not modify
/**
 * Show the popup dialog for renameQuery
 * @param aData in-out TDes data
 * @param aUseDefaults TBool use designer default editor data if ETrue
 * @param aOverridePrompt optional override prompt text
 * @return EAknSoftkeyOk (left soft key id) or 0
 */
TInt CTrackListBoxView::RunRenameQueryL( 
		TDes& aData, 
		TBool aUseDefaults, 
		const TDesC* aOverridePrompt )
	{
	if ( aUseDefaults )
		{
		HBufC* text = StringLoader::LoadLC( R_TRACK_LIST_BOX_EDIT1 );
		aData.Copy( *text );
		CleanupStack::PopAndDestroy( text );
		}
				
	CAknTextQueryDialog* queryDialog = CAknTextQueryDialog::NewL( aData );	
	
	if ( aOverridePrompt != NULL )
		{
		CleanupStack::PushL( queryDialog );
		queryDialog->SetPromptL( *aOverridePrompt );
		CleanupStack::Pop(); // queryDialog
		}
	
	return queryDialog->ExecuteLD( R_TRACK_LIST_BOX_RENAME_QUERY );
	}
// ]]] end generated function

/** 
 * Handle the selected event.
 * @param aCommand the command id invoked
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CTrackListBoxView::HandleDeleteAllTracksMenuItemSelectedL( TInt /*aCommand*/ )
	{
	CGPSTrackerAppUi* appUi = static_cast<CGPSTrackerAppUi *>(AppUi());
	
	// Confirmation dialog
	if (RunDeleteConfQueryL(NULL) == EAknSoftkeyYes)
		appUi->DeleteAllTracks();
	
	return ETrue;
	}
				
void CTrackListBoxView::ExecuteDeletionProgressDlgL()
	{
	if (iDeletionProgressDlg == NULL)
		{
		DEBUG(_L("Execute tracks deletion dialog"));
		
		// Initialization of tracks deletion progress dialog
		iDeletionProgressDlg = new (ELeave) CAknProgressDialog(REINTERPRET_CAST(CEikDialog**, &iDeletionProgressDlg));
		iDeletionProgressDlg->PrepareLC(R_TRACK_LIST_BOX_DELETION_PROGRESS_DIALOG);
		CEikProgressInfo* progressInfo = iDeletionProgressDlg->GetProgressInfoL();
		CGPSTrackerAppUi* appUi = static_cast<CGPSTrackerAppUi *>(AppUi());
		/*TInt totalCount = appUi->iAsyncFileMan->TotalFiles(); // equals to 0
		progressInfo->SetFinalValue(totalCount);*/
		iDeletionProgressDlg->RunLD();
		}
	
	if  (iDeletionProgressDlgCallback == NULL)
		{
		// Dialog callback for cancel
		iDeletionProgressDlgCallback = new (ELeave) CProgressDialogCallback(this,
				iDeletionProgressDlg, &CTrackListBoxView::HandleDeletionWaitDialogCanceledL);
		iDeletionProgressDlg->SetCallback(iDeletionProgressDlgCallback);
		}
		
	if (iDeletionProgressDlgRefreshTimer == NULL)
		{
		// Starts periodic progress bar update
		TTimeIntervalMicroSeconds32 updateInterval = KSecond / 15;
		TCallBack callback(UpdateTrackDeletionProgress, this);
		iDeletionProgressDlgRefreshTimer = CPeriodic::NewL(EPriorityNormal);
		iDeletionProgressDlgRefreshTimer->Start(0 /*updateInterval*/, updateInterval, callback);
		}
	}

void CTrackListBoxView::RemoveDeletionProgressDlgL(TBool anExceptDialog)
	{
	// Dialog
	if (!anExceptDialog)
		{
		if (iDeletionProgressDlg != NULL)
			{
			DEBUG(_L("Remove tracks deletion dialog"));
			
			iDeletionProgressDlg->SetCallback(NULL);
			iDeletionProgressDlg->ProcessFinishedL();
			iDeletionProgressDlg = NULL;
			}
		}
		
	// Dialog cancel callback
	delete iDeletionProgressDlgCallback;
	iDeletionProgressDlgCallback = NULL;
	
	// Progress refresh timer
	if (iDeletionProgressDlgRefreshTimer != NULL)
		{
		iDeletionProgressDlgRefreshTimer->Cancel();
		delete iDeletionProgressDlgRefreshTimer;
		iDeletionProgressDlgRefreshTimer = NULL;
		}
	}

TInt CTrackListBoxView::UpdateTrackDeletionProgress(TAny* anObject)
	{
	CTrackListBoxView* view = static_cast<CTrackListBoxView*>(anObject);
	CGPSTrackerAppUi* appUi = static_cast<CGPSTrackerAppUi*>(view->AppUi());
	
	if (view->iDeletionProgressDlg == NULL)
		return (TInt) ETrue;
	
	CEikProgressInfo* progressInfo = NULL;
	TRAPD(r, progressInfo = view->iDeletionProgressDlg->GetProgressInfoL());
	if (r == KErrNone && progressInfo != NULL)
		{
		TInt totalCount = appUi->iAsyncFileMan->TotalFiles();
		progressInfo->SetFinalValue(totalCount); // ToDo: May be done one time after deletion started
		
		TInt processedCount = appUi->iAsyncFileMan->ProcessedFiles();
		progressInfo->SetAndDraw(processedCount);
		}
	
	return (TInt) ETrue;
	}

/** 
 * Handle the canceled event.
 */
void CTrackListBoxView::HandleDeletionWaitDialogCanceledL( CAknProgressDialog* /* aDialog */ )
	{
	CGPSTrackerAppUi* appUi = static_cast<CGPSTrackerAppUi *>(AppUi());
	appUi->CancelCurrentFManOperation();
	
	// iDeletionProgressDlg destroys automatically when cancelled,
	// but iDeletionProgressDlgCallback and iDeletionProgressDlgRefreshTimer
	// need to be free manually.
	RemoveDeletionProgressDlgL(ETrue);
	}
				
// [[[ begin generated function: do not modify
/**
 * Show the popup dialog for deleteConfQuery
 * @param aOverrideText optional override text
 * @return EAknSoftkeyYes (left soft key id) or 0
 */
TInt CTrackListBoxView::RunDeleteConfQueryL( const TDesC* aOverrideText )
	{
				
	CAknQueryDialog* queryDialog = CAknQueryDialog::NewL();	
	
	if ( aOverrideText != NULL )
		{
		queryDialog->SetPromptL( *aOverrideText );
		}
	return queryDialog->ExecuteLD( R_TRACK_LIST_BOX_DELETE_CONF_QUERY );
	}
// ]]] end generated function

/** 
 * Handle the selected event.
 * @param aCommand the command id invoked
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CTrackListBoxView::HandleSendTrackMenuItemSelectedL( TInt /*aCommand*/ )
	{
	// ToDo: Allow to mark multiple items for sending
	
	CGPSTrackerAppUi* appUi = static_cast<CGPSTrackerAppUi *>(AppUi());
	
	// Read track file information
	HBufC* fileName = iTrackListBox->GetCurrentListBoxItemTextLC();
	/*if (fileName == NULL)
		return; // No selected item, exit*/
	TFileName fileFullName;
	appUi->TrackDir(fileFullName);
	fileFullName.Append(*fileName);
	TEntry fileEntry;
	User::LeaveIfError(iEikonEnv->FsSession().Entry(fileFullName, fileEntry));
	
	// Send
	CSendUi* sendUi = CSendUi::NewLC();
	CMessageData* message = CMessageData::NewLC();
	TSendingCapabilities sendCaps;
	sendCaps.iFlags = TSendingCapabilities::ESupportsAttachments;
	message->AppendAttachmentL(fileFullName);
	
	sendUi->ShowQueryAndSendL(message, sendCaps);
	
	CleanupStack::PopAndDestroy(3, sendUi);
	
	return ETrue;
	}
				
