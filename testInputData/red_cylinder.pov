cylinder {  <10,10,0>,<10,10,5>,1.5
  pigment {color rgb <1,0,0>}//finish {ambient 1 diffuse 0}
  //This makes it perfect solid red
  finish {ambient 0 diffuse 1}
  //This makes it respond to normal light
}

plane {
  z,0 //Perpendicular to z vector, 0 units from origin
  pigment {checker color <1,1,1> color <0,0,0>}//finish {ambient 1 diffuse 0}
  //This makes it perfect solid red
  finish {ambient 0 diffuse 1} //This makes it respond to normal light
}
light_source {
  <-20,-0,20>*1000  //quite far away
  color rgb <1,1,1> //white light
}
camera {
  sky z
  up y
  right -x*image_width/image_height
  angle 60
  location <0,0,2>
  look_at <10,10,2>
}
