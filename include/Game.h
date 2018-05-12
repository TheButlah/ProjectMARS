#ifndef MARS_GAME_H
#define MARS_GAME_H

#include <queue>
#include <utility>

#include "Matrix.h"
#include "PopulationGen.h"
#include "Terrain.h"
#include "PopulationMatrix.h"

namespace MARS {

  /*
   * Game - a simulation of a growing population of people on a terrain, in which power plants are placed to support the existing and future population
   */
  class Game {
  private:
    // Game state - traits of the simulation that change over time

    int time; // The current time in the game
    double funds; // The funds available for building plants
    Terrain terrain;
    PopulationGen pop_gen;
    std::vector<Plant*> plants_in_service;
    PopulationMatrix pop_matrix; //Integer matrix containing population density
    int number_new_plants; //Number of new plants built in current turn
    int number_plants_in_service; //Number of plants in service, discluding newly built plants
    int number_pop_serviced; //Number of people serviced by plants

    // Game parameters - supplied to the game to determine its inital state and behavior
    int size_x;
    int size_y;
    int number_turns;
    int plant_default_capacity;
    double plant_servable_distance;
    double plant_initial_cost;
    double plant_operating_cost;
    double plant_profit_margin;
    double unserviced_pop_penalty;

  public:
    Game(
      int dx,
      int dy,
      int number_turns,
      int default_capacity,
      double servable_distance,
      double initial_cost,
      double operating_cost,
      double profit_margin,
      double unserviced_penalty
    );

    int sizeX();
    int sizeY();

    /* Advance the game's progress by one time step */
    void step(bool add_plant, Coord plant_coord);

    int getPopulationAt(int, int);

    bool checkIfPlantPresent(Coord);

    int getNumberPlantsInService();

    int getNumberPopServiced();

    int getNumberPopUnserviced();

    std::pair<int, int> getSize();

    std::vector<Coord> getPlantLocations();

    PopulationMatrix getPopMatrix();

    int getNumberNewPlants();

    double currentFunds();

    int getCurrentTime();

    Terrain getTerrain();

    Matrix<int> servicedPopMatrix();

    Matrix<int> unservicedPopMatrix();

    ~Game();

    double fundsForCurrentStep();

    double calculateObjective();

    std::pair<Plant*, bool> findBestPlant(Coord person_loc);

    void processUnservicedPopulation();

    void processUnservicedElement(int i, int j);

    std::queue<Plant*> processServicedPop(Plant*, Coord, std::unordered_map<Plant*,int>, std::queue<Plant*>&);

    void addServicedPop(int);

    std::queue<Plant*> considerNewPlant(Plant*, bool);

    Plant* createPlant(Coord);

    void processTouchedPlants(std::queue<Plant*>);
  };
}

#endif
