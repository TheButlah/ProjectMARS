#include <iostream>
#include <cstdlib>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "../include/Coord.h"
#include "../include/Matrix.h"
#include "../include/BitMatrix.h"
#include "../include/PopulationGen.h"
#include "../include/Terrain.h"

using namespace MARS;

void matrixTest() {
  int num_rows = 32;
  int num_cols = 32;
  Matrix<int> matrix(num_rows, num_cols);
  
  for (int i = 0; i < num_rows; i++) {
    for (int j = 0; j < num_cols; j++) {
      matrix.at(i, j) = i * j;
    }
  }

  for (int i = 0; i < num_rows; i++) {
    for (int j = 0; j < num_cols; j++) {
      if (matrix.at(i, j) != i * j) {
        std::cout << "Matrix test failed at index " << i << " " << j << std::endl;
        std::exit(1);
      }
    }
  }

  std::cout << "Matrix test successful" << std::endl;
}

void updatePopTest() {
  int rows = 2;
  int cols = 2;

  Matrix<double> matrix(rows, cols);

  for (int k=0; k<10; k++) {
    std::cout <<"round " << k << std::endl;
    Matrix<double> newPop = PopulationGen().genNewPop(matrix);
    for (int i=0; i<rows; i++) {
      for (int j=0; j<cols; j++ ) {
        std::cout << "new pop value at  (" << i << "," << j << "):" << newPop.at(i,j) << std::endl;
      }
    }
  }

  std::cout << "Update population test finished" << std::endl;

}


void bitMatrixTest() {
  int rows = 32;
  int cols = 32;
  BitMatrix bitMatrix(rows, cols);

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (bitMatrix.get(i, j)) {
        std::cout << "BitMatrix test default to zero failed at index " << i << " " << j << std::endl;
      }
      bitMatrix.set(i, j, true);
      if (!bitMatrix.get(i, j)) {
        std::cout << "BitMatrix test set to true failed at index " << i << " " << j << std::endl;
      }
      bitMatrix.set(i, j, false);
      if (bitMatrix.get(i, j)) {
        std::cout << "BitMatrix test set back to false failed at index " << i << " " << j << std::endl;
      }
    }
  }

  std::cout << "BitMatrix test successful" << std::endl;
}

void terrainTest() {
    Terrain terrain(8, 8);
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            std::cout << terrain.weightAtXY(i,j) << " ";
        }
        std::cout << std::endl;
    }
}

void coordTest() {
  Coord c;
  Coord c2(2, 2);

  std::vector<Coord> vectorCoord;
  vectorCoord.push_back(c2);

  std::unordered_set<Coord> setCoord;
  setCoord.insert(c2);
}

int main() {
  matrixTest();
  bitMatrixTest();
  updatePopTest();
    terrainTest();
}
