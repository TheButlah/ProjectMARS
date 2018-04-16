//
// Created by Marissa Colasacco on 4/15/18.
//

#ifndef MARS_PLANT_H
#define MARS_PLANT_H
#include "Coord.h"
#include "Terrain.h"

namespace MARS{
    /*
     * Abstraction of plant
     */
    class Plant{
    public:
        Coord location; //coordinates of plant in map
        int inService; //number of people currently serviced by plant
        int capacity; //total number of people plant can service
        double serviceable_distance; //furthest distance at which the plant can service
        std::vector<Coord> serviceableArea; //list of coordinates that plant can service
        std::tuple<Coord, int> servicedMap; //(coord, int) pairs where int = # people serviced at coord

        /*
         * Constructor
         */
        Plant(int cap, double serve_dist, int x, int y) {
            capacity = cap;
            serviceable_distance = serv_dist;
            location = Coord(x,y);
            //generate serviceable area
        }

        std::vector<Coord> genServiceableArea(Terrain terrain) {
            //dependent on terrain, flood fill algorithm?
        };


    };
}

#endif //MARS_PLANT_H
