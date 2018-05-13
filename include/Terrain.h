//
// Created by Marissa Colasacco on 4/15/18.
//

#ifndef MARS_TERRAIN_H
#define MARS_TERRAIN_H

#include <ctime>
#include <limits>

#include "PerlinNoise.h"
#include "Matrix.h"
#include "Coord.h"

#define GRASSLAND_THRESHOLD 0.3
#define MOUNTAIN_THRESHOLD 0.7
#define WATER_WEIGHT std::numeric_limits<double>::max()
#define GRASSLAND_WEIGHT 1.0
#define MOUNTAIN_WEIGHT 100.0

namespace MARS {

  class Terrain {
  private:
    siv::PerlinNoise perlin;
    Matrix<float> terrainMatrix;
    int size_x;
    int size_y;
  public:
    

    Terrain(int dx, int dy);
    Terrain(int dim);
    Terrain(int dx, int dy, bool water);

    Matrix<float> getMatrixCopy() const;

    int sizeX() const;
    int sizeY() const;
    float weightAtXY(int x, int y) const;
    float weightAtCoord(const Coord& c) const;
  };
}

#endif //MARS_TERRAIN_H
