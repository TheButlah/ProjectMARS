#include "../include/PopulationMatrix.h"

using namespace MARS;

PopulationMatrix::PopulationMatrix(int dx, int dy):
  serviced_pop_matrix(dx, dy),
  unserviced_pop_matrix(dx, dy),
  plant_assign_matrix(dx, dy) 
{

}

int PopulationMatrix::numberServicedAtCoord(Coord c) {
  return serviced_pop_matrix.at(c.x, c.y);
}

int PopulationMatrix::numberUnservicedAtCoord(Coord c) {
  return unserviced_pop_matrix.at(c.x, c.y);    
}

int PopulationMatrix::numberServicedAtCoordByPlant(Coord c, Plant* p) {
  return plant_assign_matrix.at(c.x, c.y)[p];
}

std::unordered_map<Coord, std::pair<int, std::unordered_map<Plant*, int>>> PopulationMatrix::potentialPopForPlant(Plant* p) {
  std::unordered_map<Coord, std::pair<int, std::unordered_map<Plant*, int>>> result;
  
  std::unordered_map<Coord, double> serviceable_area = p->serviceableArea();
  for (std::pair<Coord, double> element : serviceable_area) {
    Coord coord = element.first;
    int num_unserviced = unserviced_pop_matrix.at(coord.x, coord.y);

    std::unordered_map<Plant*, int> serviced_potential_pop;

    for (std::pair<Plant*, int> pairing : plant_assign_matrix.at(coord.x, coord.y)) {
      Plant* plant = pairing.first;
      if (plant->distanceToCoord(coord) > element.second) {
        serviced_potential_pop[pairing.first] = pairing.second;
      }

    }
    result[coord] = std::pair<int, std::unordered_map<Plant*, int>>(num_unserviced, serviced_potential_pop);
  } 
  return result;
}

void PopulationMatrix::moveServicedPopBetweenPlants(Plant* from, Plant* to, Coord c, int num_pop) {
  plant_assign_matrix.at(c.x, c.y)[from] -= num_pop;
  plant_assign_matrix.at(c.x, c.y)[to] += num_pop;
}

void PopulationMatrix::assignUnservicedPop(Plant* p, Coord c, int num_pop) {
  unserviced_pop_matrix.at(c.x, c.y) -= num_pop;
  serviced_pop_matrix.at(c.x,c.y) += num_pop;
  plant_assign_matrix.at(c.x, c.y)[p] += num_pop;    
}

void PopulationMatrix::addUnservicedPop(Matrix<int>& newUnserviced) {
  for (int i = 0; i < unserviced_pop_matrix.numberRows(); i++) {
    for (int j = 0; j < unserviced_pop_matrix.numberCols(); j++) {
      unserviced_pop_matrix.at(i,j) += newUnserviced.at(i,j);
    }
  }
}

Matrix<int> PopulationMatrix::computeCombinedPop(){
  Matrix<int> combinedPopMatrix = Matrix<int>(serviced_pop_matrix.numberRows(), serviced_pop_matrix.numberCols());
  for (int i = 0; i < serviced_pop_matrix.numberRows(); i++) {
    for (int j = 0; j < serviced_pop_matrix.numberCols(); j++) {
      combinedPopMatrix.at(i,j) = serviced_pop_matrix.at(i,j)+ unserviced_pop_matrix.at(i,j);
    }
  }
  return combinedPopMatrix;
}

int PopulationMatrix::sizeX() {
  return serviced_pop_matrix.numberRows();
}

int PopulationMatrix::sizeY() {
  return serviced_pop_matrix.numberRows();    
}

Matrix<int> PopulationMatrix::servicedPopMatrix() {
  return serviced_pop_matrix;
}

Matrix<int> PopulationMatrix::unservicedPopMatrix() {
  return unserviced_pop_matrix;
}