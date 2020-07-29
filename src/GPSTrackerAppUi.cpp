/*
========================================================================
 Name        : GPSTrackerAppUi.cpp
 Author      : artem78
 Copyright   : General Public License v3.0
 Description : 
========================================================================
*/
// [[[ begin generated region: do not modify [Generated System Includes]
#include <eikmenub.h>
#include <akncontext.h>
#include <akntitle.h>
#include <GPSTracker.rsg>
// ]]] end generated region [Generated System Includes]

// [[[ begin generated region: do not modify [Generated User Includes]
#include "GPSTrackerAppUi.h"
#include "GPSTracker.hrh"
#include "TrackerInfoListBoxView.h"
// ]]] end generated region [Generated User Includes]

#include <lbspositioninfo.h>
#include "LBSSatelliteExtended.h"

//  Constants

_LIT(KProgramName, "GPS Tracker");
_LIT(KProgramVersion, "0.0.0"); // ToDo: Change this
_LIT(KTracksDir, "\\data\\GPSTracker\\tracks\\");
_LIT(KLogsDir, "\\data\\GPSTracker\\logs\\");
_LIT(KTimeFormatForFileName, "%F%Y%M%D_%H%T%S");
const TChar KSpace = TChar(0x20);

// [[[ begin generated region: do not modify [Generated Constants]
// ]]] end generated region [Generated Constants]

/**
 * Construct the CGPSTrackerAppUi instance
 */ 
CGPSTrackerAppUi::CGPSTrackerAppUi()
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	// ]]] end generated region [Generated Contents]
	
	}

/** 
 * The appui's destructor removes the container from the control
 * stack and destroys it.
 */
CGPSTrackerAppUi::~CGPSTrackerAppUi()
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	// ]]] end generated region [Generated Contents]
	
	delete iPosRequestor;
	delete iTrackWriter;
	
	// Remove gpx file without points
	TFileName gpxFileName;
	TInt res = iTrackFile.FullName(gpxFileName);
	iTrackFile.Close();
	if (res == KErrNone)
		{
		if (iTotalPointsCount == 0)
			CCoeEnv::Static()->FsSession().Delete(gpxFileName);
		}
	
#if LOGGING_ENABLED
	LOG(_L8("Logging ended"));
	delete iLogger;
	iLogFile.Close();
#endif
	}

// [[[ begin generated function: do not modify
void CGPSTrackerAppUi::InitializeContainersL()
	{
	iTrackerInfoListBoxView = CTrackerInfoListBoxView::NewL();
	AddViewL( iTrackerInfoListBoxView );
	SetDefaultViewL( *iTrackerInfoListBoxView );
	}
// ]]] end generated function

/**
 * Handle a command for this appui (override)
 * @param aCommand command id to be handled
 */
void CGPSTrackerAppUi::HandleCommandL( TInt aCommand )
	{
	// [[[ begin generated region: do not modify [Generated Code]
	TBool commandHandled = EFalse;
	switch ( aCommand )
		{ // code to dispatch to the AppUi's menu and CBA commands is generated here
		default:
			break;
		}
	
		
	if ( !commandHandled ) 
		{
		if ( aCommand == EAknSoftkeyExit || aCommand == EEikCmdExit )
			{
			Exit();
			}
		}
	// ]]] end generated region [Generated Code]
	
	}

/** 
 * Override of the HandleResourceChangeL virtual function
 */
void CGPSTrackerAppUi::HandleResourceChangeL( TInt aType )
	{
	CAknViewAppUi::HandleResourceChangeL( aType );
	// [[[ begin generated region: do not modify [Generated Code]
	// ]]] end generated region [Generated Code]
	
	}
				
/** 
 * Override of the HandleKeyEventL virtual function
 * @return EKeyWasConsumed if event was handled, EKeyWasNotConsumed if not
 * @param aKeyEvent 
 * @param aType 
 */
TKeyResponse CGPSTrackerAppUi::HandleKeyEventL(
		const TKeyEvent& aKeyEvent,
		TEventCode aType )
	{
	// The inherited HandleKeyEventL is private and cannot be called
	// [[[ begin generated region: do not modify [Generated Contents]
	// ]]] end generated region [Generated Contents]
	
	return EKeyWasNotConsumed;
	}

/** 
 * Override of the HandleViewDeactivation virtual function
 *
 * @param aViewIdToBeDeactivated 
 * @param aNewlyActivatedViewId 
 */
void CGPSTrackerAppUi::HandleViewDeactivation( 
		const TVwsViewId& aViewIdToBeDeactivated, 
		const TVwsViewId& aNewlyActivatedViewId )
	{
	CAknViewAppUi::HandleViewDeactivation( 
			aViewIdToBeDeactivated, 
			aNewlyActivatedViewId );
	// [[[ begin generated region: do not modify [Generated Contents]
	// ]]] end generated region [Generated Contents]
	
	}

/**
 * @brief Completes the second phase of Symbian object construction. 
 * Put initialization code that could leave here. 
 */ 
void CGPSTrackerAppUi::ConstructL()
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	
	BaseConstructL( EAknEnableSkin ); 
	InitializeContainersL();
	// ]]] end generated region [Generated Contents]
	
	TFileName programDataDir;
	/*Get*/ProgramDataDir(programDataDir);
	User::LeaveIfError(MakeDir(programDataDir));
	User::LeaveIfError(CCoeEnv::Static()->FsSession().SetSessionPath(programDataDir)); // ToDo: Is this operation safe?
#if LOGGING_ENABLED
	InitializeLoggingL();
#endif
	InitializeTrackL();
	
	iPosRequestor = CDynamicPositionRequestor::NewL(this, KProgramName);
	iPosRequestor->Start();
	
	}



TInt CGPSTrackerAppUi::MakeDir(const TDesC &aDir)
	{
	TInt res = CEikonEnv::Static()->FsSession().MkDirAll(aDir);
	if (res == KErrNone || res == KErrAlreadyExists)
		return KErrNone;
	return res;
	}

TInt CGPSTrackerAppUi::CurrentDateTime(TDes &aDes, const TDesC &aFormat)
	{
	TTime now;
	now.HomeTime();
	TRAPD(res, now.FormatL(aDes, aFormat));
	return res;
	}

#if LOGGING_ENABLED
void CGPSTrackerAppUi::InitializeLoggingL()
	{
	// Create dir
	User::LeaveIfError(MakeDir(KLogsDir));
	
	// Configure logging file
	TFileName logFileName;
	logFileName.Append(KLogsDir);
#ifndef __WINS__
	TBuf<20> timeBuff;
	CurrentDateTime(timeBuff, KTimeFormatForFileName);
	
	_LIT(KLogFilePrefix, "gps_tracker_gui_log_");
	logFileName.Append(KLogFilePrefix);
	logFileName.Append(timeBuff);
#else
	_LIT(KLogFilename, "gui_log");
	logFileName.Append(KLogFilename);
#endif
	_LIT(KLogFileExtension, ".txt");
	logFileName.Append(KLogFileExtension);
	User::LeaveIfError(iLogFile.Replace(CEikonEnv::Static()->FsSession(), logFileName, EFileWrite));
	
	// Enable logging autoflush only for emulator
#ifndef __WINS__
	TBool logAutoFlush = EFalse;
#else
	TBool logAutoFlush = ETrue;
#endif
	iLogger = CLogger::NewL(iLogFile, CLogger::ELevelAll, CLogger::EASCII/*EUtf8*/, logAutoFlush);
	LoggerStatic::SetLogger(iLogger);
	LOG(_L8("Logging started"));
	}
#endif

void CGPSTrackerAppUi::InitializeTrackL()
	{
	// Create dir
	User::LeaveIfError(MakeDir(KTracksDir));
	
	TBuf<20> timeBuff;
	CurrentDateTime(timeBuff, KTimeFormatForFileName);
	
	// Create gpx file for writing track
	TFileName gpxFileName;
	gpxFileName.Append(KTracksDir);
	_LIT(KTrackFilePrefix, "track_");
	gpxFileName.Append(KTrackFilePrefix);
	gpxFileName.Append(timeBuff);
	_LIT(KTrackFileExtension, ".gpx");
	gpxFileName.Append(KTrackFileExtension);
	User::LeaveIfError(iTrackFile.Create(CEikonEnv::Static()->FsSession(), gpxFileName, EFileWrite));
	
	TBuf<100> programFullName;
	programFullName.Append(KProgramName);
	programFullName.Append(KSpace);
	programFullName.Append(KProgramVersion);
	iTrackWriter = CGPXTrackWriter::NewL(iTrackFile, ETrue, programFullName);
	}

void CGPSTrackerAppUi::ShowDataL()
	{
	LOG(_L8("Begin show data"));

	
	// Constants
	const TReal KNaN = 0.0 / 0.0; // NaN
	const TReal KSpeedRatio  = 3.6; // m/s to km/h
	const TReal KMetersInKilometer = 1000.0;

	
	// Variables
	TReal lat = KNaN, lon = KNaN, alt = KNaN, speed = KNaN;
	TInt satUsed = 0, satTotal = 0;
	TTimeIntervalMicroSeconds posRefreshInterval = 0;
	
	
	const TPositionInfo* posInfo = iPosRequestor->LastKnownPositionInfo(); 
	TPosition pos;
	posInfo->GetPosition(pos);
	


	// Latitude, Longitude, Altitude
	if (iPosRequestor->IsPositionRecieved())
		{
		lat = pos.Latitude();
		lon = pos.Longitude();
		alt = pos.Altitude();
		}

		
	// Course info
	if (posInfo->PositionClassType() & EPositionCourseInfoClass)
		{
		const TPositionCourseInfo* courseInfo = static_cast<const TPositionCourseInfo*>(posInfo);
		TCourse course;
		courseInfo->GetCourse(course);
		
		// Speed
		if (iPosRequestor->IsPositionRecieved() && !Math::IsNaN(course.Speed()))
			speed = course.Speed() * KSpeedRatio;
		
		}
		
	
	// Sattelite info
	if (posInfo->PositionClassType() & EPositionSatelliteInfoClass)
		{
		const TPositionSatelliteInfoExtended* satelliteInfo = static_cast<const TPositionSatelliteInfoExtended*>(posInfo);
		
		// Satellites count
		if (iPosRequestor->IsRunning())
			{
			satUsed = satelliteInfo->NumSatellitesUsed();
			satTotal = satelliteInfo->NumSatellitesInView();
			}
		}

		
	// Position update interval
	if (iPosRequestor->IsRunning())
		posRefreshInterval = iPosRequestor->UpdateInterval();


	// Write information to listbox	
	iTrackerInfoListBoxView->SetDataL(lat, lon, alt, speed, satUsed, satTotal,
			iTotalPointsCount, posRefreshInterval);

		
	LOG(_L8("End show data"));
	}

void CGPSTrackerAppUi::/*Get*/ProgramDataDir(TDes &aDir)
	{
	_LIT(KProgramDataDirWithoutDrive, "\\data\\GPSTracker\\");
	
	aDir.Zero();
#ifdef __WINS__
	_LIT(KDriveC, "c:");
	aDir.Append(KDriveC);
#else
	// Get drive from current process (path to exe)
	RProcess proc;
	TFileName procPath = proc.FileName();
	TParse parser;
	parser.Set(procPath, NULL, NULL);
	aDir.Append(parser.Drive());
#endif
	aDir.Append(KProgramDataDirWithoutDrive);
	}

void CGPSTrackerAppUi::OnPositionRestored()
	{
	LOG(_L8("Position recieved"));
	iIsAfterConnectionRestored = ETrue;
	}

void CGPSTrackerAppUi::OnPositionLost()
	{
	LOG(_L8("Position lost"));
	TRAP_IGNORE(ShowDataL());
	
	TRAPD(r, iTrackWriter->StartNewSegmentL());
	if (r != KErrNone)
		{
		LOG(_L8("Error start new segment in gpx with code %d"), r);
		ShowError(_L("Error start new segment in gpx"), r);
		Exit();
		}
	}

void CGPSTrackerAppUi::OnPositionUpdated()
	{
	//////
	TTime startTime;
	startTime.HomeTime();
	//////
	
	const TPositionInfo* posInfo = iPosRequestor->LastKnownPositionInfo();
	TPosition pos;
	posInfo->GetPosition(pos);
	
	const TPositionInfo* prevPosInfo = iPosRequestor->PrevLastKnownPositionInfo();
	TPosition prevPos;
	prevPosInfo->GetPosition(prevPos);
	
	// Increment counters and calculate speed
	iTotalPointsCount++;
	TReal32 distance = 0;
	LOG(_L8("iIsAfterConnectionRestored=%d"), iIsAfterConnectionRestored);
	if (!iIsAfterConnectionRestored) // Skip distance increase when connection lost
		{
		pos.Distance(prevPos, distance);
		iTotalDistance += distance;
		}
	LOG(_L8("iTotalDistance=%f"), iTotalDistance);
	
	// Write position to file
	LOG(_L8("Before add point"));
	TRAPD(r, iTrackWriter->AddPointL(posInfo));
	LOG(_L8("After add point"));
	if (r != KErrNone)
		{
		LOG(_L8("Error write position to the file with code %d"), r);
		ShowError(_L("Error write position to the file"), r);
		Exit();
		}
	
	// Write position to the screen	
	TRAP_IGNORE(ShowDataL());
	
	iIsAfterConnectionRestored = EFalse;
	
	//////
	TTime endTime;
	endTime.HomeTime();
	
	TTimeIntervalMicroSeconds diff = endTime.MicroSecondsFrom(startTime);
	LOG(_L8("OnPositionUpdated execution time: %d ms"), diff.Int64() / 1000);
	//////
	}

void CGPSTrackerAppUi::OnPositionPartialUpdated()
	{
	TRAP_IGNORE(ShowDataL());
	}

void CGPSTrackerAppUi::OnPositionError(TInt aErrCode)
	{
	ShowError(_L("OnPositionError"), aErrCode);
	Exit();
	}

void CGPSTrackerAppUi::OnPauseTracking()
	{
	TRAP_IGNORE(ShowDataL());
	
	//TRAP(r, iTrackWriter->StartNewSegmentL());
	}

void CGPSTrackerAppUi::OnResumeTracking()
	{
	TRAP_IGNORE(ShowDataL());
	}

void CGPSTrackerAppUi::ShowError(const TDesC aMsg, TInt anErrCode)
	{
	_LIT(KErrCodeText, " (Error code: %d)");
	
	if (anErrCode == KErrNone)
		{
		iEikonEnv->AlertWin(aMsg);
		}
	else
		{
		RBuf buff;
		if (buff.Create(aMsg.Length() + 30) == KErrNone)
			{
			buff.Copy(aMsg);
			buff.AppendFormat(KErrCodeText, anErrCode);
			iEikonEnv->AlertWin(buff);
			buff.Close();
			}
		}
	}

void CGPSTrackerAppUi::StartTracking()
	{
	if (!iPosRequestor->IsRunning())
		{
		iPosRequestor->Start();
		OnResumeTracking();
		}
	}

void CGPSTrackerAppUi::StopTracking()
	{
	if (iPosRequestor->IsRunning())
		{
		iPosRequestor->Cancel();
		OnPauseTracking();
		}
	}

