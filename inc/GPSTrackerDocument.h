/*
========================================================================
 Name        : GPSTrackerDocument.h
 Author      : artem78
 Copyright   : General Public License v3.0
 Description : 
========================================================================
*/
#ifndef GPSTRACKERDOCUMENT_H
#define GPSTRACKERDOCUMENT_H

#include <akndoc.h>
		
class CEikAppUi;

/**
* @class	CGPSTrackerDocument GPSTrackerDocument.h
* @brief	A CAknDocument-derived class is required by the S60 application 
*           framework. It is responsible for creating the AppUi object. 
*/
class CGPSTrackerDocument : public CAknDocument
	{
public: 
	// constructor
	static CGPSTrackerDocument* NewL( CEikApplication& aApp );

private: 
	// constructors
	CGPSTrackerDocument( CEikApplication& aApp );
	void ConstructL();
	
public: 
	// from base class CEikDocument
	CEikAppUi* CreateAppUiL();
	};
#endif // GPSTRACKERDOCUMENT_H
