#ifndef MARS_CLUSTERING_H
#define MARS_CLUSTERING_H

#include "PopulationMatrix.h"
#include "Coord.h"

#include <utility>
#include <vector>

namespace MARS {
	class Clustering {
	private:
		int k;
		bool clusteredBefore;
		std::vector<std::vector<Coord>> clusters;
		std::vector<Coord> centroids;
		void run(PopulationMatrix popMatrix);
	public:
    Clustering(int k);
		Clustering(int k, int dx, int dy);
		std::pair<bool, Coord> placePlant(PopulationMatrix popMatrix); // Do we place a plant at this step, and where do we place it?
	};
}

#endif