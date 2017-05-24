#include <stdlib.h> //This is for malloc, atoi, strtoi
#include <stdio.h>
#include <stdbool.h>

typedef struct packet {
  double time;
  unsigned int temp;
  int ax;
  int ay;
  int az;
  int gx;
  int gy;
  int gz;
  int MagOK;
  int bx;
  int by;
  int bz;
} packet;

/*  For now, heading is a global variable that will report where we thing we're
*   pointing
*/

double heading = 0;
double lastTime = 0;
const double averageG = 0.106582519;

/*  getPacket() is going to take a line of the .csv file with each iteration
*   I'll give it the destination array and the file handle and it'll stick 
*   all of the data into a series of arrays
*   Do I need to assign it any other variables? Maybe the location of the array?
*   It's going to return how many columns it grabbed. 
*/

packet getPacket(){
  
  packet result;
  
  scanf("%lf, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d", &result.time, 
    &result.temp, &result.ax, &result.ay, &result.az, &result.gx,
    &result.gy, &result.gz, &result.MagOK, &result.bx, &result.by,
    &result.bz);
    
  return result;
}

/*  processPacket() does magic to packets. It uses special magic math and propagates
*   the compass, because that phrase is magic.
*
*
*/

int processPacket(packet data){
  double dt = data.time - lastTime;
  
  double yawRate = (((double)data.gz) / 0x7FFF) * 250 + averageG; //Later, we will account for offset.
  heading -= yawRate * dt;
  
  while(heading < 0) heading = 360 + heading;
  while(heading > 360) heading = heading - 360;
  
  lastTime = data.time;
  
}

/* printState prints the state vector.
*
*/

int printState(packet data){
  printf("%lf, %lf\n", data.time, heading);
}

/*  First, we'll run setArray. Then, we'll run setArray until grabLine() returns a zero.
*   
*/

int main(){
  
  packet data;
  
  for(int i = 0;i < 100000;i++){
    data = getPacket();
    processPacket(data);
    if(i % 10 == 0) printState(data);
  }
  
  return 0;
}
