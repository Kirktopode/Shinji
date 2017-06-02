/*
 * controller.h
 */
#ifndef controller_h
#define controller_h

#include "float.h"
#include "Log.h"
#include "Interface.h"

const fp re=6378137.0;     ///< radius of Earth, used to convert between lat/lon and northing/easting
const fp tickDistance=0.30198;

class Controller {
protected:
  Interface& interface;
public:
  virtual void showVector(Log& pkt)=0;
  virtual void navigate() {};
  virtual void guide() {};
  virtual void control() {};
  Controller(Interface& Linterface):interface(Linterface) {};
  virtual ~Controller() {};
};

#endif /* controller_h */
