#ifndef MARS_GROWTH_PREDICTION_H
#define MARS_GROWTH_PREDICTION_H

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <utility>

#include "Matrix.h"
#include "Coord.h"
#include "Game.h"

namespace MARS {

  class GrowthPrediction {
  private:
    int time_interval;
    Matrix<int> last_pop_matrix;
    std::unordered_set<Coord> last_locs_added;
    Matrix<std::pair<double, double>> growth_gradients; 

    Game* game;
  public:
    GrowthPrediction(Game* game, int time_interval);
    void updateGradients();
  };
}

#endif