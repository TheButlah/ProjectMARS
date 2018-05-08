#include "../include/Matrix.h"
#include "../include/PopulationGen.h"

#include <cmath>

using namespace MARS;

#define CURR_THRESH_INC 0.02
#define POP_MAX 50

Matrix<int> PopulationGen::generate(Matrix<int> popMatrix, Terrain terrain, int t) {
    int rows = popMatrix.numberRows();
    int cols = popMatrix.numberCols();
    Matrix<int> newMatrix(rows, cols);
    
    if (t % 10 != 0) return newMatrix;
    curr_thresh += CURR_THRESH_INC;
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (terrain.weightAtXY(i,j) == WATER_WEIGHT or terrain.weightAtXY(i,j) == MOUNTAIN_WEIGHT) {
                newMatrix.at(i,j) = 0;
            } else {
                double noise = perlin.noise0_1(i/std::log2(rows), j/std::log2(cols));
                if (noise <= curr_thresh) {
                    newMatrix.at(i,j) = (int) ((POP_MAX*(curr_thresh-noise)) - popMatrix.at(i, j));
                }
            }
        }
    }
    return newMatrix;
}