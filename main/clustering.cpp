#include "../include/Clustering.h"
#include "../include/Game.h"
#include <stdio.h>
#include <iostream>

using namespace MARS;

int main() {
  int k;
  std::cout << "Enter the number of clusters: ";
  std::cin >> k; //
  //Game initialization
  int dx = 16;
  int dy = 16;
  int number_of_turns = 1000;
  int default_capacity = 25;
  double servable_distance = 3.0;
  double initial_cost = 50.0;
  double operating_cost = 25.0;
  double profit_margin = 5.0;
  double unserviced_penalty = 1.0;
  MARS::Game *game = new MARS::Game(
          dx,
          dy,
          number_of_turns,
          default_capacity,
          servable_distance,
          initial_cost,
          operating_cost,
          profit_margin,
          unserviced_penalty);
  for (int i=0; i< number_of_turns; i++) {
    if (i%100 != 0) {
      game->step(false, Coord(0, 0));
    }
    else { //cluster
      std::pair<bool, Coord> res = Clustering::placePlantKMeans(game->popMatrixCopy(), k);
      game->step(res.first, res.second);
    }
  }

  std::cout << " number of plants built = " << game->numberPlantsInService() << std::endl;
  std::cout << " number in service = " << game->numberServicedPop() << std::endl;
  std::cout << " number unserviced = " << game->numberUnservicedPop() << std::endl;


}