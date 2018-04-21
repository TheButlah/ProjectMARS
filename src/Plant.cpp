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
        inService(0),
        serviceableArea(generateServiceableArea(terrain, location, serve_dist)),
        servicedMap(initializeServicedMap(serviceableArea))
{

}


std::unordered_map<Coord, int> Plant::initializeServicedMap(std::unordered_map<Coord, double> serviceableArea) {
  std::unordered_map<Coord, int> servicedMap;
  for (std::pair<Coord, double> element : serviceableArea) {
    Coord coord = element.first;
    servicedMap[coord] = 0;
  }
  return servicedMap;
}


std::unordered_map<Coord,double> Plant::generateServiceableArea(Terrain &terrain, Coord plantLoc, double serve_dist) {
  BitMatrix visited = BitMatrix(terrain.sizeX,terrain.sizeY);
  for (int i=0; i<terrain.sizeX; i++) {
    for (int j=0; j<terrain.sizeY; j++) {
      visited.set(i,j,0);
    }
  }

 /* std::cout << "Terrain weight matrix: " << std::endl;

  for (int i=0; i<terrain.sizeY; i++) {
    for (int j=0; j<terrain.sizeX; j++) {
      std::cout << "Coordinate (" << j << "," << i << "):" << terrain.weightAtXY(j, i) << std::endl;
    }
    std::cout << std::endl;
  }*/

  std::queue<std::tuple<Coord, double>> queue;
  std::unordered_map<Coord, double> serviceable;
  queue.push(std::make_tuple(plantLoc, 0.0));
  visited.set(plantLoc.x, plantLoc.y, 1);
  while (queue.size() > 0) {
    std::tuple<Coord, double> locInfo = queue.front();
    double weightedDist = std::get<1>(locInfo);
    Coord loc = std::get<0>(locInfo);
    //std::cout << "Coordinate in consideration: (" << loc.x << "," << loc.y << ")" << std::endl;
    //std::cout << "weighted distance of this coordinate to plant = " << weightedDist << std::endl;
    queue.pop();
    if (loc.y - 1 >= 0) { //within bounds
      //std::cout << "Considering coordinate above, (" << loc.x << "," << loc.y-1 << ")" << std::endl;
      if (visited.get(loc.x, loc.y-1) ==0) { //has not been visited
        double terrainWeight = terrain.weightAtXY(loc.x, loc.y-1);
        if (weightedDist+terrainWeight <= serve_dist ) { //within service
          std::tuple<Coord, double> newLoc = std::make_tuple(Coord(loc.x, loc.y-1), weightedDist+terrainWeight);
          serviceable[Coord(loc.x, loc.y-1)] =  weightedDist+terrainWeight;
          queue.push(newLoc);
          visited.set(loc.x, loc.y-1,1);
          //std::cout << "Added to serviceable area." << std::endl;

        }
      }
    }
    if (loc.y+1 < terrain.sizeY) {
      //std::cout << "Considering coordinate below, (" << loc.x << "," << loc.y+1 << ")" << std::endl;
      if (visited.get(loc.x, loc.y+1) ==0) {
        double terrainWeight = terrain.weightAtXY(loc.x, loc.y+1);
        if (weightedDist + terrainWeight <= serve_dist) {
          std::tuple<Coord, double> newLoc = std::make_tuple(Coord(loc.x, loc.y+1), weightedDist+terrainWeight);
          serviceable[Coord(loc.x, loc.y+1)] =  weightedDist+terrainWeight;
          queue.push(newLoc);
          visited.set(loc.x, loc.y+1, 1);
          //std::cout << "Added to serviceable area." << std::endl;
        }
      }
    }
    if (loc.x-1 >= 0) {
      //std::cout << "Considering coordinate left, (" << loc.x-1 << "," << loc.y << ")" << std::endl;
      if (visited.get(loc.x-1, loc.y) == 0) {
        double terrainWeight = terrain.weightAtXY(loc.x-1, loc.y);
        if (weightedDist + terrainWeight <= serve_dist) {
          std::tuple<Coord, double> newLoc = std::make_tuple(Coord(loc.x-1, loc.y), weightedDist+terrainWeight);
          serviceable[Coord(loc.x-1, loc.y)] =  weightedDist+terrainWeight;
          queue.push(newLoc);
          visited.set(loc.x-1, loc.y, 1);
          //std::cout << "Added to serviceable area." << std::endl;
        }
      }
    }
    if (loc.x+1 < terrain.sizeX) {
      //std::cout << "Considering coordinate right, (" << loc.x+1 << "," << loc.y << ")" << std::endl;
      if (visited.get(loc.x+1, loc.y) ==0) {
        double terrainWeight = terrain.weightAtXY(loc.x+1, loc.y);
        //std::cout << "Terrain weight = " << terrainWeight << std::endl;
        if (weightedDist + terrainWeight <= serve_dist) {
          std::tuple<Coord, double> newLoc = std::make_tuple(Coord(loc.x+1, loc.y), weightedDist+terrainWeight);
          serviceable[Coord(loc.x+1, loc.y)] =  weightedDist+terrainWeight;
          queue.push(newLoc);
          visited.set(loc.x+1, loc.y, 1);
          //std::cout << "Added to serviceable area." << std::endl;
        }
      }
    }
  }


  return serviceable;
}


