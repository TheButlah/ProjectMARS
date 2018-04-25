#ifndef MARS_POPULATIONMATRIX_H
#define MARS_POPULATIONMATRIX_H

#include <map>

#include "Plant.h"
#include "Matrix.h"

namespace MARS {

    class PopulationMatrix {
    public:
        Matrix<int> servicedPopMatrix;
        Matrix<int> unservicedPopMatrix;
        Matrix<std::unordered_map<Plant, int>> plantAssignMatrix;

        PopulationMatrix(int dx, int dy);
        
        int numberServicedAtCoord(Coord c);
        int numberUnservicedAtCoord(Coord c);
        int numberServicedAtCoordByPlant(Coord c, Plant p);
        std::unordered_map<Coord, std::pair<int, std::unordered_map<Plant, int>>> potentialPopForPlant(Plant p);
        void movePop(Plant from, Plant to, Coord c, int num_pop);
        void assignUnserviced(Plant p, Coord c, int num_pop);
        void addUnserviced(Matrix<int>& newUnserviced);
        Matrix<int> computeCombinedPop();

    };
}

#endif