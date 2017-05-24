/*
 * Simulator.h
 */
#ifndef Simulator_h
#define Simulator_h
#include <stdint.h>
#include "interface/Interface.h"
#include "log/Log.h"
#include "waypoint.h"

//const fp re=6378137.0;

/** Simulated servo. Keeps track of physical value of servo (steering angle, speed of robot, etc) and manages servo slewing */
class SimServo:public Servo {
private:
  int commanded; ///< Commanded value in DN
  const int cmdmin; ///< Minimum allowed commanded value in DN
  const int cmdmax; ///< Maximum allowed commanded value in DN
  double physical;  ///< Current value of servo in physical units
  const double physmin; ///< Physical value corresponding to minimum possible command
  const double physmax; ///< Physical value corresponding to maximum possible command
  const double slewrate; ///< Rate of change of physical value in response to command changes, in physical units/second
public:
  /** Construct a servo
   * @param cmdmin value for cmdmin field
   * @param cmdmax value for cmdmax field
   * @param physmin value for physmin field
   * @param physmax value for physmax field
   * @param slewrate value for slewrate field
   */
  SimServo(int cmdmin, int cmdmax, double physmin, double physmax, double slewrate);
  virtual void write(int n);
  /** Read the current value of the servo
   * @return current value of the servo in physical units. May not match commanded value, due to slew rate.
   */
  double read() const {return physical;}
  /** Step the servo in time. This allows the physical value to slew to the commanded value.
   * @param t Interval since last timestep in seconds
   */
  void timeStep(double t);
  /** Run the test case for the servo. This test case can't be evaluated automatically. It should be run through
   * a chart on a spreadsheet in order for a human to evaluate it.
   */
  static void test();
  /** Destructor. Doesn't do anything explicitly, but it's good form to include a virtual destructor
   * for any class which has virtual methods.
   */
  virtual ~SimServo() {};
};

/** Simulated implementation of robot interface. This keeps track of where the robot actually is
 *
 */
class Simulator : public Interface {
private:
  double dt;
  double yawRate;	  ///< Yaw rate in deg/s
  double lat0;            ///< Origin latitude in degrees north of WGS84 equator
  double lon0;            ///< Origin longitude in degrees east of WGS84 prime meridian
  waypoint pos;           ///< Actual position of robot relative to lat0 and lon0
  double heading;         ///< Actual heading in degrees east of True North
  double kappa;           ///< Turning curvature in 1/m, positive is turning to the right
  const double wheelBase=0.3; ///< Wheelbase coefficient in meters
  const double ppsInterval=1.0; ///< Position is available at this interval in seconds
  double pps;             ///< epoch time of the last PPS in seconds
  const double dpps=1.0;  ///< interval between GPS fixes in seconds
  const double nmeaDelay=0.4; ///< NMEA sentence isn't available until this long after PPS
  char nmea[256];         ///< NMEA sentence to send to robot
  const double baudRate=4800.0; ///< Simulated baud rate, characters only become available according to this rate
  const double bitsPerChar=10.0; ///< Number of bit times required to send one char, 10 because we have one start bit, 8 data bits, and one stop bit
  const double charTime=1.0/(baudRate/bitsPerChar); ///< Time required to send one char in seconds
  int charsSent;          ///<Number of characters in this sentence which have already been transmitted
  double epochTime;       ///< epoch time - current time relative to start of simulation in seconds
  double distanceTraveled; ///< distance traveled since startup

  SimServo simSteering;  ///<Actual instance of SimServo. Constructor points Interface::steering at this.
  SimServo simThrottle;  ///<Actual instance of SimServo. Constructor points Interface::throttle at this.

  void generateNewFix();

  /** Return the current latitude. This is calculated from the current northing and the initial latitude.
   * @return Latitude in degrees north of WGS84 equator
   */
  double lat() const {return lat0+(pos.northing()/re)*180.0/PI;};
  /** Return the current longitude. This is calculated from the current easting and the initial latitude and longitude.
   * Note that the cosine factor necessary for converting easting to longitude is calculated from the initial latitude,
   * not the current latitude. This doesn't matter at the small scale of an AVC course. If you are considering an
   * area big enough for the change in latitude to make a difference, easting and northing are no longer valid concepts.
   * @return Longitude in degrees east of WGS84 prime meridian (Note that all longitudes in continental USA are negative).
   */
  double lon() const {return lon0+(pos.easting()/cos(lat0*PI/180.0)/re)*180.0/PI;}
  /** Convert a measurement in degrees to a measurement in degrees and minutes, appropriate for NMEA output
   * @param deg Angle in decimal degrees
   * @return Same angle in degrees*100+minutes, so when printed in decimal, the format is DDMM.MMMMM, IE the units and
   * tens digits are minutes, the higher digits are degrees, and the fractional digits are fraction of minutes.
   */
  static double deg2dm(double deg) {return floor(deg)*100+(deg-floor(deg))*60;}
public:
  Simulator(double Lh = 0, double lat = 0, double lon = 0);
  /** Destructor. Doesn't do anything explicitly, but it's good form to include a virtual destructor
   * for any class which has virtual methods.
   */
  virtual ~Simulator() {};
  void update(double dt);
  void showVector(Log& logC) const;  ///<reports data for storage in .csv file
  static void testNMEA();            ///< Test the NMEA generation code

  virtual double checkPPS();
  virtual bool checkNavChar();
  virtual char readChar();
  virtual void readOdometer(uint32_t& timeStamp, int32_t& wheelCount, uint32_t& dt);
  virtual bool readGyro(int16_t g[]);
  virtual bool readGyro(int16_t g[], int16_t& t);
  virtual bool checkPPS(double& t) {return false;}; //PPS doesn't work yet -- all attempts to read will fail
  virtual bool readAcc(int16_t g[]) {return false;}; //Acc doesn't do anything yet -- all attempts to read will fail
  virtual bool readMag(int16_t b[]) {return false;}; //Mag doesn't do anything yet -- all attempts to read will fail
  virtual fp time() {return epochTime;};
  virtual bool button(int pin=17) {return epochTime >= 1;};
  /** Back-door direct access to heading
   * @param h [out] heading in degrees east of true north
   */
  virtual void cheatHeading(double& h) {h=heading;};
  virtual void testOdometer(double t);
};



#endif /* SIMULATOR_H_ */
