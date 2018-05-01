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
    std::cout << "yyyy" << std::endl;
    BitMatrix visited = BitMatrix(terrain.sizeX(), terrain.sizeY());

    std::queue<std::tuple<Coord, double>> queue;
    std::unordered_map<Coord, double> serviceable;
    queue.push(std::make_tuple(plantLoc, 0.0));
    visited.set(plantLoc.x, plantLoc.y, true);

    std::cout << "yyyy" << std::endl;
    while (queue.size() > 0) {
        std::tuple<Coord, double> locInfo = queue.front();
        double weightedDist = std::get<1>(locInfo);
        Coord loc = std::get<0>(locInfo);
        queue.pop();

        std::cout << "yyyy" << std::endl;
        std::vector<Coord> neighbors {
            Coord(loc.x, loc.y-1), 
            Coord(loc.x, loc.y+1), 
            Coord(loc.x-1, loc.y), 
            Coord(loc.x+1, loc.y)
        };
        std::cout << "yyyy" << std::endl;
        for (int i = 0; i < neighbors.size(); i++) {
            Coord neighbor = neighbors[i];
            if (!(neighbor.x >= 0 && neighbor.y >= 0 
            && neighbor.x < terrain.sizeX() && neighbor.y < terrain.sizeY()))
                continue;
            if (!visited.get(neighbor.x, neighbor.y)) { // has not been visited
                double terrainWeight = terrain.weightAtXY(neighbor.x, neighbor.y);
                if (weightedDist+terrainWeight <= serve_dist ) { // within service
                    std::tuple<Coord, double> newLoc = std::make_tuple(neighbor, weightedDist+terrainWeight);
                    serviceable[neighbor] =  weightedDist+terrainWeight;
                    queue.push(newLoc);
                }
            }
            visited.set(neighbor.x, neighbor.y, true);
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


bool Plant::isServiceableCoord(Coord c) {
    std::unordered_map<Coord, double>::const_iterator iter = this->serviceable_area.find(c);
    if (iter == this->serviceable_area.end()) {
        return false;
    }
    return true;
}

double Plant::distanceToCoord(Coord c) {
    return serviceable_area[c];
}

int Plant::remainingCapacity() {
    return capacity - in_service;
}

int Plant::numberServicedAtCoord(Coord c) {
    return serviced_map[c];
}

void Plant::changeServicedPop(Coord person_loc, int pop) {
    this->serviced_map[person_loc] += pop;
    this->in_service += pop;
}

std::unordered_map<Coord, double> Plant::serviceableArea() {
    return serviceable_area;
}

std::unordered_map<Coord, int> Plant::servicedMap() {
    return serviced_map;
}