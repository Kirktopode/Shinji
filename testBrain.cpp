#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
//#include "HeaderSimRobo.h"
#include "robot.h"
#include "roboBrain.h"
#include "Simulator.h"
#include "testBrain.h"
#include <wiringPi.h>
#include "LogCSV.h"

testBrain::testBrain(double h, double e, double n, Interface& Linterface, Interface& LhardInterface, LogCSV& LlogC):
roboBrain(h, e, n, Linterface), hardInterface(LhardInterface), logC(LlogC)
{ }



void testBrain::showVector() const {	//nowpoint,waypoints[nowpoint].easting(), waypoints[nowpoint].northing(),desiredHeading,headingChange)
	logC.write(hardInterface.time(), "t");
	logC.write(pos.easting(), "easting");
	logC.write(pos.northing(), "northing");
	logC.write(nowpoint, "nowpoint");
	logC.write(waypoints[nowpoint].easting(), "targetEasting");
	logC.write(waypoints[nowpoint].northing(), "targetNorthing");
	logC.write(desiredHeading, "desiredHeading");
	logC.write(headingChange, "headingChange");
}

void testBrain::control(){
	if(nowpoint == 0) return;
		else {
			if(headingChange >= 300){
				interface.throttle.write(150);
				interface.steering.write(150);
				hardInterface.throttle.write(150);
				hardInterface.steering.write(150);
				return;
			}
			interface.throttle.write(140);
			interface.steering.write(headingChange * double (50)/180+150);
			hardInterface.throttle.write(140);
			hardInterface.steering.write(headingChange * double (50)/180+150);
		}
}

void testBrain::guide(){
	if(nowpoint == 0){
			//fillBuffer();
			if(hardInterface.button()){
				nowpoint = 1;
			//	setOffSet();
			}
	} else {
		if(dot((waypoints[nowpoint]- waypoints[nowpoint - 1]),waypoints[nowpoint] - pos) < 0){
			nowpoint += 1;
		}
		if(nowpoint >= wpcount){
			headingChange=400;
					return;
		}
		desiredHeading = static_cast<waypoint>(waypoints[nowpoint]-pos).heading();

		headingChange = desiredHeading - heading;
		if(headingChange > 180){
			headingChange -= 360;
		}
		else if (headingChange < -180){
			headingChange += 360;
		}
	}
}
