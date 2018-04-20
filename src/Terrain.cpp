#include "../include/Terrain.h"
#include <iostream>

using namespace MARS;

Terrain::Terrain(int dx, int dy): perlin(std::time(NULL)), sizeX(dx), sizeY(dy), terrainMatrix(dx, dy) {
    for (int i = 0; i < sizeX; i++) {
        for (int j = 0; j <sizeY; j++) {
            double value = perlin.noise0_1(i+0.5, j+0.5);
            if (value >= MOUNTAIN_THRESHOLD)
                terrainMatrix.at(i,j) = MOUNTAIN_WEIGHT;
            else if (value >= GRASSLAND_THRESHOLD)
                terrainMatrix.at(i,j) = GRASSLAND_WEIGHT;
            else
                terrainMatrix.at(i,j) = WATER_WEIGHT;
        }
    }
}

double Terrain::weightAtXY(int x, int y) {
    return terrainMatrix.at(x, y);
}

Terrain::Terrain(int dim): perlin(std::time(NULL)), sizeX(dim), sizeY(dim), terrainMatrix(dim, dim){
  //std::cout << "In this constructor" << std::endl;
  for (int i=0; i<dim; i++) {
    for (int j=0; j<dim; j++) {
      terrainMatrix.at(i,j) = GRASSLAND_WEIGHT;
      //std::cout << "Coordinate (" << i << "," << j << "):" << weightAtXY(i,j) << std::endl;
    }
  }
}

Terrain::Terrain(int x, int y, bool water) : perlin(std::time(NULL)), sizeX(x), sizeY(y), terrainMatrix(x,y){
  for (int i=0; i<x; i++) {
    for (int j=0; j<y; j++) {
      terrainMatrix.at(i,j) = GRASSLAND_WEIGHT;
    }
  }

  terrainMatrix.at(0,1) = WATER_WEIGHT;

  terrainMatrix.at(1,0) = WATER_WEIGHT;

  terrainMatrix.at(1,2) = WATER_WEIGHT;

  terrainMatrix.at(2,1) = WATER_WEIGHT;
}

double Terrain::weightAtCoord(Coord c) {
    return weightAtXY(c.x, c.y);
}
