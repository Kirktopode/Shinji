
plane {
  x,5 //Perpendicular to z vector, 0 units from origin
  pigment {checker color <1,1,1> color <0.8,0.8,0.8>}//finish {ambient 1 diffuse 0}
  //This makes it perfect solid red
  finish {ambient 0 diffuse 1} //This makes it respond to normal light
}

plane {
  y,0 //Perpendicular to z vector, 0 units from origin
  pigment {checker color <1,1,1> color <0.8,0.8,0.8>}//finish {ambient 1 diffuse 0}
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
  look_at <10,0,2>
}
