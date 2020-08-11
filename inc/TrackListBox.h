/*
========================================================================
 Name        : TrackListBox.h
 Author      : artem78
 Copyright   : 
 Description : 
========================================================================
*/
#ifndef TRACKLISTBOX_H
#define TRACKLISTBOX_H

// [[[ begin generated region: do not modify [Generated Includes]
#include <coecntrl.h>		
// ]]] end generated region [Generated Includes]


// [[[ begin [Event Handler Includes]
// ]]] end [Event Handler Includes]

// [[[ begin generated region: do not modify [Generated Forward Declarations]
class MEikCommandObserver;		
class CAknSingleStyleListBox;
class CEikTextListBox;
// ]]] end generated region [Generated Forward Declarations]

/**
 * Container class for TrackListBox
 * 
 * @class	CTrackListBox TrackListBox.h
 */
class CTrackListBox : public CCoeControl
	{
public:
	// constructors and destructor
	CTrackListBox();
	static CTrackListBox* NewL( 
		const TRect& aRect, 
		const CCoeControl* aParent, 
		MEikCommandObserver* aCommandObserver );
	static CTrackListBox* NewLC( 
		const TRect& aRect, 
		const CCoeControl* aParent, 
		MEikCommandObserver* aCommandObserver );
	void ConstructL( 
		const TRect& aRect, 
		const CCoeControl* aParent, 
		MEikCommandObserver* aCommandObserver );
	virtual ~CTrackListBox();

public:
	// from base class CCoeControl
	TInt CountComponentControls() const;
	CCoeControl* ComponentControl( TInt aIndex ) const;
	TKeyResponse OfferKeyEventL( 
			const TKeyEvent& aKeyEvent, 
			TEventCode aType );
	void HandleResourceChange( TInt aType );
	
protected:
	// from base class CCoeControl
	void SizeChanged();

private:
	// from base class CCoeControl
	void Draw( const TRect& aRect ) const;

private:
	void InitializeControlsL();
	void LayoutControls();
	CCoeControl* iFocusControl;
	MEikCommandObserver* iCommandObserver;
	// [[[ begin generated region: do not modify [Generated Methods]
public: 
	static void AddListBoxItemL( 
			CEikTextListBox* aListBox,
			const TDesC& aString );
	static RArray< TInt >* GetSelectedListBoxItemsLC( CEikTextListBox* aListBox );
	static void DeleteSelectedListBoxItemsL( CEikTextListBox* aListBox );
	CAknSingleStyleListBox* ListBox();
	static void CreateListBoxItemL( TDes& aBuffer, 
			const TDesC& aMainText );
	void AddListBoxResourceArrayItemL( TInt aResourceId );
	void SetupListBoxIconsL();
	TBool HandleMarkableListCommandL( TInt aCommand );
	// ]]] end generated region [Generated Methods]
	
	// [[[ begin generated region: do not modify [Generated Type Declarations]
public: 
	// ]]] end generated region [Generated Type Declarations]
	
	// [[[ begin generated region: do not modify [Generated Instance Variables]
private: 
	CAknSingleStyleListBox* iListBox;
	// ]]] end generated region [Generated Instance Variables]
	
	
	// [[[ begin [Overridden Methods]
protected: 
	// ]]] end [Overridden Methods]
	
	
	// [[[ begin [User Handlers]
protected: 
	// ]]] end [User Handlers]
	
public: 
	enum TControls
		{
		// [[[ begin generated region: do not modify [Generated Contents]
		EListBox,
		
		// ]]] end generated region [Generated Contents]
		
		// add any user-defined entries here...
		
		ELastControl
		};
	enum TListBoxImages
		{
		// [[[ begin generated region: do not modify [Generated Enums]
		EListBoxFirstUserImageIndex
		
		// ]]] end generated region [Generated Enums]
		
		};

private:
	static void ParseListBoxItemL(TDes& aItemValue, const TDesC& aItemText);
	
public:
	void AddListBoxItemChangeObserverL(MListBoxItemChangeObserver* aObserver);
	void SetTrackArrayL(const CDesCArray &aTrackArr);
	//HBufC* GetSelectedListBoxItemTextLC() const;
	HBufC* GetCurrentListBoxItemTextLC() const;
	};
				
#endif // TRACKLISTBOX_H
