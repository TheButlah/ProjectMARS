#include "../include/Clustering.h"

#include <algorithm>
#include <cstdlib>
#include <ctime>

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
			Coord centroid = this->centroids[c];
			int dist = (dataPoint.x - centroid.x)*(dataPoint.x - centroid.x) + (dataPoint.y - centroid.y)*(dataPoint.y - centroid.y)
			if(nearestCentroidDistance == -1 || dist < nearestCentroidDistance) {
				nearestCentroidDistance = dist;
				nearestCentroidIndex = c;
			}
		}

		this->clusters[nearestCentroidIndex].push(point);
	}

	// Compute new centroids

}

Coord Clustering::placePlant() {
	// Take the most "compact" unserviced cluster above a certain size, if it exists,
	// and place a plant at its center
	return Coord(0, 0);
}