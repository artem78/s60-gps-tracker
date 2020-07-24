/*
========================================================================
 Name        : GPSTrackerApplication.h
 Author      : artem78
 Copyright   : General Public License v3.0
 Description : 
========================================================================
*/
#ifndef GPSTRACKERAPPLICATION_H
#define GPSTRACKERAPPLICATION_H

// [[[ begin generated region: do not modify [Generated Includes]
#include <aknapp.h>
// ]]] end generated region [Generated Includes]

// [[[ begin generated region: do not modify [Generated Constants]
const TUid KUidGPSTrackerApplication = { 0xE959FFAC };
// ]]] end generated region [Generated Constants]

/**
 *
 * @class	CGPSTrackerApplication GPSTrackerApplication.h
 * @brief	A CAknApplication-derived class is required by the S60 application 
 *          framework. It is subclassed to create the application's document 
 *          object.
 */
class CGPSTrackerApplication : public CAknApplication
	{
private:
	TUid AppDllUid() const;
	CApaDocument* CreateDocumentL();
	
	};
			
#endif // GPSTRACKERAPPLICATION_H		
