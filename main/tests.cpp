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

#include "gtest/gtest.h"

namespace {

class MarsTest : public ::testing::Test {
  protected:
  MarsTest() {
    // setup for each test
  }

  virtual ~MarsTest() {
    // teardown for each test 
  }
    
  // can declare objects here used by all tests
};

TEST_F(MarsTest, MatrixStoresData) {
  int num_rows = 32;
  int num_cols = 32;
  MARS::Matrix<int> matrix(num_rows, num_cols);
  
  for (int i = 0; i < num_rows; i++) {
    for (int j = 0; j < num_cols; j++) {
      matrix.at(i, j) = i * j;
    }
  }

  for (int i = 0; i < num_rows; i++) {
    for (int j = 0; j < num_cols; j++) {
      EXPECT_EQ(matrix.at(i, j), i * j);
    }
  }
}

TEST_F(MarsTest, PopulationGenUpdatesPop) {
  int rows = 2;
  int cols = 2;

  MARS::Matrix<double> matrix(rows, cols);

  for (int k=0; k<10; k++) {
    std::cout <<"round " << k << std::endl;
    MARS::Matrix<double> newPop = MARS::PopulationGen().genNewPop(matrix);
    for (int i=0; i<rows; i++) {
      for (int j=0; j<cols; j++ ) {
        std::cout << "new pop value at  (" << i << "," << j << "):" << newPop.at(i,j) << std::endl;
      }
    }
  }

  std::cout << "Update population test finished" << std::endl;

}


TEST_F(MarsTest, BitMatrixStoresData) {
  int rows = 32;
  int cols = 32;
  MARS::BitMatrix bitMatrix(rows, cols);

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      EXPECT_EQ(bitMatrix.get(i, j), 0);
      bitMatrix.set(i, j, true);
      EXPECT_TRUE(bitMatrix.get(i, j));
      bitMatrix.set(i, j, false);
      EXPECT_FALSE(bitMatrix.get(i, j));
    }
  }
}

TEST_F(MarsTest, TerrainGen) {
    MARS::Terrain terrain(8, 8);
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            std::cout << terrain.weightAtXY(i,j) << " ";
        }
        std::cout << std::endl;
    }
}

TEST_F(MarsTest, Coords) {
  MARS::Coord c;
  MARS::Coord c2(2, 2);

  std::vector<MARS::Coord> vectorCoord;
  vectorCoord.push_back(c2);

  std::unordered_set<MARS::Coord> setCoord;
  setCoord.insert(c2);
}

}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
