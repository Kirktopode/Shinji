/*
 * Interface.h
 */
#ifndef Interface_h
#define Interface_h

#include "math/float.h"
#include <inttypes.h>

/** Abstract interface to a servo. This is write-only, because a real physical servo is write-only */
class Servo {
public:
  /** Command the servo to a particular position
   * @param n new commanded value in DN
   */
  virtual void write(int n)=0;
  virtual ~Servo() {};
};

/** Abstract class representing the interface between the robot navigation, guidance, and control (GNC) software
 * and the real or virtual robot it is controlling.
 */
class Interface {
public:
  /** Gets time of last PPS in seconds from epoch. You may call this function as often as you like -- it wil
   * only return a new value whenever a new PPS has arrived. This represents the time at which the *next* GPS fix
   * is valid, IE a PPS pulse is generated, then some time later a new position is transmitted.
   * @param t [out] epoch time of the most recent PPS in second
   * @return true if there is new PPS since the last time this function was called
   */

  //GPS Interface
  virtual bool checkPPS(fp& t) = 0;
  /** Check if a character of GPS data is available
   * @return true if a character is available, false if not
   */
  virtual bool checkNavChar() = 0;
  /** Get the next character of GPS data. Only call this if checkNavChar() returns true, return value is undefined
   * if checkNavChar() is false.
   * @return one character of GPS data in NMEA format
   */
  virtual char readChar() = 0;
  /** Get the current time
   * @return Current epoch time in seconds
   */
  virtual fp time()=0;
  /** Check whether a button is pushed
   * @param[in] pin WiringPi pin number for the pin to check. The robot will have a button on pin 17
   * @return true if the button is pushed (pin is low voltage)
   */
  virtual bool button(int pin=17)=0;
  /**	Read the odometer
   * @param timeStamp [out] time of last time readOdometer was called in microseconds
   * @param wheelCount [out] number of sectors read since the odometer was reset
   * @param dt [out] time since last call of readOdometer in microseconds
   * 		 */
  virtual void readOdometer(uint32_t &timeStamp, int32_t &wheelCount, uint32_t &dt)=0;
  /** Read the gyroscope
   * @param[out] g vector of gyroscope readings, in DN. One DN typically represents
   * a constant fraction of a degree per second rotation rate around each axis. Number
   * is a raw readout of the sensor, in two's complement integer, in proper axis order,
   * IE element 0 is X, 1 is Y, and 2 is Z.
   * @return true if measurement was successful and valid, false if not (I2C read error, etc)
   */
  virtual bool readGyro(int16_t g[]) {int16_t t; return readGyro(g,t);};
  /** Read the accelerometer
   * @param[out] g vector of gyroscope readings, in DN. One DN typically represents
   * a constant fraction of a degree per second rotation rate around each axis. Number
   * is a raw readout of the sensor, in two's complement integer, in proper axis order,
   * IE element 0 is X, 1 is Y, and 2 is Z.
   * @return true if measurement was successful and valid, false if not (I2C read error, etc)
   */
  virtual bool readAcc(int16_t g[])=0;
  /** Read the whole MPU sensor
   * @param[out] a vector of accelerometer readings, in DN.
   * @param[out] g vector of gyroscope readings, in DN.
   * @param[out] t temperature reading in DN.
   * @return true if measurement was successful and valid, false if not (I2C read error, etc)
   */
  virtual bool readMPU(int16_t a[], int16_t g[], int16_t& t) {
    if(!readAcc(a)) return false;
    return readGyro(g,t);
  }
  /** Read the gyroscope
   * @param[out] g vector of gyroscope readings, in DN. One DN typically represents
   * a constant fraction of a degree per second rotation rate around each axis. Number
   * is a raw readout of the sensor, in two's complement integer, in proper axis order,
   * IE element 0 is X, 1 is Y, and 2 is Z.
   * @param[out] t temperature reading in DN
   * @return true if measurement was successful and valid, false if not (I2C read error, etc)
   */
  virtual bool readGyro(int16_t g[], int16_t& t) = 0;
  /** Read the magnetometer
   * @param[out] b vector of magnetometer readings, in DN. Number
   * is a raw readout of the sensor, in two's complement integer, in proper axis or$
   * IE element 0 is X, 1 is Y, and 2 is Z. In hardware, the AK8963 part does not
   * have its axes aligned with the rest of the MPU. This routine does the axis
   * rotation (merely axis swap and/or negation) to make the axes match the rest
   * of the MPU
   * @param[out] t temperature reading in DN
   * @return true if measurement was successful and valid, false if not (I2C read e$
   */
  virtual bool readMag(int16_t b[]) = 0;

  Servo& steering; ///< Reference to steering servo object
  Servo& throttle; ///< Reference to throttle servo object
  /** Construct a robot interface
   * @param Lsteering reference to servo object which controls steering
   * @param Lthrottle reference to servo object which controls throttle
   */
  Interface(Servo& Lsteering, Servo& Lthrottle):
  steering(Lsteering), throttle(Lthrottle) {};
  /** Destructor. Doesn't do anything explicitly, but it's good form to include a virtual destructor
   * for any class which has virtual methods.
   */
  virtual ~Interface() {};
};

#endif /* ROBOT_H_ */
