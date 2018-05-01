#include "../include/Plant.h"
#include <queue>
#include <iostream>


using namespace MARS;


Plant::Plant(
    int cap,
    double serve_dist,
    int x,
    int y,
    Terrain &terrain
) :
    capacity(cap),
    serviceable_distance(serve_dist),
    location(Coord(x,y)),
    in_service(0),
    serviceable_area(generateServiceableArea(terrain, location, serve_dist)),
    serviced_map(initializeServicedMap(serviceable_area))
{

}

std::unordered_map<Coord,double> Plant::generateServiceableArea(Terrain &terrain, Coord plantLoc, double serve_dist) {
    BitMatrix visited = BitMatrix(terrain.sizeX(), terrain.sizeY());
    for (int i=0; i<terrain.sizeX(); i++) {
        for (int j=0; j<terrain.sizeY(); j++) {
            visited.set(i, j, 0);
        }
    }

    std::queue<std::tuple<Coord, double>> queue;
    std::unordered_map<Coord, double> serviceable;
    queue.push(std::make_tuple(plantLoc, 0.0));
    visited.set(plantLoc.x, plantLoc.y, 1);

    while (queue.size() > 0) {
        std::tuple<Coord, double> locInfo = queue.front();
        double weightedDist = std::get<1>(locInfo);
        Coord loc = std::get<0>(locInfo);
        queue.pop();
        
        std::vector<Coord> neighbors {
            Coord(loc.x, loc.y-1), 
            Coord(loc.x, loc.y+1), 
            Coord(loc.x-1, loc.y), 
            Coord(loc.x+1, loc.y)
        };
        
        for (Coord neighbor : neighbors) {
            if (!(neighbor.x >= 0 && neighbor.y >= 0 
            && neighbor.x < terrain.sizeX() && neighbor.y < terrain.sizeY()))
                continue;
            if (visited.get(neighbor.x, neighbor.y) == 0) { // has not been visited
                double terrainWeight = terrain.weightAtXY(neighbor.x, neighbor.y);
                if (weightedDist+terrainWeight <= serve_dist ) { // within service
                    std::tuple<Coord, double> newLoc = std::make_tuple(neighbor, weightedDist+terrainWeight);
                    serviceable[neighbor] =  weightedDist+terrainWeight;
                    queue.push(newLoc);
                    visited.set(neighbor.x, neighbor.y, 1);
                }
            }
        }

    }


    return serviceable;
}

std::unordered_map<Coord, int> Plant::initializeServicedMap(std::unordered_map<Coord, double> serviceable_area) {
    std::unordered_map<Coord, int> serviced_map;
    for (std::pair<Coord, double> element : serviceable_area) {
        Coord coord = element.first;
        serviced_map[coord] = 0;
    }
    return serviced_map;
}

void Plant::changeServicedPop(Coord person_loc, int pop) {
    this->serviced_map[person_loc] += pop;
    this->in_service += pop;
}