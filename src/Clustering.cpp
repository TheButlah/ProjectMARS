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

      clusteredBefore = true;
    }

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

    clusters = std::vector<std::vector<Coord>>(); // reset clusters to be empty
    for(int i = 0; i < k; i++) {
      clusters.push_back(std::vector<Coord>());
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

    for(int i = 0; i < k; i++) {
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

std::pair<bool, Coord> Clustering::placePlant(PopulationMatrix popMatrix, int k) {
  // Take the largest unserviced cluster and place a plant at its center

  std::pair<std::vector<Coord>, std::vector<std::vector<Coord>>> clusterResult = Clustering::run(popMatrix, k);

  std::vector<Coord> centroids = clusterResult.first;
  std::vector<std::vector<Coord>> clusters = clusterResult.second;

  int maxSizeIndex = 0;
  int maxSize = PLACE_PLANT_THRESHOLD;

  for(int i = 0; i < clusters.size(); i++) {
    if(clusters.at(i).size() > maxSize) {
      maxSize = clusters.at(i).size();
      maxSizeIndex = i;
    }
  }

  bool unservicedClusterExists = maxSize > PLACE_PLANT_THRESHOLD;
  Coord placement;
  assert(maxSizeIndex < centroids.size());

  if(unservicedClusterExists) {
    placement = Coord(0, 0);
  }
  else {
    placement = centroids.at(maxSizeIndex);
  }

  return std::pair<bool,Coord>(unservicedClusterExists, placement);
}