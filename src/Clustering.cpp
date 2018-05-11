#include "../include/Clustering.h"

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <iostream>

using namespace MARS;

#define PLACE_PLANT_THRESHOLD 10
#define MIN_CENTROID_DIFFERENCE 0.5

std::pair<std::vector<Coord>, std::vector<std::vector<Coord>>> 
  Clustering::run(PopulationMatrix popMatrix, int k) {

  std::vector<std::vector<Coord>> clusters = std::vector<std::vector<Coord>>();
  std::vector<Coord> centroids = std::vector<Coord>();
  bool clusteredBefore = false;

  int dx = popMatrix.sizeX();
  int dy = popMatrix.sizeY();

  assert(k < dx * dy);

  int totalCentroidDifference = dx+dy;

  while(totalCentroidDifference > MIN_CENTROID_DIFFERENCE) {
    totalCentroidDifference = 0;

    if(!clusteredBefore) {
      // We don't have existing centroids to work with - randomly initialize them
      srand(time(0)); // Seed for random number generator

      for(int i = 0; i < k; i++) {
        Coord randomCentroid = Coord(rand() % dx, rand() % dy);

        while(std::find(centroids.begin(), centroids.end(), randomCentroid) != centroids.end()) {
          randomCentroid = Coord(rand() % dx, rand() % dy);
        }
        centroids.push_back(randomCentroid);
      }

      // Create vectors for each cluster and push them to clusters

      for(int i = 0; i < k; i++) {
        clusters.push_back(std::vector<Coord>());
      }

      clusteredBefore = true;
    } 
  
    assert(clusters.size() == centroids.size());

    // Create one data point for each person (unit of population density) in the matrix

    std::vector<Coord> dataPoints = std::vector<Coord>();

    for(int i = 0; i < dx; i++) {
      for(int j = 0; j < dy; j++) {
        int peopleHere = popMatrix.numberUnservicedAtCoord(MARS::Coord(i,j));
        for(int k = 0; k < peopleHere; k++) {
          dataPoints.push_back(Coord(i, j));
        }
      }
    }

    // For each coord in the dataset, add it to the cluster
    // corresponding to its closest centroid (k-means)

    for(int i = 0; i < dataPoints.size(); i++) {

      Coord dataPoint = dataPoints.at(i);

      int nearestCentroidIndex = 0;
      int nearestCentroidDistance = -1;

      for(int c = 0; c < centroids.size(); c++) {
        Coord centroid = centroids.at(c);
        int dist = (dataPoint.x - centroid.x)*(dataPoint.x - centroid.x) + (dataPoint.y - centroid.y)*(dataPoint.y - centroid.y);
        if(nearestCentroidDistance == -1 || dist < nearestCentroidDistance) {
          nearestCentroidDistance = dist;
          nearestCentroidIndex = c;
        }
      }

      assert(nearestCentroidIndex < clusters.size());
      clusters.at(nearestCentroidIndex).push_back(dataPoint);
    }

    // Compute new centroids

    for(int i = 0; i < clusters.size(); i++) {
      float sumX = 0.0;
      float sumY = 0.0;
      
      std::vector<Coord> currentCluster = clusters.at(i);

      for(int j = 0; j < currentCluster.size(); j++) {
        sumX += currentCluster.at(j).x;
        sumY += currentCluster.at(j).y;
      }

      int clusterSize = clusters.at(i).size();
      assert(i < centroids.size());

      if(clusterSize != 0) {
        // Add to 'centroid difference' (for convergence condition)
        totalCentroidDifference += centroids.at(i).x - (sumX/clusterSize);
        totalCentroidDifference += centroids.at(i).y - (sumY/clusterSize);

        centroids.at(i) = Coord(
          sumX/clusterSize, 
          sumY/clusterSize
        );
      }
      
    }
  }
  return std::pair<std::vector<Coord>, std::vector<std::vector<Coord>>> (centroids, clusters);
}

int sparsity(std::vector<Coord> cluster, Coord centroid) {
  // Sum up all the distances from the centroid
  int total = 0;
  for(int i = 0; i < cluster.size(); i++) {
    total += (cluster.at(i).x - centroid.x)*(cluster.at(i).x - centroid.x) + (cluster.at(i).y - centroid.y)*(cluster.at(i).y - centroid.y);
  }
  return total;
}

std::pair<bool, Coord> Clustering::placePlant(PopulationMatrix popMatrix, int k) {
  // Take the least "sparse" unserviced cluster above a certain size, if it exists,
  // and place a plant at its center

  std::pair<std::vector<Coord>, std::vector<std::vector<Coord>>> clusterResult = Clustering::run(popMatrix, k);

  std::vector<Coord> centroids = clusterResult.first;
  std::vector<std::vector<Coord>> clusters = clusterResult.second;

  std::vector<int> clusterSparsity;

  for(int i = 0; i < clusters.size(); i++) {
    std::vector<Coord> cluster = clusters.at(i);
    assert(i < centroids.size());
    Coord centroid = centroids.at(i);
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
  assert(minSparsityIndex < centroids.size());

  if(unservicedClusterExists) {
    placement = Coord(0, 0);
  }
  else {
    placement = centroids.at(minSparsityIndex);
  }

  return std::pair<bool,Coord>(unservicedClusterExists, placement);
}