#include "OpenLoopGuidance.h"
#include <cstdio>

void OpenLoopGuidance::control() {
  if(t0<=0) {
    if(interface.button()) {
      printf("Button!\n");
      t0=interface.time();
    }
    interface.steering(150);
    interface.throttle(150);
  } else if(t[currentRow]<(interface.time()-t0)) {
    printf("Controller: t=%f, i=%d, t_cmd=%f, s=%c, s=%d\n",interface.time()-t0,currentRow,t[currentRow],servoChannel[currentRow],servoSetting[currentRow]);
    if(servoChannel[currentRow]=='S') {
      interface.steering(servoSetting[currentRow]);
    } else {
      interface.throttle(servoSetting[currentRow]);
    }
    currentRow++;
  } else if(t[currentRow]>=(interface.time()-t0)) {
    if(servoChannel[currentRow]=='S') {
      interface.steering(servoSetting[currentRow-1]);
    } else {
      interface.throttle(servoSetting[currentRow-1]);
    }
  }else if(interface.button()) {
    printf("Button!\n");
    t0=interface.time();
    currentRow = 0;
  }
}

void OpenLoopGuidance::reset() {
  printf("Reset!\n");
  t0=0;
  currentRow=0;
}
