//
// Created by Marissa Colasacco on 4/14/18.
//

#ifndef MARS_POPULATIONGEN_H
#define MARS_POPULATIONGEN_H

#include "Matrix.h"
#include "PerlinNoise.h"

namespace MARS {

    class PopulationGen {
    private:
        siv::PerlinNoise perlin; //instance of Perlin Noise generator

    public:
        /*
         * Constructor
         */
        PopulationGen() {
            perlin = siv::PerlinNoise();
        }

        // Takes current population matrix as input
        // Returns matrix of new population to be added
        Matrix<double> genNewPop(Matrix<double> popMatrix) {
            int rows = popMatrix.numberRows();
            int cols = popMatrix.numberCols();
            Matrix<double> newPop(rows, cols);
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    double r = ((double) rand() / (RAND_MAX)) + 1;
                    double noise;
                    noise = perlin.noise(popMatrix.at(i,j) + r, popMatrix.at(i,j) - r);
                    if (noise > 0) {
                        newPop.at(i, j) = ceil(10*noise);
                    }
                }
            }
            return newPop;
        }
    };
}

#endif


