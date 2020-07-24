
#ifndef GPSTRACKER_PAN_H
#define GPSTRACKER_PAN_H

/** GPSTracker application panic codes */
enum TGPSTrackerPanics
	{
	EGPSTrackerUi = 1
	// add further panics here
	};

inline void Panic(TGPSTrackerPanics aReason)
	{
	_LIT(applicationName,"GPSTracker");
	User::Panic(applicationName, aReason);
	}

#endif // GPSTRACKER_PAN_H
