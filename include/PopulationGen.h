//
// Created by Marissa Colasacco on 4/14/18.
//

#ifndef MARS_POPULATIONGEN_H
#define MARS_POPULATIONGEN_H

#include <ctime>
#include "Matrix.h"
#include "PerlinNoise.h"
#include "Terrain.h"

namespace MARS {

    class PopulationGen {
    private:
        siv::PerlinNoise perlin; // instance of Perlin Noise generator
        double curr_thresh;
    public:
        /*
         * Constructor
         */
        PopulationGen() {
            perlin = siv::PerlinNoise(time(NULL));
            curr_thresh = GRASSLAND_THRESHOLD;
        }

        /* Takes current population matrix as input
         * Returns matrix of new population to be added
         */
        Matrix<int> generate(Matrix<int> popMatrix, Terrain terrain, int t);
    };
}

#endif


