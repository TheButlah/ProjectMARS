#ifndef MARS_VECTOR_H
#define MARS_VECTOR_H

namespace MARS {
  class Vector {
  public:
    double x;
    double y;

    Vector(): x(0), y(0) {};
    Vector(double x, double y): x(x), y(x) {};
    
  };
}

#endif