/*
 * roboBrain.h
 */

#ifndef ROBOBRAIN_H_
#define ROBOBRAIN_H_

#include "controller.h"
#include "waypoint.h"

class roboBrain: public Controller {
protected:
  enum nmeaParts {	///< constants to track where in the partitions array is the spot for each part of the nmea sentence
    timeSpot,		///< UTC time
    statusSpot,		///< Active/Void
    latSpot,		///< latitude
    nsSpot,			///< north or south of equator
    longSpot,		///< longitude
    ewSpot,			///< east or west of the prime meridian
    speedSpot, 		///< space for speed in knots
    headingSpot, 	///< heading
    dateSpot, 		///< current date in ddmmyyyy
    magSpot,		///< magnetic variation I DON'T KNOW WHAT THIS MEANS YET
    magewSpot,		///< magnetic variation east or west
    modeSpot,		///< mode of nmea sentence
    checksumSpot	///< checksum
  };

  double lat0 = 100;		///< latitude at time 0, initialized to 100 for navigateGPS to set, then compare with new GPS data
  double long0 = 200;		///< longitude at time 0, initialized to 200 for navigateGPS to set, then compare with new GPS data
  double latdd;		///< latitude at current time
  double longdd;		///< longitude at current time
  double heading;		///< Perceived heading
  double desiredHeading;	///< Heading needed for the robot to be on course
  double headingChange;	///< Heading change needed for the robot to be on course
  int nowpoint = 0;	///< Current waypoint for robot to navigate to
  waypoint waypoints[11];	///< Array of waypoints for the robot
  static const int wpcount;
  char nmeaReceived[256];	///< NMEA sentence received by robot
  int charsReceived;	///< Number of characters in NMEA sentence currently received
  double pps = -1;	///< epoch time of the last PPS in seconds, initialized negative to ensure it is not equal with simulator pps at startup
  bool sentenceStart;	///< begin status of the latest NMEA sentence
  int partCount;		///< number of partitions (commas and asterisk) detected in the current sentence
  int partitions[20];	///< locations of the partitions in the NMEA sentence
  void updateTime();
  int32_t oldWheelCount;
  waypoint odoDeltaPos;
  int16_t zDN,steerCmd;
  double yawRate;
  double epochTime;
  double dt;
  int offSet;
  static const int bufferDiscard = 300;
  static const int bufferMax = 1500;
  int ofBuffer[bufferMax];
  int bufferSpot;
  Log& logC;
  Log& logGps;
  void fillBuffer();
  void setOffSet();
  int servoCommand;
  int32_t wheelCount; ///< count of sector changes taken last time by the odometer
  uint32_t timeStamp; ///< epoch time(CURRENTLY IN MILLISECONDS) of last time readOdometer() was used
  uint32_t dtOdometer;	///< time between most recent call of readOdometer() and the call of readOdometer() previous to that.
  waypoint pos;		///< perceived position
public:
  roboBrain(double h, double e, double n, Interface& Linterface, Log& LlogC, Log& LlogGps);
  void navigateCompass();	//
  void navigateGPS();
  void navigateOdometer();
  virtual void navigate() {navigateCompass();navigateGPS();navigateOdometer();}
  virtual void guide();
  virtual void control();			//give data to servos, which will then be read by the simulation
  virtual void interfaceTime() {epochTime = interface.time();};
  void log() const;		//take data?
  virtual void showVector(Log& pkt);
};



#endif /* ROBOBRAIN_H_ */
