//
// Created by Marissa Colasacco on 4/15/18.
//

#ifndef MARS_PLANT_H
#define MARS_PLANT_H
#include "Coord.h"
#include "Terrain.h"
#include <list>
#include <queue>

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
        Matrix<std::tuple<Coord, int>> servicedMap; //(coord, int) pairs where int = # people serviced at coord

        /*
         * Constructor
         */
        Plant(int cap, double serve_dist, int x, int y, Terrain terrain) {
            capacity = cap;
            serviceable_distance = serve_dist;
            location = Coord(x,y);
            inService = 0;
            serviceableArea = generateServiceableArea(terrain, location, serve_dist);
            servicedMap = Matrix<std::tuple<Coord, int>>(x, y);
        }

        std::vector<Coord> generateServiceableArea(Terrain terrain, Coord plantLoc, double serve_dist) {
            BitMatrix visited = BitMatrix(terrain.sizeX,terrain.sizeY);
            std::queue<std::tuple<Coord, double>> queue;
            std::vector<Coord> serviceable;
            queue.push(std::make_tuple(plantLoc, 0));
            visited.set(plantLoc.x, plantLoc.y, 1);
            while (queue.size() > 0) {
                std::tuple<Coord, double> locInfo = queue.front();
                Coord loc = std::get<0>(locInfo);
                double weightedDist = std::get<1>(locInfo);
                queue.pop();
                if (loc.y - 1 >= 0) { //within bounds
                    if (visited.get(loc.x, loc.y-1) ==0) { //has not been visited
                        double terrainWeight = terrain.weightAtXY(loc.x, loc.y-1);
                        if (weightedDist+terrainWeight <= serve_dist ) { //within service
                            std::tuple<Coord, double> newLoc;
                            newLoc = std::make_tuple(Coord(loc.x, loc.y-1), weightedDist+terrainWeight);
                            serviceable.push_back(Coord(loc.x, loc.y-1));
                            queue.push(newLoc);
                            visited.set(loc.x, loc.y-1,1);
                        }
                    }
                }
                if (loc.y+1 < terrain.sizeY) {
                    if (visited.get(loc.x, loc.y+1) ==0) {
                        double terrainWeight = terrain.weightAtXY(loc.x, loc.y+1);
                        if (weightedDist + terrainWeight <= serve_dist) {
                            std::tuple<Coord, double> newLoc;
                            newLoc = std::make_tuple(Coord(loc.x, loc.y+1), weightedDist+terrainWeight);
                            serviceable.push_back(Coord(loc.x, loc.y+1));
                            queue.push(newLoc);
                            visited.set(loc.x, loc.y+1, 1);
                        }
                    }
                }
                if (loc.x-1 >= 0) {
                    if (visited.get(loc.x-1, loc.y) == 0) {
                        double terrainWeight = terrain.weightAtXY(loc.x-1, loc.y);
                        if (weightedDist + terrainWeight <= serve_dist) {
                            std::tuple<Coord, double> newLoc;
                            newLoc = std::make_tuple(Coord(loc.x-1, loc.y), weightedDist+terrainWeight);
                            serviceable.push_back(Coord(loc.x-1, loc.y));
                            queue.push(newLoc);
                            visited.set(loc.x-1, loc.y, 1);
                        }
                    }
                }
                if (loc.x+1 < terrain.sizeX) {
                    if (visited.get(loc.x+1, loc.y) ==0) {
                        double terrainWeight = terrain.weightAtXY(loc.x+1, loc.y);
                        if (weightedDist + terrainWeight <= serve_dist) {
                            std::tuple<Coord, double> newLoc;
                            newLoc = std::make_tuple(Coord(loc.x+1, loc.y), weightedDist+terrainWeight);
                            serviceable.push_back(Coord(loc.x+1, loc.y));
                            queue.push(newLoc);
                            visited.set(loc.x+1, loc.y, 1);
                        }
                    }
                }
            }
            return serviceable;
        }
    };
}

#endif //MARS_PLANT_H
