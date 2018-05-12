#include "../include/GrowthPrediction.h"

#include <cmath>

using namespace MARS;

GrowthPrediction::GrowthPrediction(Game *game, int time_interval): last_pop_matrix(0, 0), growth_gradients(0, 0) {
  this->game = game;
  this->time_interval = time_interval;
  std::pair<int, int> size = game->sizeXY();
  growth_gradients = Matrix<Vector>(size.first, size.second);
}

void GrowthPrediction::updateGradients() {
  int t = game->currentTime();
  if (t == 0)
    return;

  Matrix<int> pop_density_matrix = game->popMatrixCopy().totalPopMatrix();

  if (t == time_interval) {
    last_pop_matrix = pop_density_matrix;
    std::unordered_set<Coord> first_pop;
    for (int i = 0; i < last_pop_matrix.numberRows(); i++) {
      for (int j = 0; j < last_pop_matrix.numberCols(); j++) {
        if (last_pop_matrix.at(i, j) > 0) {
          first_pop.insert(Coord(i, j));
        }
      }
    }
    last_locs_added = first_pop;
    return;
  }

  std::unordered_set<Coord> new_pop;
  for (int i = 0; i < pop_density_matrix.numberRows(); i++) {
    for (int j = 0; j < pop_density_matrix.numberCols(); j++) {
      if (pop_density_matrix.at(i, j) > 0 && last_pop_matrix.at(i, j) == 0) {
        new_pop.insert(Coord(i, j));
      }
    }
  }

  for (Coord new_coord : new_pop) {
    double cumul_dvx = 0;
    double cumul_dvy = 0;
    for (Coord old_coord : last_locs_added) {
      double dvx = new_coord.x - old_coord.x;
      double dvy = new_coord.y - old_coord.y;
      double magn = std::sqrt(dvx*dvx + dvy*dvy);
      dvx /= magn;
      dvy /= magn;
      cumul_dvx += dvx;
      cumul_dvy += dvy;      
    }
    cumul_dvx /= last_locs_added.size();
    cumul_dvy /= last_locs_added.size();
    growth_gradients.at(new_coord.x, new_coord.y) = Vector(cumul_dvx, cumul_dvy);
  }

}