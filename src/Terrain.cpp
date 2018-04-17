#include "../include/Terrain.h"

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
