/*
 ============================================================================
 Name		: TrackWriter.h
 Author	  : artem78
 Version	 : 1.0
 Copyright   : 
 Description : Classes for writing track to the file
 ============================================================================
 */

#ifndef TRACKWRITER_H
#define TRACKWRITER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <lbs.h>
#include <f32file.h>
#include <XML.h>

// CLASS DECLARATION

/**
 *  Base class for track writers.
 * 
 */
class CTrackWriterBase : public CBase
	{
public:
	// Constructors and destructor

	CTrackWriterBase(RFile &aFile);
	
	// Custom properties and methods
public:
	virtual void AddPointL(const TPositionInfo* aPosInfo) = 0;
	
protected:
	RFile iFile;

	};


/** 
 *  Writes track to GPX 1.1 file
 */
class CGPXTrackWriter : public CTrackWriterBase
	{
public:
	// Constructors and destructor
	
	/**
	 * Destructor.
	 */
	~CGPXTrackWriter();

	/**
	 * Two-phased constructor.
	 */
	static CGPXTrackWriter* NewL(RFile &aFile, TBool anIsWriteExtendedData = EFalse,
			const TDesC &aCreator = KNullDesC);

	/**
	 * Two-phased constructor.
	 */
	static CGPXTrackWriter* NewLC(RFile &aFile, TBool anIsWriteExtendedData = EFalse,
			const TDesC &aCreator = KNullDesC);

private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CGPXTrackWriter(RFile &aFile, TBool anIsWriteExtendedData);

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL(const TDesC &aCreator);
	
	
	// Custom properties and methods
public:
	void AddPointL(const TPositionInfo* aPosInfo);
	void StartNewSegmentL();
	
private:
	TRealFormat iGeneralRealFormat;
	TBool iIsSegmentOpened;
	TBool iIsWriteExtendedData;
	CSimpleXMLWriter *iXml;
	
	void OpenSegmentL();
	void CloseSegmentL();

	};

#endif // TRACKWRITER_H
