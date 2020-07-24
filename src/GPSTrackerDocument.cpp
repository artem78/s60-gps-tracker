/*
========================================================================
 Name        : GPSTrackerDocument.cpp
 Author      : artem78
 Copyright   : General Public License v3.0
 Description : 
========================================================================
*/
// [[[ begin generated region: do not modify [Generated User Includes]
#include "GPSTrackerDocument.h"
#include "GPSTrackerAppUi.h"
// ]]] end generated region [Generated User Includes]

/**
 * @brief Constructs the document class for the application.
 * @param anApplication the application instance
 */
CGPSTrackerDocument::CGPSTrackerDocument( CEikApplication& anApplication )
	: CAknDocument( anApplication )
	{
	}

/**
 * @brief Completes the second phase of Symbian object construction. 
 * Put initialization code that could leave here.  
 */ 
void CGPSTrackerDocument::ConstructL()
	{
	}
	
/**
 * Symbian OS two-phase constructor.
 *
 * Creates an instance of CGPSTrackerDocument, constructs it, and
 * returns it.
 *
 * @param aApp the application instance
 * @return the new CGPSTrackerDocument
 */
CGPSTrackerDocument* CGPSTrackerDocument::NewL( CEikApplication& aApp )
	{
	CGPSTrackerDocument* self = new ( ELeave ) CGPSTrackerDocument( aApp );
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	return self;
	}

/**
 * @brief Creates the application UI object for this document.
 * @return the new instance
 */	
CEikAppUi* CGPSTrackerDocument::CreateAppUiL()
	{
	return new ( ELeave ) CGPSTrackerAppUi;
	}
				
