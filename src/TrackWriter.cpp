/*
 ============================================================================
 Name		: TrackWriter.cpp
 Author	  : artem78
 Version	 : 1.0
 Copyright   : 
 Description : Classes for writing track to the file
 ============================================================================
 */

#include "TrackWriter.h"
#include "LBSSatelliteExtended.h"

#include "Logger.h"

// CTrackWriterBase

CTrackWriterBase::CTrackWriterBase(RFile &aFile) :
	iFile(aFile)
	{
	// No implementation required
	}


// CGPXTrackWriter

CGPXTrackWriter::CGPXTrackWriter(RFile &aFile, TBool anIsWriteExtendedData) :
	CTrackWriterBase(aFile),
	iIsWriteExtendedData(anIsWriteExtendedData)
	{
	// No implementation required
	}

CGPXTrackWriter::~CGPXTrackWriter()
	{
	TRAP_IGNORE(DeinitializeL());
	
	delete iXml;
	}

void CGPXTrackWriter::DeinitializeL()
	{
	CloseSegmentL();

	iXml->CloseTagL(); // </trk>
	iXml->CloseTagL(); // </gpx>
	}

CGPXTrackWriter* CGPXTrackWriter::NewLC(RFile &aFile, TBool anIsWriteExtendedData,
		const TDesC &aCreator)
	{
	CGPXTrackWriter* self = new (ELeave) CGPXTrackWriter(aFile, anIsWriteExtendedData);
	CleanupStack::PushL(self);
	self->ConstructL(aCreator);
	return self;
	}

CGPXTrackWriter* CGPXTrackWriter::NewL(RFile &aFile, TBool anIsWriteExtendedData,
		const TDesC &aCreator)
	{
	CGPXTrackWriter* self = CGPXTrackWriter::NewLC(aFile, anIsWriteExtendedData, aCreator);
	CleanupStack::Pop(); // self;
	return self;
	}

void CGPXTrackWriter::ConstructL(const TDesC &aCreator)
	{
	// Set general format for numbers
	iGeneralRealFormat = TRealFormat();
	iGeneralRealFormat.iType = KRealFormatFixed;
	iGeneralRealFormat.iPoint = '.';
	iGeneralRealFormat.iPlaces = 6;
	iGeneralRealFormat.iTriLen = 0;
	iGeneralRealFormat.iWidth = KDefaultRealWidth;
	
	// Create XML object
	iXml = CSimpleXMLWriter::NewL(iFile, TXMLVersion(1, 0), ETrue);
	
	// Write heading data to file
	_LIT(KGpx,			"gpx");
	_LIT(KXmlns,		"xmlns");
	_LIT(KXmlnsValue,	"http://www.topografix.com/GPX/1/1");
	_LIT(KVersion,		"version");
	_LIT(KVersionValue,	"1.1");
	_LIT(KCreator,		"creator");
	_LIT(KXmlnsXsi,		"xmlns:xsi");
	_LIT(KXmlnsXsiValue,	"http://www.w3.org/2001/XMLSchema-instance");
	_LIT(KXsiSchemaLocation,		"xsi:schemaLocation");
	_LIT(KXsiSchemaLocationValue,	"http://www.topografix.com/GPX/1/1 "
							"http://www.topografix.com/GPX/1/1/gpx.xsd");
	_LIT(KTrk, "trk");
	
	iXml->OpenTagL(KGpx);
	iXml->AddAttributeL(KXmlns, KXmlnsValue);
	iXml->AddAttributeL(KVersion, KVersionValue);
	iXml->AddAttributeL(KCreator, aCreator);
	iXml->AddAttributeL(KXmlnsXsi, KXmlnsXsiValue);
	iXml->AddAttributeL(KXsiSchemaLocation, KXsiSchemaLocationValue);
	iXml->OpenTagL(KTrk);
	}

void CGPXTrackWriter::AddPointL(const TPositionInfo* aPosInfo)
	{
	LOG(_L8("Start add point to gpx"));
	TPosition pos;
	aPosInfo->GetPosition(pos);
	
	// Get point`s date and time
	TBuf<30> timeBuff;
	_LIT(KTimeFormatISO8601, "%F%Y-%M-%DT%H:%T:%S.%*C3");
	pos.Time().FormatL(timeBuff, KTimeFormatISO8601);
	
	OpenSegmentL();
	
	// Text constants
	_LIT(KTrkpt,	"trkpt");
	_LIT(KLat,		"lat");
	_LIT(KLon,		"lon");
	_LIT(KEle,		"ele");
	_LIT(KTime,		"time");
	_LIT(KCourse,	"course");
	_LIT(KSpeed,	"speed");
	_LIT(KSat,		"sat");
	_LIT(KHDoP,		"hdop");
	_LIT(KVDoP,		"vdop");
	_LIT(KPDoP,		"pdop");
	
	LOG(_L8("Start write tag"));
	
	// Write base position information
	iXml->OpenTagL(KTrkpt);
	iXml->AddAttributeL(KLat, pos.Latitude(), iGeneralRealFormat);
	iXml->AddAttributeL(KLon, pos.Longitude(), iGeneralRealFormat);

	if (!Math::IsNaN(pos.Altitude()))
		{
		// Sometimes Altitude() may be NAN. Seems when real elevation about 0m.
		
		iXml->OpenTagL(KEle);
		iXml->AddTextL(pos.Altitude(), iGeneralRealFormat);
		iXml->CloseTagL(); // </ele>
		}
	
	iXml->OpenTagL(KTime);
	iXml->AddTextL(timeBuff);
	iXml->CloseTagL(); // </time>
	
	
	// Write extended position information
	if (iIsWriteExtendedData)
		{
		// Process course info
		if (aPosInfo->PositionClassType() & EPositionCourseInfoClass)
			{
			const TPositionCourseInfo* courseInfo =
					static_cast<const TPositionCourseInfo*>(aPosInfo);
			TCourse course;
			courseInfo->GetCourse(course);
			
			// Course
			if (!Math::IsNaN(course.Heading()))
				{
				iXml->OpenTagL(KCourse);
				iXml->AddTextL(course.Heading(), iGeneralRealFormat);
				iXml->CloseTagL(); // </course>
				}
			
			// Speed
			if (!Math::IsNaN(course.Speed()))
				{
				iXml->OpenTagL(KSpeed);
				iXml->AddTextL(course.Speed(), iGeneralRealFormat);
				iXml->CloseTagL(); // </speed>
				}
			}
		
		// Process satellite info
		if (aPosInfo->PositionClassType() & EPositionSatelliteInfoClass)
			{
			const TPositionSatelliteInfoExtended* satelliteInfo =
					static_cast<const TPositionSatelliteInfoExtended*>(aPosInfo);
			
			// Satellites count
			iXml->OpenTagL(KSat);
			iXml->AddTextL(satelliteInfo->NumSatellitesUsed());
			iXml->CloseTagL(); // </sat>
			
			// HDOP
			if (!Math::IsNaN(satelliteInfo->HorizontalDoP()))
				{
				iXml->OpenTagL(KHDoP);
				iXml->AddTextL(satelliteInfo->HorizontalDoP(), iGeneralRealFormat);
				iXml->CloseTagL(); // </hdop>
				}
			
			// VDOP
			if (!Math::IsNaN(satelliteInfo->VerticalDoP()))
				{
				iXml->OpenTagL(KVDoP);
				iXml->AddTextL(satelliteInfo->VerticalDoP(), iGeneralRealFormat);
				iXml->CloseTagL(); // </vdop>
				}
			
			// PDOP
			if (!Math::IsNaN(satelliteInfo->PositionDoP()))
				{
				iXml->OpenTagL(KPDoP);
				iXml->AddTextL(satelliteInfo->PositionDoP(), iGeneralRealFormat);
				iXml->CloseTagL(); // </pdop>
				}
			}
		}
	
	iXml->CloseTagL(); // </trkpt>
	// ToDo: Flush xml`s file buffer to disk after each track point writed

	LOG(_L8("End add point to gpx"));
	}

void CGPXTrackWriter::StartNewSegmentL()
	{
	CloseSegmentL();
	}

void CGPXTrackWriter::OpenSegmentL()
	{
	if (!iIsSegmentOpened)
		{
		_LIT(KTrkseg, "trkseg");
		iXml->OpenTagL(KTrkseg);
		iIsSegmentOpened = ETrue;
		}
	}

void CGPXTrackWriter::CloseSegmentL()
	{
	if (iIsSegmentOpened)
		{
		iXml->CloseTagL(); // </trkseg>
		iIsSegmentOpened = EFalse;
		}
	}



// CNMEATrackWriter

CNMEATrackWriter::CNMEATrackWriter(RFile &aFile/*, TBool anIsWriteExtendedData*/) :
	CTrackWriterBase(aFile)/*,
	iIsWriteExtendedData(anIsWriteExtendedData)*/
	{

	}

CNMEATrackWriter::~CNMEATrackWriter()
	{

	}

CNMEATrackWriter* CNMEATrackWriter::NewLC(RFile &aFile/*, TBool anIsWriteExtendedData*/)
	{
	CNMEATrackWriter* self = new (ELeave) CNMEATrackWriter(aFile/*, anIsWriteExtendedData*/);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CNMEATrackWriter* CNMEATrackWriter::NewL(RFile &aFile/*, TBool anIsWriteExtendedData*/)
	{
	CNMEATrackWriter* self = CNMEATrackWriter::NewLC(aFile/*, anIsWriteExtendedData*/);
	CleanupStack::Pop(); // self;
	return self;
	}

void CNMEATrackWriter::ConstructL()
	{
	// Set general format for numbers
	/*iGeneralRealFormat = TRealFormat();
	iGeneralRealFormat.iType = KRealFormatFixed;
	iGeneralRealFormat.iPoint = '.';
	iGeneralRealFormat.iPlaces = 6;
	iGeneralRealFormat.iTriLen = 0;
	iGeneralRealFormat.iWidth = KDefaultRealWidth;*/
	}

void CNMEATrackWriter::AddPointL(const TPositionInfo* aPosInfo)
	{
	// ToDo: Wrire other NMEA sentences (elevation, satellites, DoPs, etc...)
	
	LOG(_L8("Start writing point to NMEA"));
	TPosition pos;
	aPosInfo->GetPosition(pos);
	
	
	// Constants
	_LIT8(KRMCSentenceFmt, "GPRMC,%S,A,%S,%S,%.1f,%.1f,%S,,,");
	TReal32 KMetersPerSecondToKnots = 1.9438444924406046;
	
	
	// Process general position information
	TBuf8<10> timeBuff;
	TimeToDes8L(pos.Time(), timeBuff);
	
	TBuf8<16> latBuff;
	LatitudeToDes8(pos.Latitude(), latBuff);
	
	TBuf8<16> lonBuff;
	LongitudeToDes8(pos.Longitude(), lonBuff);
	
	TBuf8<6> dateBuff;
	DateToDes8L(pos.Time(), dateBuff);
	
	
	// Process extended position information
	TReal32 speed = /*KNaN*/ 0.0 / 0.0;
	TReal32 heading = /*KNaN*/ 0.0 / 0.0;
	
	if (/*iIsWriteExtendedData*/ 1)
		{
		// Process course info
		if (aPosInfo->PositionClassType() & EPositionCourseInfoClass)
			{
			const TPositionCourseInfo* courseInfo =
					static_cast<const TPositionCourseInfo*>(aPosInfo);
			TCourse course;
			courseInfo->GetCourse(course);
			
			// Course
			if (!Math::IsNaN(course.Heading()))
				{
				heading = course.Heading(); 
				}
			
			// Speed
			if (!Math::IsNaN(course.Speed()))
				{
				speed = course.Speed() * KMetersPerSecondToKnots;
				}
			}
		}
	
	TBuf8<KMaxNMEALineLength> buff;
	buff.Format(KRMCSentenceFmt, &timeBuff, &latBuff, &lonBuff, speed, heading, &dateBuff);
	
	// Write to the file
	WriteSentence(buff);

	LOG(_L8("End writing point to NMEA"));
	}

void CNMEATrackWriter::TimeToDes8L(const TTime &aTime, TDes8 &aDes)
	{
	TBuf<10> buff;
	_LIT(KTimeFmt, "%H%T%S.%*C3");
	aTime.FormatL(buff, KTimeFmt);
	aDes.Copy(buff);
	}

void CNMEATrackWriter::DateToDes8L(const TTime &aTime, TDes8 &aDes)
	{
	TBuf<6 + 2> buff;
	_LIT(KTimeFmt, "%F%D%M%Y");
	aTime.FormatL(buff, KTimeFmt);
	buff.Delete(4, 2); // Delete first two digits from year
	aDes.Copy(buff);
	}

void CNMEATrackWriter::DegreesToDes8(const TReal64 &aDegs, TInt aDigits,
		TChar aPositiveDirection, TChar aNegativeDirection, TDes8 &aDes)
	{
	const TInt KSecondsInMinute = 60;
	TInt degrees = Abs(static_cast<TInt>(aDegs));
	TReal minutes = Abs(aDegs);
	Math::Frac(minutes, minutes); // ToDo: check error code
	minutes *= KSecondsInMinute;
	TChar directionChar = aDegs > 0 ? aPositiveDirection : aNegativeDirection;
	/*TRealFormat realFmt;
	realFmt.iType = KRealFormatFixed;
	realFmt.iPoint = '.';
	realFmt.iPlaces = 4;
	realFmt.iTriLen = 0;
	realFmt.iWidth = KDefaultRealWidth;*/
	aDes.Zero();
	aDes.AppendNumFixedWidth(degrees, EDecimal, aDigits);
	if (minutes < 10)
		{ // Add leading zero
		aDes.Append('0');
		}
	TInt r = aDes.AppendNum(minutes, /*realFmt*/ TRealFormat(10, 4)); // ToDo: check error code
	aDes.Append(',');
	aDes.Append(directionChar);
	}

	
void CNMEATrackWriter::LatitudeToDes8(const TReal64 &aLat, TDes8 &aDes)
	{
	DegreesToDes8(aLat, 2, 'N', 'S', aDes);
	}
	
void CNMEATrackWriter::LongitudeToDes8(const TReal64 &aLon, TDes8 &aDes)
	{
	DegreesToDes8(aLon, 3, 'E', 'W', aDes);
	}

TNMEAChecksum CNMEATrackWriter::Checksum(const TDesC8 &aDes)
	{
	TUint8 checksum = 0;
	for (TInt i = 0; i < aDes.Length(); i++)
		{
		checksum ^= aDes[i];
		}
	
	TNMEAChecksum res;
	res.NumFixedWidth(checksum, EHex, 2);
	res.UpperCase();
	
	return res;
	}

void CNMEATrackWriter::WriteSentence(const TDesC8 &aSentence)
	{
	TNMEAChecksum checksum = Checksum(aSentence);
	
	TBuf8<KMaxNMEALineLength> buff;
	buff.Append('$');
	buff.Append(aSentence);
	buff.Append('*');
	buff.Append(checksum);
	buff.Append('\r');
	buff.Append('\n');
	
	iFile.Write(buff);
	}
