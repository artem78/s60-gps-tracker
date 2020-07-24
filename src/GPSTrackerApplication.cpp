/*
========================================================================
 Name        : GPSTrackerApplication.cpp
 Author      : artem78
 Copyright   : General Public License v3.0
 Description : 
========================================================================
*/
// [[[ begin generated region: do not modify [Generated System Includes]
// ]]] end generated region [Generated System Includes]

// [[[ begin generated region: do not modify [Generated Includes]
#include "GPSTrackerApplication.h"
#include "GPSTrackerDocument.h"
#ifdef EKA2
#include <eikstart.h>
#endif
// ]]] end generated region [Generated Includes]

/**
 * @brief Returns the application's UID (override from CApaApplication::AppDllUid())
 * @return UID for this application (KUidGPSTrackerApplication)
 */
TUid CGPSTrackerApplication::AppDllUid() const
	{
	return KUidGPSTrackerApplication;
	}

/**
 * @brief Creates the application's document (override from CApaApplication::CreateDocumentL())
 * @return Pointer to the created document object (CGPSTrackerDocument)
 */
CApaDocument* CGPSTrackerApplication::CreateDocumentL()
	{
	return CGPSTrackerDocument::NewL( *this );
	}

#ifdef EKA2

/**
 *	@brief Called by the application framework to construct the application object
 *  @return The application (CGPSTrackerApplication)
 */	
LOCAL_C CApaApplication* NewApplication()
	{
	return new CGPSTrackerApplication;
	}

/**
* @brief This standard export is the entry point for all Series 60 applications
* @return error code
 */	
GLDEF_C TInt E32Main()
	{
	return EikStart::RunApplication( NewApplication );
	}
	
#else 	// Series 60 2.x main DLL program code

/**
* @brief This standard export constructs the application object.
* @return The application (CGPSTrackerApplication)
*/
EXPORT_C CApaApplication* NewApplication()
	{
	return new CGPSTrackerApplication;
	}

/**
* @brief This standard export is the entry point for all Series 60 applications
* @return error code
*/
GLDEF_C TInt E32Dll(TDllReason /*reason*/)
	{
	return KErrNone;
	}

#endif // EKA2
