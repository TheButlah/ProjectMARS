#ifndef MARS_GROWTHPREDICTION_H
#define MARS_GROWTHPREDICTION_H

#include <vector>
#include <unordered_set>
#include <utility>

#include "Game.h"
#include "Coord.h"
#include "Matrix.h"
#include "BitMatrix.h"

namespace MARS {
  class GrowthPrediction {
  private:
    Game* game;
    int avg_cover;
    std::unordered_map<Coord, std::vector<Coord>> bin_assign;

    // Previous game state
    Matrix<int> last_pop_matrix;
    int last_diff;
    int last_diff_slope;
    
    int binXY();
    Coord nearestValidCoord(Coord c, Terrain& terrain);
    Coord plantLocationInBin(Coord bin);
    std::unordered_set<Coord> unservicedCoords(bool old);
  public:
    GrowthPrediction(Game* game, int sample_size);

    std::vector<Coord> predictNewPlants();
    void updateStateRecord();
  };
}

#endif