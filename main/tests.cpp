#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <gtest/gtest.h>

#include "../include/Clustering.h"
#include "../include/Coord.h"
#include "../include/Game.h"
#include "../include/Matrix.h"
#include "../include/BitMatrix.h"
#include "../include/PopulationGen.h"
#include "../include/Terrain.h"
#include "../include/PopulationMatrix.h"
#include "../include/Plant.h"


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
        MARS::PopulationMatrix popMat = MARS::PopulationMatrix(8, 8);
        MARS::Game game = MARS::Game(8, 8, 100, 100, 50, 200, 200, 200, 1.0);
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

      MARS::Matrix<int> matrix2 = matrix;
      for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
          EXPECT_EQ(matrix2.at(i, j), i * j);
        }
      }
    };


    TEST_F(MarsTest, GenerateServiceableAreaTest1_FlatLand) {
      MARS::Terrain terrain(4);

      int cap = 10;
      double servable_dist = 4.0;
      int x = 1;
      int y = 1;
      MARS::Plant plant = MARS::Plant(cap, servable_dist, x, y, terrain);

      std::unordered_map<MARS::Coord, double> serviceableArea = plant.serviceableArea();
      EXPECT_EQ(15, serviceableArea.size());

    }


    TEST_F(MarsTest, GenServiceableAreaTest2_SomeUnreachable) {
      MARS::Terrain terrain(4);

      int cap = 10;
      double serv_dist = 1.0;
      int x = 1;
      int y = 1;
      MARS::Plant plant = MARS::Plant(cap, serv_dist, x, y, terrain);

      std::unordered_map<MARS::Coord, double> serviceableArea = plant.serviceableArea();


      EXPECT_EQ(4, serviceableArea.size());


    };

    TEST_F(MarsTest, GenServiceableAreaTest3_SomeUnreachable) {
      MARS::Terrain terrain(4);

      int cap = 10;
      double serv_dist = 2.0;
      int x = 1;
      int y = 1;
      MARS::Plant plant = MARS::Plant(cap, serv_dist, x, y, terrain);

      std::unordered_map<MARS::Coord, double> serviceableArea = plant.serviceableArea();

      EXPECT_EQ(10, serviceableArea.size());

    };

    TEST_F(MarsTest, GenServicedMap_SomeUnreachable) {
      MARS::Terrain terrain(4);

      int cap = 10;
      double serv_dist = 2.0;
      int x = 1;
      int y = 1;
      MARS::Plant plant = MARS::Plant(cap, serv_dist, x, y, terrain);

      std::unordered_map<MARS::Coord, double> serviceableArea = plant.serviceableArea();

      std::unordered_map<MARS::Coord, int> servicedMap = plant.servicedMap();

      EXPECT_EQ(10, servicedMap.size());
    }


    TEST_F(MarsTest, GenServiceableAreaTest4_SurroundedByWater) {
      MARS::Terrain terrain(4, 4, true);

      int cap = 10;
      double serv_dist = 4.0;
      int x = 1;
      int y = 1;
      MARS::Plant plant = MARS::Plant(cap, serv_dist, x, y, terrain);

      std::unordered_map<MARS::Coord, double> serviceableArea = plant.serviceableArea();

      EXPECT_EQ(0, serviceableArea.size());
    }

  
    TEST_F(MarsTest, PopulationGenUpdatesPop) {
        int rows = 2;
        int cols = 2;

        MARS::Matrix<int> matrix(rows, cols);
        MARS::PopulationGen popGenerator = MARS::PopulationGen();
        MARS::Terrain terrain = MARS::Terrain(rows,cols);

        for (int k=0; k<10; k++) {
            matrix = popGenerator.generate(matrix, terrain, 0);
            for (int i=0; i<rows; i++) {
                for (int j=0; j<cols; j++ ) {
                    EXPECT_FALSE(std::isnan(matrix.at(i,j)));
                }
            }
        }
        std::cout << "before" << std::endl;
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
          EXPECT_FALSE(std::isnan(terrain.weightAtXY(i, j)));
        }
      }
    }

    TEST_F(MarsTest, Coords) {
      MARS::Coord c1(2, 2);
      MARS::Coord c2(2, 2);

      EXPECT_EQ(c1, c2);
    }

    TEST_F(MarsTest, PopulationMatrixServicedByPlant) {
      // sum up all plants, check that it's equal to total num serviced
      MARS::Terrain terrain(8, 8);
      MARS::Plant p1(100, 5, 2, 3, terrain);
      MARS::Plant p2(100, 3, 4, 4, terrain);
      MARS::Coord c(2, 3);

      popMat.assignUnservicedPop(&p1, c, 5);
      popMat.assignUnservicedPop(&p2, c, 10);

      EXPECT_EQ(
              popMat.numberServicedAtCoord(c),
              popMat.numberServicedAtCoordByPlant(c, &p1) + popMat.numberServicedAtCoordByPlant(c, &p2)
      );
    }

    TEST_F(MarsTest, MovePopNonZero) {
      // move x from one plant to another
      // check that population for plant 'from' has decreased by x
      // and that population for plant 'to' has increased by x
      MARS::Terrain terrain(8, 8);
      MARS::Plant p1(100, 5, 2, 3, terrain);
      MARS::Plant p2(100, 3, 4, 4, terrain);
      MARS::Coord c(2, 3);

      popMat.assignUnservicedPop(&p1, c, 5);
      popMat.assignUnservicedPop(&p2, c, 10);

      int oldPop1 = popMat.numberServicedAtCoordByPlant(c, &p1);
      int oldPop2 = popMat.numberServicedAtCoordByPlant(c, &p2);

      popMat.moveServicedPopBetweenPlants(&p2, &p1, c, 5);

      int newPop1 = popMat.numberServicedAtCoordByPlant(c, &p1);
      int newPop2 = popMat.numberServicedAtCoordByPlant(c, &p2);

      EXPECT_EQ(
              newPop1, oldPop1 + 5
      );
      EXPECT_EQ(
              newPop2, oldPop2 - 5
      );
    }


    TEST_F(MarsTest, InitializationTest) {
      MARS::Game game2 = MARS::Game(8, 8, 1, 100, 50, 200, 200, 200, 1.0);
      EXPECT_EQ(game2.getNumberPlantsInService(), 0);
    }
    // Q: are NULLs the best way to pass in nonexistent coords?


    TEST_F(MarsTest, StepNoPlant) {
      // Stepping without a plant added
      MARS::Game game1 = MARS::Game(8, 8, 1, 100, 50, 200, 200, 200, 1.0);
      game1.step(false, MARS::Coord(1, 1));
      EXPECT_EQ(game1.getNumberPlantsInService(), 0);
    }


    TEST_F(MarsTest, StepWithPlant) {
      // Stepping with a plant added
      game.step(true, MARS::Coord(3, 4));
      EXPECT_NE(game.getNumberPlantsInService(), 0);
    }


    /* Q: what is in the first element of the pair when there is no plant? (cannot be NULL as NULL is numeric) */

    // Commenting this test out for now

     TEST_F(MarsTest, FindBestPlantNoPlants) {
      // No best plant if there are no plants
      MARS::Coord arbitrary_coord(3, 4);
      //std::pair<MARS::Plant, bool> expected_result = std::pair<MARS::Plant, bool>(NULL, false);
      EXPECT_EQ(
          game.findBestPlant(arbitrary_coord).second, false
      );
    }



    TEST_F(MarsTest, FindBestPlant) {
      for (int i = 0; i < 100; i++) {
        game.step(false, MARS::Coord(1, 1));
      }

      for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
          std::cout << game.getPopulationAt(i, j) << " ";
        }
        std::cout << std::endl;
      }
      game.step(true, MARS::Coord(5, 5));
      // after 100 steps, check to see if there's a "best plant" for some coord
      bool foundPlant = false;
      for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
          foundPlant = foundPlant || game.findBestPlant(MARS::Coord(i, j)).second;
        }
      }
      EXPECT_TRUE(foundPlant);
    }

    TEST_F(MarsTest, BuildPlantOnAnother) {
      game.step(true, MARS::Coord(1,1));
      game.step(true, MARS::Coord(1,1));
      game.step(true, MARS::Coord(1,1));
      game.step(true, MARS::Coord(1,1));
      EXPECT_EQ(game.getNumberPlantsInService(),1);
    }



  TEST_F(MarsTest, UnservicedPopulationProcessed) {
    // check to see that number in service goes up over time
    for(int i = 0; i < 15; i++) {
        if(i % 5 == 0) {
            game.step(true, MARS::Coord(i/5, i/5));
        }
        else {
            game.step(false, MARS::Coord(0, 0));
        }
    }
    EXPECT_NE(game.getNumberPopServiced(), 0);
  }



  TEST_F(MarsTest, ObjectiveFunctionValid) {
    for(int i = 0; i < 50; i++) {
        if(i % 5 == 0) {
            game.step(true, MARS::Coord(i/5, i/5));
        }
        else {
            game.step(false, MARS::Coord(0, 0));
        }
    }
    EXPECT_FALSE(std::isnan(game.calculateObjective()));
    EXPECT_NE(game.calculateObjective(), 0);
  }


}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
