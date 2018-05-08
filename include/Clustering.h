#ifndef MARS_CLUSTERING_H
#define MARS_CLUSTERING_H

#include "PopulationMatrix.h"
#include "Coord.h"

#include <vector>

namespace MARS {
	class Clustering {
	private:
		int k;
		std::vector<std::vector<Coord>> clusters;
		std::vector<Coord> centroids;
		void run(PopulationMatrix popMatrix);
	public:
		Clustering(int k, int dx, int dy);
		Coord placePlant();
	};
}

#endif