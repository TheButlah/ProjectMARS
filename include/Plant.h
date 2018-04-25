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
        Coord location; //coordinates of plant in map
        int in_service; //number of people currently serviced by plant
        int capacity; //total number of people plant can service
        double serviceable_distance; //furthest distance at which the plant can service
        //std::vector<Coord> serviceableArea; //list of coordinates that plant can service
        std::unordered_map<Coord, double> serviceable_area;
        std::unordered_map<Coord, int> serviced_map; //contains mapping from coordinate to number of people being
                                                    // serviced at that coordinate
        /*
         * Constructor
         */
        Plant(
                int cap,
                double serve_dist,
                int x,
                int y,
                Terrain &terrain);

        std::unordered_map<Coord,double> generateServiceableArea(Terrain &terrain, Coord plant_loc, double serve_dist);

        std::unordered_map<Coord, int> initializeServicedMap( std::unordered_map<Coord, double> serviceable_area);


        void addToService(int pop);

        //Adds integer to THIS plant's serviced population
        //Removes integer from inputted plant's serviced population
        //if int is negative, this is equivalent to removing population from THIS plant's serviced population
        void changeService(Plant, int pop);

        void updateMapping(Plant plant, Coord person_loc, int pop);

        bool operator==(const Plant& other) const {
            return location == other.location;
        }
    };
}

namespace std {
    /*
     * Hash a Coord instance
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