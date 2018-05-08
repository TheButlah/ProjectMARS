#include "../include/Clustering.h"

#include <algorithm>
#include <cstdlib>
#include <ctime>

#define PLACE_PLANT_THRESHOLD = 10;

using namespace MARS;

Clustering::Clustering(int k)
	{
		this->k = k;
		this->clusters = std::vector<std::vector<std::pair>>();
		this->centroids = std::vector<Coord>;
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
			this->centroids.push(randomCentroid);
		}

		// Create vectors for each cluster and push them to this->clusters

		for(int i = 0; i < k; i++) {
			this->clusters.push(std::vector<Coord>());
		}
	}	
	
	// Create one data point for each person (unit of population density) in the matrix

	std::vector<Coord> dataPoints = std::vector<Coord>();

	for(int i = 0; i < dx; i++) {
		for(int j = 0; j < dy; j++) {
			int peopleHere = popMatrix.unserviced_pop_matrix.at(i, j);
			for(int k = 0; k < peopleHere; k++) {
				dataPoints.push(Coord(i, j));
			}
		}
	}

	// For each coord in the dataset, add it to the cluster
	// corresponding to its closest centroid (k-means)

	for(int i = 0; i < dataPoints.size(); i++) {

		Coord point = dataPoints.at(i);

		Coord nearestCentroidIndex;
		int nearestCentroidDistance = -1;

		for(int c = 0; c < this->centroids->size(); c++) {
			Coord centroid = this->centroids.at(c);
			int dist = (dataPoint.x - centroid.x)*(dataPoint.x - centroid.x) + (dataPoint.y - centroid.y)*(dataPoint.y - centroid.y)
			if(nearestCentroidDistance == -1 || dist < nearestCentroidDistance) {
				nearestCentroidDistance = dist;
				nearestCentroidIndex = c;
			}
		}

		this->clusters[nearestCentroidIndex].push(point);
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
		total += (cluster.x - centroid.x)*(cluster.x - centroid.x) + (cluster.y - centroid.y)*(cluster.y - centroid.y);
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
			clusterSparsity.push(sparsity(cluster, centroid));
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

	return this->centroids[minSparsityIndex];
}