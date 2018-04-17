#include "../include/Game.h"
#include "../include/Matrix.h"
#include "../include/PopulationGen.h"
#include "../include/Terrain.h"

using namespace MARS;
  
Game::Game(
      int dx, 
      int dy, 
      double default_capacity, 
      double servable_distance, 
      double initial_cost, 
      double operating_cost, 
      double profit_margin
    ) :
	sizeX(dx), 
	sizeY(dy),
	plant_default_capacity(default_capacity),
	plant_servable_distance(servable_distance),
	plant_initial_cost(initial_cost),
	plant_operating_cost(operating_cost),
	plant_profit_margin(profit_margin),
	popMatrix(Matrix<double>(dx, dy)),
	terrain(Terrain(dx, dy)),
	populationGenerator(PopulationGen())
{

}
void Game::step() {
	this->popMatrix = populationGenerator.generate(this->popMatrix);
}
Game::~Game() {

}
