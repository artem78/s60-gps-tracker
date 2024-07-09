# GPS track recorder for Symbian OS based phones

This software allows you to record your trip using Nokia phone with GPS reciever.

![](images/tracking_info.png) ![](images/track_list_with_menu.png) ![](images/track_info.png)

From recorded track you can know travelled distance, average speed, elevation and many other information using [miscellaneous tools](#see-also). Also you can use tracks for edit maps like [OpenStreetMap](https://www.openstreetmap.org/) for adding new roads, trails and etc.

![](images/20210527_210321.png "View of recorded track in GPS Track Editor") ![](images/20210527_214913_2.png "View of recorded track in GPS Studio")

This application is GUI fork of my previous [console version](https://github.com/artem78/s60-gps-tracker-cli). Initially created for Symbian 9.2, probably can works with Symbian 9.1—9.4. Installer package is **unsigned**, so you need to sign it first or install on hacked phone. You can download latest stable version from [this link](https://github.com/artem78/s60-gps-tracker/releases/latest).

## Using
First go out into the open space. Positioning probably will not work or accuracy will be poor indors or near buildings. Run application. Track recording will start automatically when position from satellites will be recieved (often this takes some time up to several minutes). When it happened you will see your coordinates, speed, traveled distance, amount of recorded points and etc. At any time you can pause track recording (for example: when you stopped for a rest or entered the building). All tracks save in [GPX](https://en.wikipedia.org/wiki/GPS_Exchange_Format) format in `[C or E]:\data\GPSTracker\tracks` directory. Close program for stop track writing. After this you may transfer GPX-file(s) to your PC for future use.

## Track samples
* [Car driving](https://trackprofiler.com/track:1720528508189968369)

## Localization
* English
* Russian (Русский)
* Ukrainian (thanks to [baranovskiykonstantin](https://github.com/baranovskiykonstantin))

## Tasks
* List of tracks
   * [x] Show track info: file size, distance, points count, date, etc...
   * [ ] Send track file via sms/email/bluetooth
   * [x] Delete files
* File formats
   * [ ] KML
   * [ ] NMEA
* Settings
   * [ ] Use static or dinamic position requestor
   * [ ] Change position update interval
   
## See also
  - [Accuracy of GNSS data](https://wiki.openstreetmap.org/wiki/Accuracy_of_GNSS_data)

### Useful tools for view/edit GPS tracks
 - [GPS Track Editor](http://www.gpstrackeditor.com/)
 - https://maplorer.com/view_gpx.html
 - https://www.mygpsfiles.com/app/
 - https://gpxstudio.github.io/
