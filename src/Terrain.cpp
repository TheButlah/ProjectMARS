#include "Terrain.h"

#include <iostream>
#include <cmath>

using namespace MARS;

Terrain::Terrain(int dx, int dy): perlin(std::time(NULL)), size_x(dx), size_y(dy), terrainMatrix(dx, dy), weightMatrix(dx, dy) {
  for (int i = 0; i < size_x; i++) {
    for (int j = 0; j < size_y; j++) {
      float value = perlin.noise0_1(i/std::log2(size_x), j/std::log2(size_x));
      if (value >= MOUNTAIN_THRESHOLD) {
        terrainMatrix.at(i, j) = 1;
        weightMatrix.at(i, j) = MOUNTAIN_WEIGHT;
      } else if (value >= GRASSLAND_THRESHOLD) {
        terrainMatrix.at(i, j) = 0;
        weightMatrix.at(i, j) = GRASSLAND_WEIGHT;
      } else {
        terrainMatrix.at(i, j) = 2;
        weightMatrix.at(i, j) = WATER_WEIGHT;
      }
    }
  }
}

Terrain::Terrain(int dim) :
  perlin(std::time(NULL)),
  size_x(dim),
  size_y(dim),
  terrainMatrix(dim, dim),
  weightMatrix(dim, dim)
{
  //std::cout << "In this constructor" << std::endl;
  for (int i=0; i<dim; i++) {
    for (int j=0; j<dim; j++) {
      weightMatrix.at(i,j) = GRASSLAND_WEIGHT;
    //std::cout << "Coordinate (" << i << "," << j << "):" << weightAtXY(i,j) << std::endl;
    }
  }
}

Terrain::Terrain(int x, int y, bool water) :
  perlin(std::time(NULL)),
  size_x(x),
  size_y(y),
  terrainMatrix(x,y),
  weightMatrix(x, y)
{
  for (int i=0; i<x; i++) {
    for (int j=0; j<y; j++) {
      weightMatrix.at(i,j) = GRASSLAND_WEIGHT;
    }
  }

  weightMatrix.at(0,1) = WATER_WEIGHT;
  weightMatrix.at(1,0) = WATER_WEIGHT;
  weightMatrix.at(1,2) = WATER_WEIGHT;
  weightMatrix.at(2,1) = WATER_WEIGHT;
}

int Terrain::sizeX() const {
  return size_x;
}

int Terrain::sizeY() const {
  return size_y;
}

float Terrain::weightAtXY(int x, int y) const {
  return weightMatrix.at(x, y);
}

float Terrain::weightAtCoord(const Coord& c) const {
  return weightAtXY(c.x, c.y);
}

Matrix<float> Terrain::getMatrixCopy() const {
  return weightMatrix;
}

Matrix<int> Terrain::getTerrainMatrix() const {
  return terrainMatrix;
}