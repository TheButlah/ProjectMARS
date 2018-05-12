#include "GrowthPrediction.h"

using namespace MARS;

GrowthPrediction::GrowthPrediction(Game *game, int time_interval): last_pop_matrix(0, 0) {
  this->game = game;
  this->time_interval = time_interval;
}

void GrowthPrediction::updateGradients() {
  int t = game->currentTime();
  if (t == 0)
    return;

  Matrix<int> pop_density_matrix = game->popMatrixCopy.totalPopMatrix();

  if (t == time_interval) {
    last_pop_matrix = pop_density_matrix;
    std::unordered_set<Coord> first_pop;
    for (int i = 0; i < last_pop_matrix.numberRows(); i++) {
      for (int j = 0; j < last_pop_matrix.numberCols(); j++) {
        if (last_pop_matrix.at(i, j) > 0) {
          first_pop.push_back(Coord(i, j));
        }
      }
    }
    last_locs_added = first_pop;
    return;
  }

  std::unordered_set<Coord> new_pop;
  for (int i = 0; i < pop_density_matrix.numberRows(); i++) {
    for (int j = 0; j < pop_density_matrix.numberCols(); j++) {
      for (pop_density_matrix.at(i, j) > 0 && last_pop_matrix.at(i, j) == 0) {
        new_pop.push_back(Coord(i, j));
      }
    }
  }
}