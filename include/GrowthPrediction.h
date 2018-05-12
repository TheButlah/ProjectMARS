#ifndef MARS_GROWTH_PREDICTION_H
#define MARS_GROWTH_PREDICTION_H

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <utility>

<<<<<<< HEAD
#include "Game.h"
#include "Matrix.h"
#include "Coord.h"
#include "Vector.h"
=======
#include "Matrix.h"
#include "Coord.h"
#include "Game.h"
>>>>>>> 52d17ccd585c0d8c7aceb6d054e485e803f26b09

namespace MARS {

  class GrowthPrediction {
  private:
    int time_interval;
    Matrix<int> last_pop_matrix;
    std::unordered_set<Coord> last_locs_added;
    Matrix<Vector> growth_gradients; 

    Game* game;
  public:
    GrowthPrediction(Game* game, int time_interval);
    void updateGradients();
  };
}

#endif