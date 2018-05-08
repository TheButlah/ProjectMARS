#include "Game.h"

using namespace MARS;

Clustering::Clustering(int k): 
	k(-1),
	{
		this->clusters = std::vector<std::vector<std::pair>>();
		this->centroids = std::vector<Coord>;
	}

void Clustering::run(PopulationMatrix popMatrix) {
	
}

Coord Clustering::placePlant() {
	// Take the most "compact" unserviced cluster above a certain size, if it exists,
	// and place a plant at its center
	return Coord(0, 0);
}