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

#include <aknprogressdialog.h>


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
	static TInt RunRenameQueryL( 
			TDes& aData, 
			TBool aUseDefaults = ETrue, 
			const TDesC* aOverridePrompt = NULL );
	static TInt RunDeleteConfQueryL( const TDesC* aOverrideText = NULL );
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
	TBool HandleControlPaneRightSoftKeyPressedL( TInt aCommand );
	TBool HandleTrackDetailsMenuItemSelectedL( TInt aCommand );
	TBool HandleDeleteTrackMenuItemSelectedL( TInt aCommand );
	TBool HandleRenameTrackMenuItemSelectedL( TInt aCommand );
	TBool HandleDeleteAllTracksMenuItemSelectedL( TInt aCommand );
	void HandleDeletionWaitDialogCanceledL( CAknProgressDialog* aDialog );
	TBool HandleSendTrackMenuItemSelectedL( TInt aCommand );
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
	
	CAknProgressDialog* iDeletionProgressDlg; // Tracks deletion dialog with progress bar
	class CProgressDialogCallback;
	CProgressDialogCallback* iDeletionProgressDlgCallback; // Callback for cancel deletion dialog 
	CPeriodic* iDeletionProgressDlgRefreshTimer; // For periodically refresh progress bar position
	
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
	
	void ExecuteDeletionProgressDlgL();
	
	// If called from Cancel callback, anExceptDialog must be set to ETrue, because
	// the dialog already has been destroyed by ui framework
	void RemoveDeletionProgressDlgL(TBool anExceptDialog=EFalse);
	static TInt UpdateTrackDeletionProgress(TAny* anObject);
	
	
	// [[[ begin [MProgressDialogCallback support]
private: 
	typedef void ( CTrackListBoxView::*ProgressDialogEventHandler )( 
			CAknProgressDialog* aProgressDialog );
	
	/**
	 * This is a helper class for progress/wait dialog callbacks. It routes the dialog's
	 * cancel notification to the handler function for the cancel event.
	 */
	class CProgressDialogCallback : public CBase, public MProgressDialogCallback
		{ 
		public:
			CProgressDialogCallback( 
					CTrackListBoxView* aHandlerObj, 
					CAknProgressDialog* aDialog, 
					ProgressDialogEventHandler aHandler ) :
				handlerObj( aHandlerObj ), dialog( aDialog ), handler( aHandler )
				{}
				
			void DialogDismissedL( TInt aButtonId ) 
				{
				( handlerObj->*handler )( dialog );
				}
		private:
			CTrackListBoxView* handlerObj;
			CAknProgressDialog* dialog;
			ProgressDialogEventHandler handler;
		};
		
	// ]]] end [MProgressDialogCallback support]
	
	};

#endif // TRACKLISTBOXVIEW_H
