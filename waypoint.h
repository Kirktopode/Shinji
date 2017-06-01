/*
 * robot.h
 */
#ifndef waypoint_h
#define waypoint_h

#include <cmath>
#include "Vector.h"

class waypoint: public Vector<2,fp> {
public:
  fp& easting()  {return comp[0];}; ///< Associate the name "easting" with component 0
  fp& northing() {return comp[1];}; ///< Associate the name "northing" with component 1
  fp easting() const {return comp[0];}; ///< Get the easting component, used in const context (see http://www.cprogramming.com/tutorial/const_correctness.html)
  fp northing() const {return comp[1];};///< Get the northing component, used in const context
  /** Compute the heading of this waypoint relative to the origin.
   * @return Heading in degrees east of true north, from 0 to almost 360
   */
  fp heading() {
    fp result=atan2(easting(),northing())*180/PI;
    if(result<0) result+=360;
    return result;
  }
  /** Construct a waypoint
   * @param e easting value
   * @param n northing value
   */
  waypoint(fp e=0, fp n=0) {easting()=e;northing()=n;};
  /** Copy constructor */
  waypoint(Vector<2,fp> other):waypoint(other[0],other[1]) {};
};

#endif /* ROBOT_H_ */
