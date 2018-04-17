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
        Matrix<double> generate(Matrix<double> popMatrix) {
            int rows = popMatrix.numberRows();
            int cols = popMatrix.numberCols();
            Matrix<double> newMatrix(rows, cols);
            // zero out matrix to avoid NaNs
            for(int i = 0; i < rows; i++) {
                for(int j = 0; j < cols; j++) {
                    newMatrix.at(i, j) = 0;
                }
            }
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    double r = ((double) rand() / (RAND_MAX)) + 1;
                    double noise;
                    noise = perlin.noise(popMatrix.at(i,j) + r, popMatrix.at(i,j) - r);
                    if (noise > 0) {
                        newMatrix.at(i, j) = ceil(10*noise);
                    }
                }
            }
            return newMatrix;
        }
    };
}

#endif


