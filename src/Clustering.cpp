#include "../include/Clustering.h"

#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace MARS;

#define PLACE_PLANT_THRESHOLD 10

Clustering::Clustering(int k)
	{
		this->k = k;
    this->clusters = std::vector<std::vector<Coord>>();
		this->centroids = std::vector<Coord>();
		this->clusteredBefore = false;
	}

void Clustering::run(PopulationMatrix popMatrix) {

	int dx = popMatrix.sizeX();
	int dy = popMatrix.sizeY();

	assert(this->k < dx * dy);
	if(!this->clusteredBefore) {
		// We don't have existing centroids to work with - randomly initialize them
		srand(time(0)); // Seed for random number generator

		for(int i = 0; i < k; i++) {
			Coord randomCentroid = Coord(rand() % dx, rand() % dy);

			while(std::find(this->centroids.begin(), this->centroids.end(), randomCentroid) != this->centroids.end()) {
				randomCentroid = Coord(rand() % dx, rand() % dy);
			}
			this->centroids.push_back(randomCentroid);
		}

		// Create vectors for each cluster and push them to this->clusters

		for(int i = 0; i < k; i++) {
			this->clusters.push_back(std::vector<Coord>());
		}
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

	// For each coord in the dataset, add it to the cluster
	// corresponding to its closest centroid (k-means)

	for(int i = 0; i < dataPoints.size(); i++) {

		Coord dataPoint = dataPoints.at(i);

		//Coord nearestCentroidIndex;
    int nearestCentroidIndex;
		int nearestCentroidDistance = -1;

		for(int c = 0; c < this->centroids.size(); c++) {
			Coord centroid = this->centroids.at(c);
			int dist = (dataPoint.x - centroid.x)*(dataPoint.x - centroid.x) + (dataPoint.y - centroid.y)*(dataPoint.y - centroid.y);
			if(nearestCentroidDistance == -1 || dist < nearestCentroidDistance) {
				nearestCentroidDistance = dist;
				nearestCentroidIndex = c;
			}
		}

		this->clusters[nearestCentroidIndex].push_back(dataPoint);
	}

	// Compute new centroids

	for(int i = 0; i < this->clusters.size(); i++) {
		float sumX = 0.0;
		float sumY = 0.0;
		
		std::vector<Coord> currentCluster = this->clusters.at(i);

		for(int j = 0; j < currentCluster.size(); j++) {
			sumX += currentCluster.at(j).x;
			sumY += currentCluster.at(j).y;
		}

		this->centroids[i] = Coord(
			sumX/this->clusters.at(i).size(), 
			sumY/this->clusters.at(i).size()
		);
	}

}

int sparsity(std::vector<Coord> cluster, Coord centroid) {
	// Sum up all the distances from the centroid

	int total = 0;

	for(int i = 0; i < cluster.size(); i++) {
		total += (cluster[i].x - centroid.x)*(cluster[i].x - centroid.x) + (cluster[i].y - centroid.y)*(cluster[i].y - centroid.y);
	}

	return total;

}

std::pair<bool, Coord> Clustering::placePlant(PopulationMatrix popMatrix) {
	// Take the least "sparse" unserviced cluster above a certain size, if it exists,
	// and place a plant at its center

	this->run(popMatrix);

	std::vector<int> clusterSparsity;

	for(int i = 0; i < this->clusters.size(); i++) {
		std::vector<Coord> cluster = this->clusters.at(i);
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

  //unsure about how to define this boolean? assuming the return type is necessary...
  bool unservicedClusterExists = clusterSparsity.size()>0;

	return std::pair<bool,Coord>(unservicedClusterExists, this->centroids[minSparsityIndex]);
}