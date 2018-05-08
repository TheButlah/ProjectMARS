#include "Game.h"

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
	}	
	
	// For each coord in the unserviced matrix, add it to the cluster
	// corresponding to its closest centroid

	this->clusters = std::vector<std::vector<std::pair>>();
	for(int i = 0; i < dx; i++) {
		for(int j = 0; j < dy; j++) {
			
		}
	}

}

Coord Clustering::placePlant() {
	// Take the most "compact" unserviced cluster above a certain size, if it exists,
	// and place a plant at its center
	return Coord(0, 0);
}