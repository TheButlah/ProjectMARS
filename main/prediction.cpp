#include "../include/Game.h"
#include "../include/GrowthPrediction.h"
#include "../include/Logger.h"
#include "../include/GameDisplay.h"

#include <queue>
#include <vector>
#include <limits>

using namespace MARS;

void runExperiment(
  int sizeX, 
  int sizeY, 
  int default_capacity, 
  int servable_distance,
  int steps,
  std::string filename) 
{
  Game game(
    sizeX,
    sizeY,
    steps,
    default_capacity,
    servable_distance,
    0,
    0,
    1,
    1
  );
  std::cout << "Size: " << sizeX << "x" << sizeY << ", ServeDist " << servable_distance << std::endl;
  GameDisplay gd(&game, 10, 50);
  GrowthPrediction gp(&game, 10);
  gp.updateStateRecord();
  Logger logger(&game, filename);
  std::queue<Coord> plant_queue;
  for (int i = 0; i < steps; i++) {
    bool decision = false;
    Coord loc(0,0);
    if (plant_queue.size() > 0) {
      decision = true;
      loc = plant_queue.front();
      plant_queue.pop();
    }
    game.step(decision, loc);
    std::vector<Coord> new_plants = gp.predictNewPlants();
    for (Coord c : new_plants) {
      plant_queue.push(c);
    }
    gp.updateStateRecord();
    logger.log(decision, loc);
    gd.updateDisplay();
  }
}

int main() {
  runExperiment(16, 16, std::numeric_limits<int>::max(), 3.0, 1000, "predict_16_16_3_1000.csv");
  runExperiment(64, 64, std::numeric_limits<int>::max(), 3.0, 1000, "predict_64_64_3_1000.csv");  
  runExperiment(64, 64, std::numeric_limits<int>::max(), 8.0, 1000, "predict_64_64_8_1000.csv");
  runExperiment(64, 64, std::numeric_limits<int>::max(), 16.0, 1000, "predict_64_64_16_1000.csv");  
  runExperiment(128, 128, std::numeric_limits<int>::max(), 16.0, 1000, "predict_128_128_16_1000.csv");
  runExperiment(256, 256, std::numeric_limits<int>::max(), 40.0, 1000, "predict_256_256_40_1000.csv");  
}

/*

init():
    P := sample of potential power plants
    avg_cover = average size of serviceable areas of plants p in P
    B := split terrain into tiled subgrids of roughly size avg_cover
    for each subgrid b in B:
      assign potential plant p to b

predict():
    S := population cells to which population has spread
    P' = empty set, used to store plants to place
    for each coord c in S
        b := subgrid that c belongs to
        p := the potential plant assigned to subgrid
        if p is not already placed:
            add p to P'
      return P'

*/