#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "robot.h"    //Not sure if this header is even implemented here.
#include "PacketReader.h"
#include "LogCSV.h"

using namespace std;

/** Construct a simulator
 * @param h initial heading in degrees east of true north
 * @param Llat0 initial latitude in degrees north of WGS84 equator
 * @param Llon0 initial longitude in degrees east of WGS84 prime meridian
 */
PacketReader::PacketReader(const char* Ldata)
: Interface(steering,throttle), epochTime(0){
  if( (data = fopen(Ldata, "r") ) == NULL){
    printf("PacketReader is trying to read from an invalid file name.\n");
    exit(1);
  }
  char c = fgetc(data);
  while(c != '\n' && c != EOF){ //Skip one line before grabbing data.
  c = fgetc(data);
  }
}

PacketReader::~PacketReader(){
  fclose(data);
}

void PacketReader::readOdometer(uint32_t &timeStamp, int32_t &wheelCount, uint32_t &dt){
  //All time is in units of microseconds
//  wheelCount = floor(distanceTraveled/tickDistance); //Since it is PI/2 instead of PI*2, this is in quarter-turns, as appropriate.
//  timeStamp =time()*10000000; //Not quite accurate, should be the time that the last click happened, but roboBrain doesn't care.
//  dt=0; //We will eventually fake this from the current wheel speed. Not quite accurate, but it will do for now, especially as the roboBrain doesn't currently use it.
}

bool PacketReader::checkNavChar() {
//  int nCharsShouldTransmit=((epochTime-pps)-nmeaDelay)/charTime;
//  if(nCharsShouldTransmit<0) nCharsShouldTransmit=0;
//  if(nCharsShouldTransmit>strlen(nmea)) nCharsShouldTransmit=strlen(nmea);
//  return nCharsShouldTransmit>charsSent;
  return false;
}

char PacketReader::readChar() {
//  if(!checkNavChar()) {
//	cout << "Not allowed to read when nothing available" << endl;
//	exit(2);
//  }
//  char result=nmea[charsSent];
//  charsSent++;
  return 0;
}



/** Update the MPU data of the robot
 */
void PacketReader::update() {
  if(fscanf(data,"%lf, %hd, %hd, %hd, %hd, %hd, %hd, %hd, %hd, %hd, %hd, %hd", &epochTime, 
  &temp, &a[0], &a[1], &a[2], &g[0], &g[1], &g[2], &MagOK, &b[0], &b[1], &b[2]) == EOF)
    finished = true;
}

/** Print information related to the current state in CSV format */
void PacketReader::showVector(Log& logC) const {
//  logC.write(simThrottle.read(), "throttle");
//  logC.write(simSteering.read(), "steering");
}

void PacketReader::testOdometer(double et){ }
