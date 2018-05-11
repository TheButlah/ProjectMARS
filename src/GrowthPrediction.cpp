#include "../include/GrowthPrediction.h"

using namespace MARS;

GrowthPrediction::GrowthPrediction(Game *game, int time_interval): last_pop_matrix(0, 0) {
  this->game = game;
  this->time_interval = time_interval;
}

void GrowthPrediction::updateGradients() {
  int t = game->getCurrentTime();
  if (t == 0)
    return;

  Matrix<int> pop_density_matrix = game->getPopMatrix.computeCombinedPop();

  if (t == time_interval) {
    last_pop_matrix = pop_density_matrix;
    for (int i = 0; i < last_pop_matrix.numberRows(); i++) {
      for (int j = 0; j < last_pop_matrix.numberCols(); j++) {
        
      }
    }
  }


}