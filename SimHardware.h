#ifndef SIMHARDWARE_H_
#define SIMHARDWARE_H_

#include "Simulator.h"
#include "HardwarePi.h"

class SimHardware: public Simulator
{
  private:
    HardwarePiInterfaceArduino hardware;
  public:
    SimHardware(double Lh = 0, double lat = 0, double lon = 0): Simulator(Lh, lat, lon);
    bool button(int n = 17) {hardware.button(n);};
    double time() {return hardware.time();};
    void throttle (int n) {hardware.throttle(n); throttle(n);};
    void throttle (int n) {hardware.throttle(n); throttle(n);};
};

#endif
