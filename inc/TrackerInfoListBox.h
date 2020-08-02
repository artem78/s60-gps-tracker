/*
========================================================================
 Name        : TrackerInfoListBox.h
 Author      : artem78
 Copyright   : General Public License v3.0
 Description : 
========================================================================
*/
#ifndef TRACKERINFOLISTBOX_H
#define TRACKERINFOLISTBOX_H

// [[[ begin generated region: do not modify [Generated Includes]
#include <coecntrl.h>		
// ]]] end generated region [Generated Includes]


// [[[ begin [Event Handler Includes]
// ]]] end [Event Handler Includes]

// [[[ begin generated region: do not modify [Generated Forward Declarations]
class MEikCommandObserver;		
class CAknDoubleStyleListBox;
class CEikTextListBox;
// ]]] end generated region [Generated Forward Declarations]

/**
 * Container class for TrackerInfoListBox
 * 
 * @class	CTrackerInfoListBox TrackerInfoListBox.h
 */
class CTrackerInfoListBox : public CCoeControl
	{
public:
	// constructors and destructor
	CTrackerInfoListBox();
	static CTrackerInfoListBox* NewL( 
		const TRect& aRect, 
		const CCoeControl* aParent, 
		MEikCommandObserver* aCommandObserver );
	static CTrackerInfoListBox* NewLC( 
		const TRect& aRect, 
		const CCoeControl* aParent, 
		MEikCommandObserver* aCommandObserver );
	void ConstructL( 
		const TRect& aRect, 
		const CCoeControl* aParent, 
		MEikCommandObserver* aCommandObserver );
	virtual ~CTrackerInfoListBox();

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
	CAknDoubleStyleListBox* ListBox();
	static void CreateListBoxItemL( TDes& aBuffer, 
			const TDesC& aMainText,
			const TDesC& aSecondaryText );
	void AddListBoxResourceArrayItemL( TInt aResourceId );
	void SetupListBoxIconsL();
	TBool HandleMarkableListCommandL( TInt aCommand );
	// ]]] end generated region [Generated Methods]
	
	// [[[ begin generated region: do not modify [Generated Type Declarations]
public: 
	// ]]] end generated region [Generated Type Declarations]
	
	// [[[ begin generated region: do not modify [Generated Instance Variables]
private: 
	CAknDoubleStyleListBox* iListBox;
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
	
public:
	void SetItemValueL(TInt anIdx, const TDesC &aVal);

	};
				
#endif // TRACKERINFOLISTBOX_H
