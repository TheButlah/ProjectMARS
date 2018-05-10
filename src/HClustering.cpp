#include "../include/Clustering.h"

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <iostream>

using namespace MARS;

#define PLACE_PLANT_THRESHOLD 10
#define MIN_CENTROID_DIFFERENCE 0.5

HClustering::HClustering()
  {
    this->clusters = std::vector<std::vector<Coord>>();
  }

void HClustering::run(PopulationMatrix popMatrix) {
  this->clusters = std::vector<std::vector<Coord>>();
  
}

std::pair<bool, Coord> HClustering::placePlant(PopulationMatrix popMatrix) {

  /* This is identical to Clustering::placePlant. */

  // Take the least "sparse" unserviced cluster above a certain size, if it exists,
  // and place a plant at its center

  this->run(popMatrix);

  std::vector<int> clusterSparsity;

  for(int i = 0; i < this->clusters.size(); i++) {
    std::vector<Coord> cluster = this->clusters.at(i);
    assert(i < this->centroids.size());
    Coord centroid = this->centroids.at(i);
    if(cluster.size() > PLACE_PLANT_THRESHOLD) {
      clusterSparsity.push_back(sparsity(cluster, centroid));
    }
  }

  int minSparsityIndex;
  int minSparsity = -1;

  for(int i = 0; i < clusterSparsity.size(); i++) {
    if(minSparsity == -1 || clusterSparsity.at(i) < minSparsity) {
      minSparsity = clusterSparsity.at(i);
      minSparsityIndex = i;
    }
  }

  bool unservicedClusterExists = clusterSparsity.size()>0;
  Coord placement;
  assert(minSparsityIndex < this->centroids.size());

  if(unservicedClusterExists) {
    placement = Coord(0, 0);
  }
  else {
    placement = this->centroids.at(minSparsityIndex);
  }

  return std::pair<bool,Coord>(unservicedClusterExists, placement);
}