#include "../include/Matrix.h"
#include "../include/PopulationGen.h"


using namespace MARS;

Matrix<double> PopulationGen::generate(Matrix<double> popMatrix) {
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