#ifndef MARS_GAME_H
#define MARS_GAME_H

#include "Matrix.h"
#include "PopulationGen.h"
#include "Terrain.h"

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
        PopulationGen populationGenerator;

        Matrix<int> popMatrix; //Integer matrix containing population density
        int numberNewPlants; //Number of new plants built in current turn
        int numberPlantsInService; //Number of plants in service, discluding newly built plants
        int numberInService; //Number of people serviced by plants

        // Game parameters - supplied to the game to determine its inital state and behavior
        int sizeX;
        int sizeY;
        int numberTurns;
        int plant_default_capacity;
        double plant_servable_distance;
        double plant_initial_cost;
        double plant_operating_cost;
        double plant_profit_margin;


    public:
        Game(
                int dx,
                int dy,
                int numberTurns,
                int default_capacity,
                double servable_distance,
                double initial_cost,
                double operating_cost,
                double profit_margin
        );

        /* Advance the game's progress by one time step */
        void step();

        ~Game();

        double calculateObjective();
    };
}

#endif