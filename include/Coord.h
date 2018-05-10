#ifndef MARS_COORD_H
#define MARS_COORD_H

#include <functional>

namespace MARS {
  /*
   * Coord - 2D location with xy coordinates. Can be used in std::vector, std::unordered_set, std::unordered_map
   */
  class Coord {
  public:
    int x; // X coordinate
    int y; // Y coordinate
    
    Coord(): x(-1), y(-1) {};
    Coord(int x, int y): x(x), y(y) {};

    /*
     * Compare two coordinates
     */
    bool operator==(const Coord& other) const {
      return x == other.x && y == other.y;
    }
  };
}

namespace std {
  /*
   * Hash a Coord instance
   */
  template <>
  struct hash<MARS::Coord> {
  
    std::size_t operator()(const MARS::Coord& other) const {
      return other.x * 10000 + other.y;
    }
  
  };

}

#endif
