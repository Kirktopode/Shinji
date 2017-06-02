#include <stdio.h>
#include <stdlib.h>
#include "roboBrain.h"
#include "PacketReader.h"
#include "LogCSV.h"

/*

Okay, what do I need to do here? I need to build the robot, first of all. I'll need the
roboBrain with the interface I want. The interface I want, of course, is a PacketReader.

So, create the roboBrain object and the interface.

*/

const char* dataFile = "20170509T000918/record.csv";
const char* logCOutput = "packetOutput.csv";
const char* logGpsOutput = "packetGpsOutput.csv";

LogCSV logC(logCOutput);
LogCSV logGps(logGpsOutput);

PacketReader interface(dataFile);
roboBrain controller(0, 0, 0, interface, logC, logGps);

void setup(){
  
}

void loop(){
  interface.update();
  controller.navigateCompass();
  controller.showVector(logC);
}

int main(){
  setup();
  for(;;){
    loop();
    if(interface.isFinished()) break;
  }
  return 0;
}
