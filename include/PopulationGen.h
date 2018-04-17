//
// Created by Marissa Colasacco on 4/14/18.
//

#ifndef MARS_POPULATIONGEN_H
#define MARS_POPULATIONGEN_H

#include <ctime>
#include "Matrix.h"
#include "PerlinNoise.h"

namespace MARS {

    class PopulationGen {
    private:
        siv::PerlinNoise perlin; // instance of Perlin Noise generator

    public:
        /*
         * Constructor
         */
        PopulationGen() {
            perlin = siv::PerlinNoise(time(NULL));
        }

        /* Takes current population matrix as input
         * Returns matrix of new population to be added
         */
        Matrix<double> generate(Matrix<double> popMatrix);
    };
}

#endif


