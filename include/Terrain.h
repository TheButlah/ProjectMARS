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
    public:
        siv::PerlinNoise perlin;
        Matrix<double> terrainMatrix;
        int sizeX;
        int sizeY;

        Terrain(int dx, int dy): perlin(std::time(NULL)), sizeX(dx), sizeY(dy), terrainMatrix(dx, dy) {
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

        double weightAtXY(int x, int y) {
            return terrainMatrix.at(x, y);
        }

        double weightAtCoord(Coord c) {
            return weightAtXY(c.x, c.y);
        }
    };
}

#endif //MARS_TERRAIN_H
