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
