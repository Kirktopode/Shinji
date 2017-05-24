#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Loginator/Yukari/sim/robot.h"
#include "Simulator.h"
#include "LogCSV.h"

using namespace std;

/** Construct a simulator
 * @param h initial heading in degrees east of true north
 * @param Llat0 initial latitude in degrees north of WGS84 equator
 * @param Llon0 initial longitude in degrees east of WGS84 prime meridian
 */
Simulator::Simulator(double h, double Llat0, double Llon0)
: Interface(simSteering,simThrottle),heading(h), lat0(Llat0), lon0(Llon0), kappa(0),simThrottle(100, 200, 10, -10, 5), simSteering(100, 200, -15, 15, 75),epochTime(0),distanceTraveled(0)
{
    generateNewFix();
}


//Implement the robot Interface
double Simulator::checkPPS() {
  return pps;
}

void Simulator::readOdometer(uint32_t &timeStamp, int32_t &wheelCount, uint32_t &dt){
  //All time is in units of microseconds
  wheelCount = floor(distanceTraveled/tickDistance); //Since it is PI/2 instead of PI*2, this is in quarter-turns, as appropriate.
  timeStamp =time()*10000000; //Not quite accurate, should be the time that the last click happened, but roboBrain doesn't care.
  dt=0; //We will eventually fake this from the current wheel speed. Not quite accurate, but it will do for now, especially as the roboBrain doesn't currently use it.
}

bool Simulator::checkNavChar() {
  int nCharsShouldTransmit=((epochTime-pps)-nmeaDelay)/charTime;
  if(nCharsShouldTransmit<0) nCharsShouldTransmit=0;
  if(nCharsShouldTransmit>strlen(nmea)) nCharsShouldTransmit=strlen(nmea);
  return nCharsShouldTransmit>charsSent;
}

char Simulator::readChar() {
  if(!checkNavChar()) {
	cout << "Not allowed to read when nothing available" << endl;
	exit(2);
  }
  char result=nmea[charsSent];
  charsSent++;
  return result;
}

bool Simulator::readGyro(int16_t g[], int16_t& t) {
  g[0] = g[1] = 0;
  g[2] = (yawRate * 0x7FFF)/250;
  return true;
}
bool Simulator::readGyro(int16_t g[]) {
  int16_t t;
  return readGyro(g,t);
}

/** Generate a new GPS fix. Update the PPS value, create the RMC sentence, set the pointers for spooling out the sentence */
void Simulator::generateNewFix() {
  pps=floor(epochTime/dpps+0.5);
  int s=pps;
  int m=s/60;
  s=s%60;
  int h=m/60;
  m=m%60;
  char ns=lat()>0?'N':'S';
  double latdm=deg2dm(fabs(lat()));
  char ew=lon()>0?'E':'W';
  double londm=deg2dm(fabs(lon()));
  double speed=simSteering.read()*1.94384449; // Convert throttle servo value (speed of robot in m/s) to knots

  sprintf(nmea,"$GPRMC,%02d%02d%02d,A,%010.5f,%c,%011.5f,%c,%05.1f,%05.1f,170916,000.0,W*",h,m,s,latdm,ns,londm,ew,speed,heading);
  char checksum=0;
  for(int i=1;i<strlen(nmea)-1;i++) {
	checksum ^= nmea[i];
  }
  sprintf(nmea,"$GPRMC,%02d%02d%02d,A,%010.5f,%c,%011.5f,%c,%05.1f,%05.1f,170916,000.0,W*%02X\x0d\x0a",h,m,s,latdm,ns,londm,ew,speed,heading,checksum);
  charsSent=0;
//  cout << nmea << endl;
}

/** Update the actual position and heading of the robot
 * @param dt update time interval in seconds
 */
void Simulator::update(double Ldt) {
  dt = Ldt;
  simSteering.timeStep(dt);
  simThrottle.timeStep(dt);

  epochTime+=dt; //Update current time
  distanceTraveled += simThrottle.read() * double(dt);

  if((epochTime-pps)>dpps) {
    generateNewFix();
  }

  kappa = tan( simSteering.read() * PI / 180)/wheelBase;
  yawRate = kappa*simThrottle.read()*180/PI; //Yaw rate in deg/s
  heading +=yawRate*dt; //Add yaw increment in degrees
  if (heading < 0) {
    heading = 360 + heading;
  } else if (heading > 360) {
    heading -= 360;
  }
  waypoint dpos(sin(heading*PI/180)*simThrottle.read()*dt,
                cos(heading*PI/180)*simThrottle.read()*dt);
  pos+=dpos;
}

/** Print information related to the current stat in CSV format */
void Simulator::showVector(Log& logC) const {
  logC.write(pos.easting(), "easting");
  logC.write(pos.northing(), "northing");
  logC.write(simThrottle.read(), "throttle");
  logC.write(simSteering.read(), "steering");
  logC.write(heading, "heading");
  logC.write(kappa, "kappa");
}

void Simulator::testNMEA() {
  Simulator testSim;
  testSim.throttle.write(127);
  while(true) {
	double dt = .05; //Interval time; simulates amount of time between each function's call

	testSim.update(dt); //contains simulation adjustment and timesteps the servos

	if(testSim.time() >= 60)
	  break;
	}
}
void Simulator::testOdometer(double et){ //virtual void readOdometer(uint32_t& timeStamp, int32_t& wheelCount, uint32_t& dt);
	simThrottle.write(127);
	int32_t wheelCount = 0;
	uint32_t timeStamp = 0;
	uint32_t dt = 0;
	double predictedNorthing = 0;
	while(et > 0){
		update(.05);
		readOdometer(timeStamp, wheelCount, dt);
		et -= .05;
		predictedNorthing += wheelCount * PI * .03175 / 2;
	}
	printf("wheelCount: %d\nNorthing: %2.2f\nNorthing predicted by wheelCount: %2.2f\nDistance traveled: %2.2f", wheelCount, pos.northing(), predictedNorthing, distanceTraveled);
}


//++++++++++++++++++++++++++++++++++++++++++++++++++Servo methods.

SimServo::SimServo(int cmdmin, int cmdmax, double physmin, double physmax, double slewrate) :
cmdmin(cmdmin), cmdmax(cmdmax), physmin(physmin), physmax(physmax), slewrate(slewrate), commanded(0), physical(0)
{
	if(cmdmax < cmdmin)
	{
		cerr << "bad servo max/min -- set max greater than or equal to min";
		exit(1);
	}
}
void SimServo::write(int n)
{
	if(n > cmdmax)
	{
		n = cmdmax;
	}
	else if (n < cmdmin)
	{
		n = cmdmin;
	}
	else
	{
		commanded = n;
	}
}
void SimServo::timeStep(double t)
{
	double commandPhysical = (double(commanded - cmdmin)/(cmdmax - cmdmin)) * (physmax - physmin) + physmin;
	if (physical < commandPhysical)				//physical vs command * command should be 8-bit int, make physical into a proportional double from -15 to 15 degrees
	{
		physical += t * slewrate;
		if (physical > commandPhysical)
			physical = commandPhysical;
	}
	else if (physical > commandPhysical)
	{
		physical -= t * slewrate;
		if (physical < commandPhysical)
			physical = commandPhysical;
	}
}
void SimServo::test()
{
	SimServo Steering = SimServo(1000, 2000, -15, 15, 5);
	double time = 0;
	while(time < 20)
	{
		cout << time << ", " << Steering.read() << endl;
		if(time < 4)
			Steering.write(2000);
		else if(time < 14)
			Steering.write(1000);
		else if(time >= 14)
		{
			Steering.write(1500);
		}
		Steering.timeStep(.05);
		time += .05;
	}
}

