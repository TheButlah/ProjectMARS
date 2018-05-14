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
}