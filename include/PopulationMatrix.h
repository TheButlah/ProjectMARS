#ifndef MARS_POPULATIONMATRIX_H
#define MARS_POPULATIONMATRIX_H

#include <map>

#include "Plant.h"
#include "Matrix.h"

namespace MARS {

  class PopulationMatrix {
  private:
    /*
     * servicedPopMatrix: matrix containing serviced populations
     * unservicedPopMatrix: matrix containining unserviced populations
     * plantAssignMatrix: assignment of plants to populations
     *
     */
    Matrix<int> serviced_pop_matrix;
    Matrix<int> unserviced_pop_matrix;
    Matrix<std::unordered_map<Plant*, int>> plant_assign_matrix;
  public:
    
    /*
     * Constructor
     */
    PopulationMatrix(int dx, int dy);
    
    /*
     * Number of people serviced at a given coordinate.
     */
    int numberServicedAtCoord(const Coord& c) const;
    
    /*
     * Number of people unserviced at a given coordinate.
     */
    int numberUnservicedAtCoord(const Coord& c) const;
    
    /*
     * Number of people serviced at a given coordinate by a given plant.
     */
    int numberServicedAtCoordByPlant(const Coord& c, Plant* p) const;
    
    /*
     * Returns a mapping of coordinates to (unserviced, {plant => serviced_by_plant})
     * pairs within a plant's serviceable area.
     */
    std::unordered_map<Coord, std::pair<int, std::unordered_map<Plant*, int>>> potentialPopForPlant(Plant* p);
    
    /*
     * Moves a population at a given coordinate from one plant to another.
     */
    void moveServicedPopBetweenPlants(Plant* from, Plant* to, const Coord& c, int num_pop);
    
    /*
     * Assigns an unserviced population at a given coordinate to a given plant.
     */
    void assignUnservicedPop(Plant* p, const Coord& c, int num_pop);
     
    /*
     * Matrix-adds a new unserviced population mapping to the existing unserviced population mapping.
     */
    void addUnservicedPop(Matrix<int>& newUnserviced);
    
    Matrix<int> servicedPopMatrix() const;
    Matrix<int> unservicedPopMatrix() const;
    Matrix<int> totalPopMatrix() const;
    int sizeX() const;
    int sizeY() const;
    
  };
}

#endif
