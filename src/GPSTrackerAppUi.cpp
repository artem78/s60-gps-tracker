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
#include "TrackerInfoListBox.hrh"
#include "TrackListBox.hrh"
#include "TrackerInfoListBoxView.h"
#include "TrackListBoxView.h"
// ]]] end generated region [Generated User Includes]

#include <lbspositioninfo.h>
#include "LBSSatelliteExtended.h"
#include <bautils.h>
#include <stringloader.h>
#include <aknmessagequerydialog.h>

//  Constants

_LIT(KProgramName, "GPS Tracker");
_LIT(KProgramVersion, "1.2.0");
_LIT(KTracksDirRel, "tracks\\");
_LIT(KLogsDirRel, "logs\\");
_LIT(KTimeFormatForFileName, "%F%Y%M%D_%H%T%S");
const TChar KSpace = TChar(0x20);

// [[[ begin generated region: do not modify [Generated Constants]
// ]]] end generated region [Generated Constants]

/**
 * Construct the CGPSTrackerAppUi instance
 */ 
CGPSTrackerAppUi::CGPSTrackerAppUi() :
		iIsGoingToExit(EFalse)
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
	DEBUG(_L("AppUI destructor start"));
	
	iIsGoingToExit = ETrue;
	
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
	
	delete iAsyncFileMan;
	
#if LOGGING_ENABLED
	LOG(_L8("Logging ended"));
	delete iLogger;
	iLogFile.Close();
#endif
	
	
	// [[[ begin generated region: do not modify [Generated Contents]
	// ]]] end generated region [Generated Contents]
	
	DEBUG(_L("AppUi destructor end"));
	}

// [[[ begin generated function: do not modify
void CGPSTrackerAppUi::InitializeContainersL()
	{
	iTrackerInfoListBoxView = CTrackerInfoListBoxView::NewL();
	AddViewL( iTrackerInfoListBoxView );
	SetDefaultViewL( *iTrackerInfoListBoxView );
	iTrackListBoxView = CTrackListBoxView::NewL();
	AddViewL( iTrackListBoxView );
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
	BaflUtils::EnsurePathExistsL(CCoeEnv::Static()->FsSession(), programDataDir);
	User::LeaveIfError(CCoeEnv::Static()->FsSession().SetSessionPath(programDataDir)); // ToDo: Is this operation safe?
	
	iAsyncFileMan = CAsyncFileMan::NewL(CCoeEnv::Static()->FsSession(), this);
	
#if LOGGING_ENABLED
	InitializeLoggingL();
#endif
	InitializeTrackL();
	
	iPosRequestor = CDynamicPositionRequestor::NewL(this, KProgramName);
	iPosRequestor->Start();
	
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
	TFileName logDir;
	LogDir(logDir);
	BaflUtils::EnsurePathExistsL(CCoeEnv::Static()->FsSession(), logDir);
	
	// Configure logging file
	TFileName logFileName;
	logFileName.Append(logDir);
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
	TFileName trackDir;
	TrackDir(trackDir);
	BaflUtils::EnsurePathExistsL(CCoeEnv::Static()->FsSession(), trackDir);
	
	TBuf<20> timeBuff;
	CurrentDateTime(timeBuff, KTimeFormatForFileName);
	
	// Create gpx file for writing track
	TFileName gpxFileName;
	gpxFileName.Append(trackDir);
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
	// There are some problems with controls when application exiting,
	// so skip interaction with UI in this case
	if (iIsGoingToExit)
		return;
	
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
		
	
	// Satellite info
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
	
	TRAPD(ret, iTrackWriter->StartNewSegmentL());
	if (ret != KErrNone)
		{
		LOG(_L8("Error start new segment in gpx with code %d"), ret);
		
		// Stop track recording
		StopTracking();

		// Display error message		
		HBufC* errMsg;
		TRAPD(r, errMsg = StringLoader::LoadL(R_APPLICATION_GPX_WRITE_ERROR_TEXT, iEikonEnv));
		if (r == KErrNone)
			{
			ShowError(*errMsg, ret);
			delete errMsg;
			}
		
		//Exit();
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
	TRAPD(ret, iTrackWriter->AddPointL(posInfo));
	LOG(_L8("After add point"));
	if (ret != KErrNone)
		{
		LOG(_L8("Error write position to the file with code %d"), ret);
		
		// Stop track recording
		StopTracking();

		// Display error message
		HBufC* errMsg;
		TRAPD(r, errMsg = StringLoader::LoadL(R_APPLICATION_GPX_WRITE_ERROR_TEXT, iEikonEnv));
		if (r == KErrNone)
			{
			ShowError(*errMsg, ret);
			delete errMsg;
			}
		
		//Exit();
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

//MFileManObserver::TControl CGPSTrackerAppUi::NotifyFileManStarted()
//	{
//	if (iCancelFManOperation)
//		{
//		DEBUG(_L("FMan operation cancelled"));
//		return MFileManObserver::EAbort;
//		}
//	else
//		{
//		DEBUG(_L("FMan started"));
//		return MFileManObserver::EContinue;
//		}
//	}

//MFileManObserver::TControl CGPSTrackerAppUi::OnFileManStarted()
//	{
//	return MFileManObserver::EContinue;
//	}

//MFileManObserver::TControl CGPSTrackerAppUi::NotifyFileManOperation()
//	{	
//	if (iCancelFManOperation)
//		{
//		DEBUG(_L("FMan operation cancelled"));
//		return MFileManObserver::EAbort;
//		}
//	else
//		{
//		DEBUG(_L("FMan operation"));
//		return MFileManObserver::EContinue;
//		}

//MFileManObserver::TControl CGPSTrackerAppUi::OnFileManOperation()
//	{
//	return MFileManObserver::EContinue;
//	}

	
//	
//	switch (iFileMan->CurrentAction())
//		{
//		case CFileMan::EDelete:
//			{
//			if (iFileMan->GetLastError() != KErrNone)
//				{
//				_LIT(KErrMsg, "Failed to delete file %S!");
//				RBuf msg;
//				msg.Create(KErrMsg().Length() + iFileMan->CurrentEntry().iName.Length());
//				msg.Format(KErrMsg, &iFileMan->CurrentEntry().iName);
//				ShowError(msg);
//				msg.Close();
//				}
//
//			//TRAP_IGNORE(iTrackListBoxView->HideDeletionDialogL());
//			return MFileManObserver::EContinue;
//			}
//			break;
//			
//			default:
//			return MFileManObserver::EContinue;
//		}
//	
//	return MFileManObserver::EContinue;
//	}

//MFileManObserver::TControl CGPSTrackerAppUi::NotifyFileManEnded()
//	{
//	iFManProcessedFiles++;
//	
//	if (iFManProcessedFiles == iFManTotalFiles)
//		NotifyFileManTotallyEnded();
//	
//	if (iCancelFManOperation)
//		{
//		DEBUG(_L("FMan operation cancelled"));
//		return MFileManObserver::EAbort;
//		}
//	else
//		{
//		DEBUG(_L("FMan ended"));
//		//iFManProcessedFiles++;
//		INFO(_L("File %S deleted"), &iFileMan->CurrentEntry().iName);
//		DEBUG(_L("Processed files: %d/%d"), iFManProcessedFiles, iFManTotalFiles);
//		return MFileManObserver::EContinue;
//		}
//	}

MFileManObserver::TControl CGPSTrackerAppUi::OnFileManEnded()
	{
	//	if (iFileMan->CurrentAction() == CFileMan::EDelete)
#ifdef __WINS__
		// Delay for testing wait window on emulator
		User::After(20*1000);
#endif
	
		INFO(_L("File deleted"));
	//
	
	return MFileManObserver::EContinue;
	}
	
//void CGPSTrackerAppUi::NotifyFileManTotallyEnded()
//	{
//	DEBUG(_L("FMan totally ended"));
//	
//	if (iFileMan->CurrentAction() == CFileMan::EDelete)
//		{
//		DEBUG(_L("Deletion ended"));
//		TRAP_IGNORE(iTrackListBoxView->HideDeletionDialogL());
//		UpdateTrackListL();
//		}
//	}

void CGPSTrackerAppUi::OnFileManFinished(TInt /*aStatus*/)
	{
	//	if (iFileMan->CurrentAction() == CFileMan::EDelete)
	//		{
			DEBUG(_L("Deletion ended"));
//			if (aStatus != KErrCancel)
//				TRAP_IGNORE(iTrackListBoxView->RemoveDeletionProgressDlgL());
			UpdateTrackListL();
	//		}
	}

void CGPSTrackerAppUi::ShowError(const TDesC &aMsg, TInt anErrCode)
	{
	// ToDo: Use error icon instead info
	
	if (anErrCode == KErrNone)
		{
		iEikonEnv->AlertWin(aMsg);
		}
	else
		{
		HBufC* errCodeText;
		TRAPD(r, errCodeText = StringLoader::LoadL(R_APPLICATION_ERROR_CODE_TEXT, anErrCode, iEikonEnv));
		if (r != KErrNone)
			return;
		
		RBuf buff;
		if (buff.Create(aMsg.Length() + 3 + errCodeText->Length()) == KErrNone)
			{
			buff.Copy(aMsg);
			buff.Append(' ');
			buff.Append('(');
			buff.Append(*errCodeText);
			buff.Append(')');
			iEikonEnv->AlertWin(buff);
			buff.Close();
			}
		
		delete errCodeText;		
		}
	}

void CGPSTrackerAppUi::ShowMsgL(const TDesC &aTitle, const TDesC &aMsg)
	{
	CAknMessageQueryDialog* dlg = new (ELeave) CAknMessageQueryDialog();
	dlg->PrepareLC(R_APPLICATION_MAIN_QUERY_DIALOG);
	dlg->QueryHeading()->SetTextL(aTitle);
	dlg->SetMessageTextL(aMsg);
	dlg->RunLD();
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

void CGPSTrackerAppUi::GetTracksArrayL(CDesCArray &aTracksArr)
	{
	aTracksArr.Reset();
	
	// Read list of all saved tracks
	TFileName path;
	TrackDir(path);
	_LIT(KFileMask, "*.gpx"); // Tracks extension
	path.Append(KFileMask);
	CDir* files(NULL);
	TInt r = CCoeEnv::Static()->FsSession().GetDir(path, KEntryAttNormal, ESortByDate, files);
	User::LeaveIfError(r);
	if (files != NULL)
		{
		CleanupStack::PushL(files);
		
		for (TInt i = 0; i < files->Count(); i++)
			{
			if ((*files)[i].IsDir())
				continue;
			
			aTracksArr.AppendL((*files)[i].iName);
			}
		
		CleanupStack::PopAndDestroy(files);
		}
	}

void CGPSTrackerAppUi::UpdateTrackListL()
	{
	CDesCArray* trackArr = new (ELeave) CDesCArraySeg(20);
	CleanupStack::PushL(trackArr);
	GetTracksArrayL(*trackArr);
	iTrackListBoxView->SetTrackArrayL(*trackArr);
	CleanupStack::PopAndDestroy(trackArr);
	}

void CGPSTrackerAppUi::ShowTrackListL()
	{
	ActivateViewL(iTrackListBoxView->ViewId());
	}

void CGPSTrackerAppUi::TrackDir(TDes &aDes)
	{
	ProgramDataDir(aDes);
	aDes.Append(KTracksDirRel);
	}

void CGPSTrackerAppUi::LogDir(TDes &aDes)
	{
	ProgramDataDir(aDes);
	aDes.Append(KLogsDirRel);
	}

void CGPSTrackerAppUi::DeleteAllTracks/*L*/()
	{
	TFileName path;
	TrackDir(path);
	_LIT(KTrackFileMask, "*.gpx");
	path.Append(KTrackFileMask);
	
	// Start delete files	
	TInt r = iAsyncFileMan->Delete(path, 0, ETrue);
	// ToDo: check r ...
	}

const TDesC& CGPSTrackerAppUi::ProgramName() const
	{
	return KProgramName;
	}

const TDesC& CGPSTrackerAppUi::ProgramVersion() const
	{
	return KProgramVersion;
	}
