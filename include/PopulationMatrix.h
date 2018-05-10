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
    int numberServicedAtCoord(Coord c);
    
    /*
     * Number of people unserviced at a given coordinate.
     */
    int numberUnservicedAtCoord(Coord c);
    
    /*
     * Number of people serviced at a given coordinate by a given plant.
     */
    int numberServicedAtCoordByPlant(Coord c, Plant* p);
    
    /*
     * Returns a mapping of coordinates to (unserviced, {plant => serviced_by_plant})
     * pairs within a plant's serviceable area.
     */
    std::unordered_map<Coord, std::pair<int, std::unordered_map<Plant*, int>>> potentialPopForPlant(Plant* p);
    
    /*
     * Moves a population at a given coordinate from one plant to another.
     */
    void moveServicedPopBetweenPlants(Plant* from, Plant* to, Coord c, int num_pop);
    
    /*
     * Assigns an unserviced population at a given coordinate to a given plant.
     */
    void assignUnservicedPop(Plant* p, Coord c, int num_pop);
     
    /*
     * Matrix-adds a new unserviced population mapping to the existing unserviced population mapping.
     */
    void addUnservicedPop(Matrix<int>& newUnserviced);
    
    /*
     * Combines unserviced and serviced population mappings into a single population matrix. 
     */
    Matrix<int> computeCombinedPop();

    int sizeX();
    int sizeY();
    Matrix<int> servicedPopMatrix();
    Matrix<int> unservicedPopMatrix();        
  
  };
}

#endif
