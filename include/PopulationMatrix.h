#ifndef MARS_POPULATIONMATRIX_H
#define MARS_POPULATIONMATRIX_H

#include <map>

#include "Matrix.h"

namespace MARS {

    class PopulationMatrix {
    private:
        Matrix<int> servicedPopMatrix;
        Matrix<int> unservicedPopMatrix;

        /*
        int numberServicedAtCoord(Coord c);
        int numberUnservicedAtCoord(Coord c);
        int numberServicedAtCoordByPlant(Coord c, Plant p);
        std::pair<int, std::vector<Plant, int>> potentialPopForPlant(Plant p);
        void moveCustomers(Plant src, int count, Plant dest);
        void assignUnserviced(int count, Plant dest);
        */
    };
}

#endif