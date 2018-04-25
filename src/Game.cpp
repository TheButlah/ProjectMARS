#include "../include/Game.h"
#include "../include/Matrix.h"
#include "../include/PopulationGen.h"
#include "../include/Terrain.h"

using namespace MARS;

Game::Game(
				int dx,
				int dy,
				int default_capacity,
				int number_turns,
				double serveable_distance,
				double initial_cost,
				double operating_cost,
				double profit_margin
) :
				size_x(dx),
				size_y(dy),
        time(0),
				plant_default_capacity(default_capacity),
				number_turns(number_turns),
        number_in_service(0),
				plant_servable_distance(serveable_distance),
				plant_initial_cost(initial_cost),
				plant_operating_cost(operating_cost),
				plant_profit_margin(profit_margin),
				pop_matrix(PopulationMatrix(dx, dy)),
				terrain(Terrain(dx, dy)),
				populationGenerator(PopulationGen())


{

}

void Game::step() {
	Matrix<int> new_population = populationGenerator.generate(this->pop_matrix.computeCombinedPop());
  //this->popMatrix = populationGenerator.generate(this->popMatrix);
  pop_matrix.addUnserviced(new_population);
  processUnservicedPopulation();

}



std::pair<Plant, bool> Game::findBestPlant(Coord person_loc) {
  std::pair<Plant,double> best_plant(this->plants_in_service.front(), 10000.0);
  bool found_plant = true;
  for (Plant plant : this->plants_in_service) {
    std::unordered_map<Coord,double>::const_iterator iter = plant.serviceable_area.find(person_loc);
    if (iter == plant.serviceable_area.end()) {
      //no plant can service this coord
      found_plant = false;
    }
    else {
      if (best_plant.second > iter->second) {
        if (plant.in_service < plant.capacity) {
          //this plant has room to take new person
          best_plant.second = iter->second;
          best_plant.first = plant;
        }
      }
    }
  }
  return std::pair<Plant, bool>(best_plant.first, found_plant);
}



void Game::processUnservicedElement(int i, int j) {
  if (pop_matrix.unservicedPopMatrix.at(i,j) >0) {
    int number_to_service = pop_matrix.unservicedPopMatrix.at(i,j);
    std::pair<Plant, bool> result = findBestPlant(Coord(i,j));
    while (number_to_service > 0 or !(result.second)) {
      Plant new_plant = result.first;
      int add_to_service = std::min(new_plant.capacity- new_plant.in_service, number_to_service);
      this->addToService(add_to_service);
      new_plant.addToService(add_to_service);
      this->pop_matrix.assignUnserviced(new_plant, Coord(i,j), add_to_service);
      new_plant.updateMapping(new_plant, Coord(i,j), add_to_service);
      number_to_service -= add_to_service;
      result = findBestPlant(Coord(i,j));
    }
  }
}

void Game::processUnservicedPopulation() {
  for (int i = 0; i < this->pop_matrix.unservicedPopMatrix.numberRows(); i++) {
    for (int j = 0; j < this->pop_matrix.unservicedPopMatrix.numberCols(); j++) {
      this->processUnservicedElement(i,j);
    }
  }
}

void Game::addToService(int new_serviced_pop) {
  this->number_in_service += new_serviced_pop;
}

void Game::processServicedPopulation() {

}

double Game::calculateObjective() {
	double objective;
	objective = this->funds - (this->plant_operating_cost)*(this->number_plants_in_service)
							-(this->number_new_plants * this->plant_initial_cost)
							+ (this->number_in_service * this->plant_profit_margin);
	return objective;

}

Game::~Game() {

}