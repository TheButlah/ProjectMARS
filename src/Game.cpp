#include "../include/Game.h"
#include "../include/Matrix.h"
#include "../include/PopulationGen.h"
#include "../include/Terrain.h"

using namespace MARS;

Game::Game(
				int dx,
				int dy,
				int default_capacity,
				int numberTurns,
				double serveable_distance,
				double initial_cost,
				double operating_cost,
				double profit_margin
) :
				sizeX(dx),
				sizeY(dy),
				plant_default_capacity(default_capacity),
				numberTurns(numberTurns),
				plant_servable_distance(serveable_distance),
				plant_initial_cost(initial_cost),
				plant_operating_cost(operating_cost),
				plant_profit_margin(profit_margin),
				popMatrix(Matrix<int>(dx, dy)),
				terrain(Terrain(dx, dy)),
				populationGenerator(PopulationGen())
{

}

void Game::step() {
	this->popMatrix = populationGenerator.generate(this->popMatrix);
}

double Game::calculateObjective() {
	double objective;
	objective = this->funds - (this->plant_operating_cost)*(this->numberPlantsInService)
							-(this->numberNewPlants * this->plant_initial_cost)
							+ (this->numberInService * this->plant_profit_margin);
	return objective;

}

Game::~Game() {

}