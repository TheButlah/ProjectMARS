#include "../include/Game.h"
#include "../include/Matrix.h"
#include "../include/PopulationGen.h"
#include "../include/Terrain.h"
#include <queue>

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
        number_plants_in_service(0),
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
  
  //add new plants from last round to number_plants
  this->number_plants_in_service += number_new_plants;
  this->number_new_plants = 0;

  processUnservicedPopulation();
  //if new plant was added
    //createPlant()
    //considerNewPlant()
    //process queue of touched plants
  //calculate objective
  //return
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


Plant Game::createPlant(Coord plant_loc) {
  //update number_new_plants
  this->number_new_plants++;
  Plant new_plant = Plant(plant_default_capacity, plant_servable_distance, plant_loc.x, plant_loc.y, this->terrain);
  this->plants_in_service.push_back(new_plant);
  return new_plant;
};

std::queue<Plant> Game::processServicedPop(Plant plant, std::pair<Coord, std::unordered_map<Plant,int>> coord_map, std::queue<Plant> queue) {
  Coord coord = coord_map.first;
  std::unordered_map<Plant, int> serviced_map = coord_map.second;
  for (std::pair<Plant, int> mapping : serviced_map) {
    if (plant.in_service < plant.capacity) {
      Plant old_plant = mapping.first;
      if (plant.serviceable_area[coord] < old_plant.serviceable_area[coord]) {
        int add_to_service = std::min(plant.capacity-plant.in_service, mapping.second);
        plant.addToService(add_to_service);
        this->pop_matrix.movePop(old_plant, plant, coord, add_to_service);
        queue.push(old_plant);
      }
    }
  }
  return queue;
}

std::queue<Plant> Game::considerNewPlant(Plant plant, bool touched) {
  std::queue<Plant> touched_plants;
  std::unordered_map<Coord, std::pair<int, std::unordered_map<Plant, int>>> pop_to_consider
          = this->pop_matrix.potentialPopForPlant(plant);
  for (std::pair<Coord, std::pair<int, std::unordered_map<Plant,int>>> element : pop_to_consider) {
    Coord coord = element.first;
    int unserviced = element.second.first;
    std::unordered_map<Plant, int> serviced_map = element.second.second;
    processUnservicedElement(coord.x, coord.y);
    touched_plants = processServicedPop(plant, std::pair<Coord, std::unordered_map<Plant,int>>(coord, serviced_map), touched_plants);
  }
  if (!touched) {  //return queue of "touched" plants
    return touched_plants;
  }
  else { //processing a "touched" plant
    std::queue<Plant> empty;
    return empty;
  }
};


void Game::processTouchedPlants(std::queue<Plant> touched_plants) {
  while (touched_plants.size() > 0) {
    //process each plant
    Plant plant = touched_plants.front();
    std::queue<Plant> empty = this->considerNewPlant(plant, true);
  }
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