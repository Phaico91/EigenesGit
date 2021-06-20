#!/usr/bin/env python3

#lst{PyGeometricShape}
import math  # for math.pi
# for isinstance use this import
from numbers import Number

class Shape():
  instance_counter = 0
  def __init__(self):
    self.instance_id = \
      Shape.instance_counter
    Shape.instance_counter += 1
  def iid(self):
    return self.instance_id
  def __str__(self):
    return 'Shape'
#lstend#

#lst{PyGeometricLine}
class Line(Shape): # inherit from Shape
  def __init__(self,arg=None):
    Shape.__init__(self)
    if arg is None:
      self.length = 1
    elif isinstance(arg,Number):
      self.length = arg
    else:
      assert isinstance(arg,Line)
      self.length = arg.length_get()
  def length_set(self,_length):
    self.length = _length
  def length_get(self):
    return self.length
  def __str__(self):
    return 'Line, attr : {{length : {}}}'\
            .format(self.length)
#lstend#

#lst{PyGeometricCircle}
class Circle(Shape): # inherit from Shape
  def __init__(self,arg=None):
    Shape.__init__(self)
    if arg is None:
      self.radius = 1
    elif isinstance(arg,Number):
      self.radius = arg
    else:
      assert isinstance(arg,Circle)
      self.radius = arg.radius_get()
  def radius_set(self,_radius):
    self.radius = _radius
  def radius_get(self):
    return self.radius
  def area_get(self):
    radius = self.radius_get()
    return math.pi * radius * radius
  def __str__(self):
    return 'Circle, attr : {{radius : {}, area : {:.6f}}}'\
            .format(self.radius,self.area_get())
#lstend#

#lst{PyGeometricRectangleInit}
class Rectangle(Shape): # inherit from Shape
  def __init__(self,arg=None,width=None):
    Shape.__init__(self)
    if arg is None and width is None:
      self.height = self.width = 1
    else:
      assert arg is not None
      if isinstance(arg,Rectangle):
        self.height = arg.height_get()
        self.width = arg.width_get()
        assert width is None
      else:
        if isinstance(arg,Number):
          self.height = arg
        else:
          assert isinstance(arg,Line)
          self.height = arg.length_get()
        assert width is not None
        if isinstance(width,Number):
          self.width = width
        else:
          assert isinstance(width,Line)
          self.width = width.length_get()
#lstend#
#lst{PyGeometricRectangleGetSet}
  def height_set(self,height):
    self.height = height
  def width_set(self,width):
    self.width = width
  def height_get(self):
    return self.height
  def width_get(self):
    return self.width
  def area_get(self):
    return self.height * self.width
  def __str__(self):
    return ('Rectangle, attr : {{height : {}, '
            'width : {}, area : {}}}')\
            .format(self.height,self.width,
                    self.area_get())
#lstend#

#lst{PyGeometricSquare}
class Square(Rectangle): # inherit from Rectangle
  def __init__(self,arg=None):
    if arg is None:
      side_length = 1
    elif isinstance(arg,Line):
      side_length = arg.length_get()
    elif isinstance(arg,Square):
      side_length = arg.side_length_get()
    else:
      side_length = arg
    Rectangle.__init__(self,side_length,side_length)
  def side_length_set(self,side_length):
    self.height_set(side_length)
    self.width_set(side_length)
  def side_length_get(self):
    return self.height_get()
  def __str__(self):
    return ('Square, attr : {{side_length : {}, '
            'area : {}}}')\
            .format(self.side_length_get(),self.area_get())
#lstend#

#lst{PyGeometricOutput}
def output(name,var):
  print('{} : {{id : {}, class : {}}}'.format(name,var.iid(),var))
#lstend#

#lst{PyGeometricMain}
def main():
  sh = Shape()
  l0 = Line()
  l1 = Line(3)
  l2 = Line(l1)
  c0 = Circle()
  c1 = Circle(2)
  c2 = Circle(c0)
  r0 = Rectangle()
  r1 = Rectangle(1,2)
  r2 = Rectangle(l1,l2)
  r3 = Rectangle(r0)
  sq0 = Square()
  sq1 = Square(10)
  sq2 = Square(l1)
  sq3 = Square(sq2)
  output('sh',sh)
  output('l0',l0)
#lstend#
  output('l1',l1)
  output('l2',l2)
  l2.length_set(10)
  output('l2',l2)
  output('c0',c0)
  output('c1',c1)
  output('c2',c2)
  output('r0',r0)
  output('r1',r1)
  output('r2',r2)
  output('r3',r3)
  output('sq0',sq0)
  output('sq1',sq1)
  sq1.side_length_set(5)
  output('sq1',sq1)
  output('sq2',sq2)
  output('sq3',sq3)
  r4 = Rectangle(2.5,2.8)
  output('r4',r4)

main()
