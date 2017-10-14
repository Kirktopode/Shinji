import math

class Vector:
  def __init__(self, x, y):
    self.x = x
    self.y = y
  def magnitude(self):
    return (math.sqrt(self.x**2 + self.y**2))
  def dot(self, vector):
    return self.x * vector.x + self.y * vector.y
  def cross(self, vector):
    return abs(self.x * vector.y - self.y * vector.x)
  def dist(self, vector):
    return math.sqrt( (self.x - vector.x)**2 + (self.y - vector.y)**2 )
  def add(self, vector):
    return Vector(vector.x + self.x, vector.y + self.y)
  def diff(self, vector):
    return Vector(vector.x - self.x, vector.y - self.y)
  def equals(self, vector):
    return self.x == vector.x and self.y == vector.y
  def div(self, num):
    return Vector(self.x/num, self.y/num)
  def mult(self, num):
    return Vector(self.x*num, self.y*num)
  def sin(self, vector):
    return self.cross(vector)/(self.magnitude() * vector.magnitude())
  def cos(self, vector):
    return self.dot(vector)/(self.magnitude() * vector.magnitude())
  def norm(self):
    return self.div(self.magnitude())
