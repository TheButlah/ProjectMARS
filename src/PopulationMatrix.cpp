#include "../include/PopulationMatrix.h"

using namespace MARS;

PopulationMatrix::PopulationMatrix(int dx, int dy):
  serviced_pop_matrix(dx, dy),
  unserviced_pop_matrix(dx, dy),
  plant_assign_matrix(dx, dy) 
{

}

int PopulationMatrix::numberServicedAtCoord(const Coord& c) const {
  return serviced_pop_matrix.at(c.x, c.y);
}

int PopulationMatrix::numberUnservicedAtCoord(const Coord& c) const {
  return unserviced_pop_matrix.at(c.x, c.y);    
}

int PopulationMatrix::numberServicedAtCoordByPlant(const Coord& c, Plant* p) const {
  return plant_assign_matrix.at(c.x, c.y).at(p);
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

void PopulationMatrix::moveServicedPopBetweenPlants(Plant* from, Plant* to, const Coord& c, int num_pop) {
  plant_assign_matrix.at(c.x, c.y)[from] -= num_pop;
  plant_assign_matrix.at(c.x, c.y)[to] += num_pop;
}

void PopulationMatrix::assignUnservicedPop(Plant* p, const Coord& c, int num_pop) {
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

Matrix<int> PopulationMatrix::totalPopMatrix() const {
  Matrix<int> combinedPopMatrix = Matrix<int>(serviced_pop_matrix.numberRows(), serviced_pop_matrix.numberCols());
  for (int i = 0; i < serviced_pop_matrix.numberRows(); i++) {
    for (int j = 0; j < serviced_pop_matrix.numberCols(); j++) {
      combinedPopMatrix.at(i,j) = serviced_pop_matrix.at(i,j)+ unserviced_pop_matrix.at(i,j);
    }
  }
  return combinedPopMatrix;
}

Matrix<int> PopulationMatrix::servicedPopMatrix() const {
  return serviced_pop_matrix;
}

Matrix<int> PopulationMatrix::unservicedPopMatrix() const {
  return unserviced_pop_matrix;
}


int PopulationMatrix::sizeX() const {
  return serviced_pop_matrix.numberRows();
}

int PopulationMatrix::sizeY() const {
  return serviced_pop_matrix.numberRows();
}  