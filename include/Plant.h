//
// Created by Marissa Colasacco on 4/15/18.
//

#ifndef MARS_PLANT_H
#define MARS_PLANT_H
#include "Coord.h"
#include "Terrain.h"
#include "BitMatrix.h"
#include <unordered_map>


namespace MARS {
    /*
     * Abstraction of plant
     */
    class Plant {
    public:
        /*
         * location: coordinates of plant in map
         * in_service: number of people currently serviced by plant
         * capacity: total number of people plant can service
         * serviceable_distance: furthest distance which the plant can service
         * serviceable_area: coordinates that a plant can service
         * serviced_map: mapping from a coordinate to number of people being serviced there
         */
        Coord location;
        int in_service; 
        int capacity;
        double serviceable_distance; 
        std::unordered_map<Coord, double> serviceable_area; 
        std::unordered_map<Coord, int> serviced_map; 

        /*
         * Constructor
         */
        Plant(
                int cap,
                double serve_dist,
                int x,
                int y,
                Terrain &terrain);

        /*
         * Generate the plant's serviceable area given a terrain, location, and serviceable distance.
         */
        std::unordered_map<Coord,double> generateServiceableArea(Terrain &terrain, Coord plant_loc, double serve_dist);

        /*
         * Initialize the serviced map using the plant's serviceable area.
         */
        std::unordered_map<Coord, int> initializeServicedMap( std::unordered_map<Coord, double> serviceable_area);


        /*
         * Adds `pop' to THIS plant's serviced population.
         */
        void addToService(int pop);

        /* Adds `pop' to THIS plant's serviced population and removes `pop' from the
         * other plant's serviced population.
         * If `pop' is negative, this is equivalent to removing population from THIS plant's 
         * serviced population and adding it to the other.
         */
        void changeService(Plant, int pop);

        /*
         * Adds a population at a given location to the plant's serviced map.
         */
        void updateMapping(Plant plant, Coord person_loc, int pop);

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
