#include "Clustering.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <iostream>

using namespace MARS;

#define PLACE_PLANT_THRESHOLD 10
#define MIN_CENTROID_DIFFERENCE 0.5

std::pair<std::vector<Coord>, std::vector<std::vector<Coord>>> 
  Clustering::runKMeans(PopulationMatrix popMatrix, int k) {

  std::vector<std::vector<Coord>> clusters = std::vector<std::vector<Coord>>();
  std::vector<Coord> centroids = std::vector<Coord>();

  int dx = popMatrix.sizeX();
  int dy = popMatrix.sizeY();

  assert(k < dx * dy);

  int totalCentroidDifference = dx+dy;

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

  srand(time(0)); // Seed for random number generator

  for(int i = 0; i < k; i++) {
    Coord randomCentroid = Coord(rand() % dx, rand() % dy);
    while(std::find(centroids.begin(), centroids.end(), randomCentroid) != centroids.end()) {
      randomCentroid = Coord(rand() % dx, rand() % dy);
    }
    centroids.push_back(randomCentroid);
  }

  while(totalCentroidDifference > MIN_CENTROID_DIFFERENCE) {
    totalCentroidDifference = 0;

    clusters = std::vector<std::vector<Coord>>(); // (re)set clusters to be empty
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

      //std::cout << "nearest " << nearestCentroidIndex << " clusters size " << clusters.size() << std::endl;

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

      if(clusterSize > 0) {
        // Add to 'centroid difference' (for convergence condition)
        totalCentroidDifference += abs(centroids.at(i).x - (sumX/clusterSize));
        totalCentroidDifference += abs(centroids.at(i).y - (sumY/clusterSize));

        centroids.at(i) = Coord(
          sumX/clusterSize, 
          sumY/clusterSize
        );
      }
      
    }
  }

  return std::pair<std::vector<Coord>, std::vector<std::vector<Coord>>> (centroids, clusters);
}

std::pair <std::vector<Coord>, std::vector<std::vector<Coord>>> 
  Clustering::runKMedians(PopulationMatrix popMatrix, int k) {

    /* TODO: DRY */

  std::vector<std::vector<Coord>> clusters = std::vector<std::vector<Coord>>();
  std::vector<Coord> centroids = std::vector<Coord>();

  int dx = popMatrix.sizeX();
  int dy = popMatrix.sizeY();

  assert(k < dx * dy);

  int totalCentroidDifference = dx+dy;

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

  srand(time(0)); // Seed for random number generator

  for(int i = 0; i < k; i++) {
    Coord randomCentroid = Coord(rand() % dx, rand() % dy);
    while(std::find(centroids.begin(), centroids.end(), randomCentroid) != centroids.end()) {
      randomCentroid = Coord(rand() % dx, rand() % dy);
    }
    centroids.push_back(randomCentroid);
  }

  while(totalCentroidDifference > MIN_CENTROID_DIFFERENCE) {
    totalCentroidDifference = 0;

    clusters = std::vector<std::vector<Coord>>(); // (re)set clusters to be empty
    for(int i = 0; i < k; i++) {
      clusters.push_back(std::vector<Coord>());
    }

    // For each coord in the dataset, add it to the cluster
    // corresponding to its closest centroid

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
    // Take the median x and median y value and use those

    for(int i = 0; i < k; i++) {
      std::vector<Coord> currentCluster = clusters.at(i);

      std::vector<int> currentX = std::vector<int>();
      for(int j = 0; j < currentCluster.size(); j++) {
        currentX.push_back(currentCluster.at(j).x);
      }
      std::vector<int> currentY = std::vector<int>();
      for(int j = 0; j < currentCluster.size(); j++) {
        currentY.push_back(currentCluster.at(j).y);
      }

      std::sort(currentX.begin(), currentX.end());
      std::sort(currentY.begin(), currentY.end());


      assert(i < centroids.size());
      assert(i < clusters.size());
      int clusterSize = clusters.at(i).size();

      if(clusterSize > 0) {
        int medianX = currentX.at(currentX.size()/2);
        int medianY = currentY.at(currentY.size()/2);

        // Add to 'centroid difference' (for convergence condition)
        totalCentroidDifference += abs(centroids.at(i).x - medianX);
        totalCentroidDifference += abs(centroids.at(i).y - medianY);

        centroids.at(i) = Coord(
          medianX,
          medianY
        );
      }
      
    }
  }

  return std::pair<std::vector<Coord>, std::vector<std::vector<Coord>>> (centroids, clusters);

}

std::pair <bool, Coord> Clustering::processClusteringResults(
  std::vector<Coord> centroids,
  std::vector<std::vector<Coord>> clusters) {

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

  if(!unservicedClusterExists) {
    placement = Coord(0, 0);
  }
  else {
    placement = centroids.at(maxSizeIndex);
  }
  
  return std::pair<bool,Coord>(unservicedClusterExists, placement);
}

std::pair<bool, Coord> Clustering::placePlantKMeans(PopulationMatrix popMatrix, int k) {
  // Take the largest unserviced cluster and place a plant at its center

  std::pair<std::vector<Coord>, std::vector<std::vector<Coord>>> clusterResult = Clustering::runKMeans(popMatrix, k);

  std::vector<Coord> centroids = clusterResult.first;
  std::vector<std::vector<Coord>> clusters = clusterResult.second;

  return Clustering::processClusteringResults(centroids, clusters);
}

std::pair<bool, Coord> Clustering::placePlantKMedians(PopulationMatrix popMatrix, int k) {
  // Take the largest unserviced cluster and place a plant at its center

  std::pair<std::vector<Coord>, std::vector<std::vector<Coord>>> clusterResult = Clustering::runKMedians(popMatrix, k);

  std::vector<Coord> centroids = clusterResult.first;
  std::vector<std::vector<Coord>> clusters = clusterResult.second;

  return Clustering::processClusteringResults(centroids, clusters);
}

std::pair<bool, Coord> Clustering::placePlantRandom(PopulationMatrix popMatrix) {
  /* Random baseline method */
  int coinFlip = rand() % 2;
  if(coinFlip == 0) {
    // don't place a plant this time
    return std::pair<bool, Coord>(false, Coord(0, 0));
  }
  else {
    // place a plant in a random location
    int x = rand() % popMatrix.sizeX();
    int y = rand() % popMatrix.sizeY();
    return std::pair<bool, Coord>(true, Coord(x, y));
  }
}