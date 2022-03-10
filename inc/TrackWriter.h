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
	void DeinitializeL(); // Leaving methods for destructor for using in trap

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



const TInt KNMEAChecksumLength = 2;
typedef TBuf8<KNMEAChecksumLength> TNMEAChecksum;
const TInt KMaxNMEALineLength = 82;

/*
 * Class for writing track in NMEA 0183 format
 */
class CNMEATrackWriter : public CTrackWriterBase
	{
	
	// Constructors and destructor
public:
	~CNMEATrackWriter();
	static CNMEATrackWriter* NewL(RFile &aFile/*, TBool anIsWriteExtendedData = EFalse*/);
	static CNMEATrackWriter* NewLC(RFile &aFile/*, TBool anIsWriteExtendedData = EFalse*/);

private:
	CNMEATrackWriter(RFile &aFile/*, TBool anIsWriteExtendedData*/);
	void ConstructL();
	
	
	// From CTrackWriterBase
public:
	void AddPointL(const TPositionInfo* aPosInfo);
	
	
	// Custom properties and methods
private:
	//TRealFormat iGeneralRealFormat;
	//TBool iIsWriteExtendedData;

	static void TimeToDes8L(const TTime &aTime, TDes8 &aDes);
	static void DateToDes8L(const TTime &aTime, TDes8 &aDes);
	static void DegreesToDes8(const TReal64 &aDegs, TInt aDigits,
			TChar aPositiveDirection, TChar aNegativeDirection, TDes8 &aDes);
	static void LatitudeToDes8(const TReal64 &aLat, TDes8 &aDes);
	static void LongitudeToDes8(const TReal64 &aLon, TDes8 &aDes);
	static TNMEAChecksum Checksum(const TDesC8 &aDes);
	void WriteSentence(const TDesC8 &aSentence);
	
	};

#endif // TRACKWRITER_H
