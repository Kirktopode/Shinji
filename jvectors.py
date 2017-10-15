import math

class Vector:
  def __init__(self, x, y):
    self.x = x * 1.0
    self.y = y * 1.0
  def magnitude(self):
    return (math.sqrt(self.x**2 + self.y**2))
  def dot(self, vector):
    return self.x * vector.x + self.y * vector.y
  def cross(self, vector):
    return abs(self.x * vector.y - self.y * vector.x)
  def dist(self, vector):
    return math.sqrt( (self.x - vector.x)**2 + (self.y - vector.y)**2 )
  def sin(self, vector):
    return self.cross(vector)/(self.magnitude() * vector.magnitude())
  def cos(self, vector):
    return self.dot(vector)/(self.magnitude() * vector.magnitude())
  def norm(self):
    return self.div(self.magnitude())
<<<<<<< HEAD
=======
  def __add__(self, other):
      return Vector(self.x + other.x, self.y + other.y)
  def mult(self, factor):
      return Vector(self.x * factor, self.y * factor)
  def __sub__(self, other):
      return Vector(self.x - other.x, self.y - other.y)
  def div(self, divisor):
      divisor *= 1.0
      return Vector(self.x/divisor, self.y/divisor)
  def __eq__(self, other):
      return self.x == other.x and self.y == other.y
  def __ne__(self, other):
      return not self == other
  def __str__(self):
      return str(self.x) + " " + str(self.y)
>>>>>>> bug-fixing
