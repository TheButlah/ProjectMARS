#ifndef MARS_PLANT_H
#define MARS_PLANT_H
#include "Coord.h"
#include "Terrain.h"
#include "BitMatrix.h"
#include <unordered_map>

namespace MARS {
  /**
   * Abstraction of plant
   */
  class Plant {
  private:
    int in_service; 
    int capacity;
    double serviceable_distance;
    std::unordered_map<Coord, double> serviceable_area; 
    std::unordered_map<Coord, int> serviced_map;
  public:
    Coord location;        
    
    /**
     * Constructor
     */
    Plant(
      int cap,
      double serve_dist,
      int x,
      int y,
      Terrain &terrain
    );

    /**
     * Generate the plant's serviceable area given a terrain, location, and serviceable distance.
     */
    std::unordered_map<Coord,double> generateServiceableArea(const Terrain& terrain, const Coord& plant_loc, double serve_dist);

    /**
     * Initialize the serviced map using the plant's serviceable area.
     */
    std::unordered_map<Coord, int> initializeServicedMap(std::unordered_map<Coord, double> serviceable_area);

    /**
     * Check if coordinate is serviceable by plant
     */
    bool isServiceableCoord(const Coord& c) const;

    /**
     * Get distance to coord, assuming it is serviceable by the plant
     */
    double distanceToCoord(const Coord& c) const;
    
    /**
     * Access the number of people this plant services at a coordinate, assuming
     * it is serviceable
     */
    int numberServicedAtCoord(const Coord& c) const;

    /**
     * Changes the size of population the plant services at a given location.
     */
    void changeServicedPop(const Coord& person_loc, int pop);

    /**
     * Access the remaining capacity of the plant
     */
    int remainingCapacity() const;

    /**
     * Get a copy of the serviceable area
     */
    std::unordered_map<Coord, double> serviceableArea() const;

    /**
     * Get a copy of the serviced map
     */
    std::unordered_map<Coord, int> servicedMap() const;

    bool operator==(const Plant& other) const {
      return location == other.location;
    }
  };
}

namespace std {
  /*
   * Hash a Plant instance
   */
  template <>
  struct hash<MARS::Plant> {
  
    std::size_t operator()(const MARS::Plant& other) const {
      std::hash<MARS::Coord> coordHash;
      return coordHash(other.location);
    }
  
  };

}
#endif
