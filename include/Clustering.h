#ifndef MARS_CLUSTERING_H
#define MARS_CLUSTERING_H

#include "PopulationMatrix.h"
#include "Coord.h"

#include <utility>
#include <vector>

namespace MARS {
	class Clustering {
	private:
		static std::pair<std::vector<Coord>, std::vector<std::vector<Coord>>> runKMeans(PopulationMatrix popMatrix, int k);
		static std::pair<std::vector<Coord>, std::vector<std::vector<Coord>>> runKMedians(PopulationMatrix popMatrix, int k);
		static std::pair<bool, Coord> processClusteringResults (std::vector<Coord> centroids, std::vector<std::vector<Coord>> clusters);
	public:
		static std::pair<bool, Coord> placePlantKMeans(PopulationMatrix popMatrix, int k);
		static std::pair<bool, Coord> placePlantKMedians(PopulationMatrix popMatrix, int k);
		static std::pair<bool, Coord> placePlantRandom(PopulationMatrix popMatrix);
	};
}

#endif