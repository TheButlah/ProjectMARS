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

    //void updateRLState();

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
    ~Game();    

    int sizeX() const;
    int sizeY() const;


    /* Advance the game's progress by one time step */
    void step(bool add_plant, Coord plant_coord);
    double calculateObjective() const;

    int numberPlantsInService() const;
    std::vector<Coord> plantLocations() const;

    int numberTotalPopAt(int, int);
    int numberServicedPop() const;
    int numberUnservicedPop() const;

    double currentFunds() const;
    int currentTime() const;
    int plantDefaultCapacity() const;
    double plantServableDistance() const;
    PopulationMatrix popMatrixCopy() const;
    Terrain terrainCopy() const;
    std::pair<int, int> sizeXY() const;

    std::pair<Plant*, bool> findBestPlant(Coord person_loc);
    void processUnservicedElement(int i, int j);
    void processUnservicedPopulation();
    std::queue<Plant*> processServicedPop(Plant*, Coord, std::unordered_map<Plant*,int>, std::queue<Plant*>&);

    Plant* createPlant(Coord);
    std::queue<Plant*> considerNewPlant(Plant*, bool);
    void processTouchedPlants(std::queue<Plant*>);

    bool isPlantPresent(Coord);
    double fundsForCurrentStep();
  };
}

#endif
