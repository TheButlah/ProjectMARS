#include "../include/GrowthPrediction.h"
#include "../include/Terrain.h"

#include <cstdlib>
#include <cmath>
#include <unordered_map>

using namespace MARS;

GrowthPrediction::GrowthPrediction(Game* game, int sample_size):
  game(game),
  last_pop_matrix(0, 0),
  last_diff(0),
  last_diff_slope(0)
{
  double sum_size = 0;
  Terrain terrain = game->terrainCopy();
  for (int i = 0; i < sample_size; i++) {
    int randx = std::rand() % game->sizeX();
    int randy = std::rand() % game->sizeY();
    double terrain_weight = terrain.weightAtXY(randx, randy);
    while (terrain_weight == WATER_WEIGHT || terrain_weight == MOUNTAIN_WEIGHT) {
      randx = std::rand() % game->sizeX();
      randy = std::rand() % game->sizeY();
      terrain_weight = terrain.weightAtXY(randx, randy);
    }
    Plant p(game->plantDefaultCapacity(), game->plantServableDistance(), randx, randy, terrain);
    sum_size += p.serviceableArea().size();
  }
  avg_cover = (int) (sum_size / sample_size);
}

std::unordered_set<Coord> GrowthPrediction::unservicedCoords(bool old) {
  Matrix<int> unserviced_pop_matrix = game->popMatrixCopy().unservicedPopMatrix();
  std::unordered_set<Coord> result;
  for (int i = 0; i < game->sizeX(); i++) {
    for (int j = 0; j < game->sizeY(); j++) {
      if (unserviced_pop_matrix.at(i, j) > 0) {
        if (old && last_pop_matrix.at(i, j) > 0) {
          result.insert(Coord(i, j));
        } else if (!old && last_pop_matrix.at(i, j) == 0) {
          result.insert(Coord(i, j));          
        }
      }
    }
  }
  return result;
}

int GrowthPrediction::binXY() {
  return (int) ((game->plantServableDistance() + std::sqrt(avg_cover)) / 2.0);
}

Coord GrowthPrediction::nearestValidCoord(Coord c, Terrain& terrain) {
  Coord new_coord = c;
  while (new_coord.x < terrain.sizeX()) {
    if (terrain.weightAtCoord(new_coord) == GRASSLAND_WEIGHT)
      return new_coord;
    new_coord.x++;
  }

  new_coord = c;
  while (new_coord.x >= 0) {
    if (terrain.weightAtCoord(new_coord) == GRASSLAND_WEIGHT)
      return new_coord;
    new_coord.x--;
  }

  new_coord = c;
  while (new_coord.y < terrain.sizeY()) {
    if (terrain.weightAtCoord(new_coord) == GRASSLAND_WEIGHT)
      return new_coord;
    new_coord.y++;
  }

  new_coord = c;
  while (new_coord.y >= 0) {
    if (terrain.weightAtCoord(new_coord) == GRASSLAND_WEIGHT)
      return new_coord;
    new_coord.y--;
  }
  return c;
}

Coord GrowthPrediction::plantLocationInBin(Coord bin) {
  int x = bin.x*binXY();
  int y = bin.y*binXY();
  x = std::min(x, game->sizeX()-1);
  y = std::min(y, game->sizeY()-1);
  Terrain terrain = game->terrainCopy();
  if (terrain.weightAtXY(x, y) != GRASSLAND_WEIGHT) 
    return nearestValidCoord(Coord(x, y), terrain);
  return Coord(x, y);
}

std::vector<Coord> GrowthPrediction::predictNewPlants() {
  Matrix<int> unserviced_pop_matrix = game->popMatrixCopy().unservicedPopMatrix();  
  std::unordered_set<Coord> unserv_coords = unservicedCoords(false);
  std::unordered_map<Coord, int> bins;

  for (Coord c : unserv_coords) {
    int binx = c.x / binXY();
    int biny = c.y / binXY();
    Coord bin(binx, biny);
    bins[bin] = bins[bin] + unserviced_pop_matrix.at(c.x, c.y);
  }

  std::vector<Coord> result;
  for (std::pair<Coord, int> element : bins) {
    Coord plant_loc = plantLocationInBin(element.first);
    if (!game->isPlantPresent(plant_loc)) {
      result.push_back(plant_loc);
    }
  }

  return result;
}

void GrowthPrediction::updateStateRecord() {
  last_pop_matrix = game->popMatrixCopy().totalPopMatrix();
  int diff = game->numberServicedPop() - game->numberUnservicedPop();
  last_diff_slope = diff - last_diff;
  last_diff = diff;
}