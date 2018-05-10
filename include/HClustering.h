#ifndef MARS_CLUSTERING_H
#define MARS_CLUSTERING_H

#include "PopulationMatrix.h"
#include "Coord.h"

#include <utility>
#include <vector>

namespace MARS {
	class HClustering {
	private:
		std::vector<std::vector<Coord>> clusters;
		void run(PopulationMatrix popMatrix);
	public:
		HClustering();
		std::pair<bool, Coord> placePlant(PopulationMatrix popMatrix); // Do we place a plant at this step, and where do we place it?
	};
}

#endif