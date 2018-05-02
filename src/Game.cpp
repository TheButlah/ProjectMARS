#include "../include/Game.h"
#include "../include/Matrix.h"
#include "../include/PopulationGen.h"
#include "../include/Terrain.h"

#include <queue>
#include <limits>

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
    number_pop_serviced(0),
    number_plants_in_service(0),
    plant_servable_distance(serveable_distance),
    plant_initial_cost(initial_cost),
    plant_operating_cost(operating_cost),
    plant_profit_margin(profit_margin),
    pop_matrix(PopulationMatrix(dx, dy)),
    terrain(Terrain(dx, dy)),
    pop_gen(PopulationGen())

{

}

void Game::step(bool add_plant, Coord plant_coord) {
    Matrix<int> new_population = pop_gen.generate(this->pop_matrix.computeCombinedPop(), this->terrain);
    pop_matrix.addUnservicedPop(new_population);

    processUnservicedPopulation();
    
    //if new plant was added
    if (add_plant) {
        Plant* new_plant = createPlant(plant_coord);
        std::queue<Plant*> touched_plant = considerNewPlant(new_plant, false);
        processTouchedPlants(touched_plant);
    }

    //calculate objective
    double objective = this->calculateObjective();    
    this->funds = objective;
    
    //add new plants from last round to number_plants
    this->number_plants_in_service += number_new_plants;
    this->number_new_plants = 0;
}

std::pair<Plant*, bool> Game::findBestPlant(Coord person_loc) {
    std::pair<Plant*, double> best_plant(this->plants_in_service.front(), std::numeric_limits<double>::max());
    bool found_plant = false;
    for (Plant* plant : this->plants_in_service) {
        if (plant->isServiceableCoord(person_loc)) {
            if (best_plant.second > plant->distanceToCoord(person_loc)) {
                if (plant->remainingCapacity() > 0) {
                    //this plant has room to take new person
                    best_plant.second = plant->distanceToCoord(person_loc);
                    best_plant.first = plant;
                    found_plant = true;
                }
            }
        }
    }
    return std::pair<Plant*, bool>(best_plant.first, found_plant);
}

void Game::processUnservicedElement(int i, int j) {
    int number_to_service = pop_matrix.numberUnservicedAtCoord(Coord(i, j));
    if (number_to_service > 0) {
        int number_to_service = pop_matrix.numberUnservicedAtCoord(Coord(i, j));
        std::pair<Plant*, bool> result = findBestPlant(Coord(i,j));
        while (number_to_service > 0 or !(result.second)) {
            Plant* new_plant = result.first;
            int add_to_service = std::min(new_plant->remainingCapacity(), number_to_service);
            this->addServicedPop(add_to_service);
            this->pop_matrix.assignUnservicedPop(new_plant, Coord(i,j), add_to_service);
            new_plant->changeServicedPop(Coord(i,j), add_to_service);
            number_to_service -= add_to_service;
            result = findBestPlant(Coord(i,j));
        }
    }
}

void Game::processUnservicedPopulation() {
    for (int i = 0; i < this->pop_matrix.sizeX(); i++) {
        for (int j = 0; j < this->pop_matrix.sizeY(); j++) {
        this->processUnservicedElement(i,j);
        }
    }
}

void Game::addServicedPop(int new_serviced_pop) {
  this->number_pop_serviced += new_serviced_pop;
}

Plant* Game::createPlant(Coord plant_loc) {
    //update number_new_plants
    this->number_new_plants++;
    Plant* new_plant = new Plant(
        plant_default_capacity, 
        plant_servable_distance, 
        plant_loc.x, 
        plant_loc.y, 
        this->terrain
    );
    this->plants_in_service.push_back(new_plant);
    return new_plant;
};

std::queue<Plant*> Game::processServicedPop(
    Plant* plant, 
    Coord coord, 
    std::unordered_map<Plant*, int> serviced_map, 
    std::queue<Plant*>& queue) 
{
    for (std::pair<Plant*, int> mapping : serviced_map) {
        if (plant->remainingCapacity() > 0) {
            Plant* old_plant = mapping.first;
            if (plant->distanceToCoord(coord) < old_plant->distanceToCoord(coord)) {
                int add_to_service = std::min(plant->remainingCapacity(), mapping.second);
                this->pop_matrix.moveServicedPopBetweenPlants(old_plant, plant, coord, add_to_service);
                plant->changeServicedPop(coord, add_to_service);
                old_plant->changeServicedPop(coord, -add_to_service);                
                queue.push(old_plant);
            }
        }
    }
    return queue;
}

std::queue<Plant*> Game::considerNewPlant(Plant* plant, bool touched) {
    std::queue<Plant*> touched_plants;
    std::unordered_map<Coord, std::pair<int, std::unordered_map<Plant*, int>>> pop_to_consider
            = this->pop_matrix.potentialPopForPlant(plant);
    for (std::pair<Coord, std::pair<int, std::unordered_map<Plant*,int>>> element : pop_to_consider) {
        Coord coord = element.first;
        int unserviced = element.second.first;
        std::unordered_map<Plant*, int> serviced_map = element.second.second;
        processUnservicedElement(coord.x, coord.y);
        processServicedPop(plant, coord, serviced_map, touched_plants);
    }
    if (!touched) {  //return queue of "touched" plants
        return touched_plants;
    } else { //processing a "touched" plant
        std::queue<Plant*> empty;
        return empty;
    }
};

void Game::processTouchedPlants(std::queue<Plant*> touched_plants) {
    while (touched_plants.size() > 0) {//process each plant
        Plant* plant = touched_plants.front();
        std::queue<Plant*> empty = this->considerNewPlant(plant, true);
        touched_plants.pop();
    }
}

double Game::calculateObjective() {
	double objective;
	objective = this->funds - (this->plant_operating_cost)*(this->number_plants_in_service)
							-(this->number_new_plants * this->plant_initial_cost)
							+ (this->number_pop_serviced * this->plant_profit_margin);
	return objective;

}

Game::~Game() {
    for (Plant* p: plants_in_service) {
        delete p;
    }
}