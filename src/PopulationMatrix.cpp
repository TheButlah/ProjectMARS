#include "../include/PopulationMatrix.h"

using namespace MARS;

PopulationMatrix::PopulationMatrix(int dx, int dy):
    servicedPopMatrix(dx, dy),
    unservicedPopMatrix(dx, dy),
    plantAssignMatrix(dx, dy) 
{

}

int PopulationMatrix::numberServicedAtCoord(Coord c) {
    return servicedPopMatrix.at(c.x, c.y);
}

int PopulationMatrix::numberUnservicedAtCoord(Coord c) {
    return unservicedPopMatrix.at(c.x, c.y);    
}

int PopulationMatrix::numberServicedAtCoordByPlant(Coord c, Plant p) {
    return plantAssignMatrix.at(c.x, c.y)[p];
}

std::unordered_map<Coord, std::pair<int, std::unordered_map<Plant, int>>> PopulationMatrix::potentialPopForPlant(Plant p) {
  std::unordered_map<Coord, std::pair<int, std::unordered_map<Plant, int>>> result;
    
    std::unordered_map<Coord, double> serviceable_area = p.serviceable_area;
    for (std::pair<Coord, double> element : serviceable_area) {
        Coord coord = element.first;
        int num_unserviced = unservicedPopMatrix.at(coord.x, coord.y);
        int num_serviced = 0;

        std::unordered_map<Plant, int> serviced_potential_pop;

        for (std::pair<Plant, int> pairing : plantAssignMatrix.at(coord.x, coord.y)) {
            Plant plant = pairing.first;
            if (plant.serviceable_area[coord] > p.serviceable_area[coord]) {
              serviced_potential_pop[pairing.first] = pairing.second;
            }

        }
        result[coord] = std::pair<int, std::unordered_map<Plant, int>>(num_unserviced, serviced_potential_pop);
        //result.push_back(std::pair<Coord, int>(coord, num_unserviced + num_serviced));
    } 
    return result;
}

void PopulationMatrix::movePop(Plant from, Plant to, Coord c, int num_pop) {
    plantAssignMatrix.at(c.x, c.y)[from] -= num_pop;
    plantAssignMatrix.at(c.x, c.y)[to] += num_pop;
}

void PopulationMatrix::assignUnserviced(Plant p, Coord c, int num_pop) {
    unservicedPopMatrix.at(c.x, c.y) -= num_pop;
    plantAssignMatrix.at(c.x, c.y)[p] += num_pop;    
}

void PopulationMatrix::addUnserviced(Matrix<int>& newUnserviced) {
    for (int i = 0; i < unservicedPopMatrix.numberRows(); i++) {
        for (int j = 0; j < unservicedPopMatrix.numberCols(); j++) {
            unservicedPopMatrix.at(i,j) += newUnserviced.at(i,j);
        }
    }
}

Matrix<int> PopulationMatrix::computeCombinedPop(){
    Matrix<int> combinedPopMatrix = Matrix<int>(servicedPopMatrix.numberRows(), servicedPopMatrix.numberCols());
    for (int i = 0; i < servicedPopMatrix.numberRows(); i++) {
        for (int j = 0; j < servicedPopMatrix.numberCols(); j++) {
            combinedPopMatrix.at(i,j) = servicedPopMatrix.at(i,j)+ unservicedPopMatrix.at(i,j);
        }
    }
    return combinedPopMatrix;
}