/*
 *  PacketReader.h
 */
 
#ifndef PacketReader_h
#define PacketReader_h

#include <cstdio>
#include "Interface.h"
#include "Log.h"

class NullServo:public Servo {
public:
  /** Command the servo to a particular position
   * @param n new commanded value in DN
   */
  virtual void write(int n) {};
  virtual ~NullServo() {};
};

class PacketReader:public Interface {
private:
  FILE* data;
  bool finished;          ///< becomes true if the end of the file has been reached
  double epochTime;       ///< epoch time - current time relative to start of recording in seconds
  int16_t temp;           ///< temperature reading from .csv file
  int16_t a[3];           ///< Accelerometer readings from .csv file, in order x, y, z
  int16_t g[3];           ///< Gyroscope readings from .csv file, in order x, y, z
  int16_t b[3];           ///< Magnetometer readings from .csv file, in order x, y, z
  int16_t MagOK;          ///< MagOK value from .csv file.

//We'll hold onto commented-out GPS attributes and methods for now; we might need PacketReader
//to read nmea data.

//  const double ppsInterval=1.0; ///< Position is available at this interval in seconds
//  double pps;             ///< epoch time of the last PPS in seconds
//  const double dpps=1.0;  ///< interval between GPS fixes in seconds
//  const double nmeaDelay=0.4; ///< NMEA sentence isn't available until this long after PPS
//  char nmea[256];         ///< NMEA sentence to send to robot
//  const double baudRate=4800.0; ///< Simulated baud rate, characters only become available according to this rate
//  const double bitsPerChar=10.0; ///< Number of bit times required to send one char, 10 because we have one start bit, 8 data bits, and one stop bit
//  const double charTime=1.0/(baudRate/bitsPerChar); ///< Time required to send one char in seconds
//  int charsSent;          ///<Number of characters in this sentence which have already been transmitted
  NullServo steering;
  NullServo throttle;

public:
  PacketReader(const char* Ldata);
  /** Destructor. Doesn't do anything explicitly, but it's good form to include a virtual destructor
   * for any class which has virtual methods.
   */
  virtual ~PacketReader();
  
  /** update() will move to the next line of information, grabbing each piece of information and
  * storing it somewhere important. This is the critical part of what PacketReader does.
  */
  
  void update();
  
  /** showVector() will take the state vector and stick it in a log file, just like normal.
  */
  
  void showVector(Log& logC) const;  ///<reports data for storage in .csv file
  
//  static void testNMEA();            ///< Test the NMEA generation code
  virtual bool checkNavChar();  //Not using NMEA, so this returns false every time.
  virtual char readChar(); //Not using NMEA, so this returns 0 every time.
  virtual void readOdometer(uint32_t& timeStamp, int32_t& wheelCount, uint32_t& dt); //Just an empty function for now.

  /** Grabs data taken from the line. Returns the most recent state.
  * Why does readGyro take "t" as a parameter? It isn't in the implementation file.
  * Commenting this out until I know that I have a use for it.
  */

  virtual bool readGyro(int16_t Lg[]){
    Lg[0] = g[0];
    Lg[1] = g[1];
    Lg[2] = g[2];
    return true;
  };
  virtual bool readGyro(int16_t Lg[], int16_t& t){
    Lg[0] = g[0];
    Lg[1] = g[1];
    Lg[2] = g[2];
    t = temp;
    return true;
  };
//  virtual bool readGyro(int16_t Lg[], int16_t& t);
  
  /** Not using PPS just yet; commenting it out.
  */
  
  virtual bool checkPPS(fp& t) {return false;}; //PPS doesn't work yet -- all attempts to read will fail

  /** Returns information read from packet.
  */
  virtual bool readAcc(int16_t La[]) {
    La[0] = a[0];
    La[1] = a[1];
    La[2] = a[2];
    return true;
  };
  /** Returns information read from packet.
  */
  virtual bool readMag(int16_t Lb[]) {
    Lb[0] = a[0];
    Lb[1] = a[1];
    Lb[2] = a[2];
    return true;
  };
  virtual fp time() {return epochTime;};
  virtual bool button(int pin=17) {return epochTime >= 60;}; //Button will not be used, but must be defined.
  /** Back-door direct access to heading
   * @param h [out] heading in degrees east of true north
   */
//  virtual void cheatHeading(double& h) {h=heading;};
  virtual void testOdometer(double t); //Not sure how we'll use it, but keeping it for now.
  bool isFinished(){ return finished; };

};

#endif
