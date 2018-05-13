#include "RLState.h"

using namespace MARS;

RLState::RLState(const MARS::Game& game) :
  totalPops(game.sizeY(), game.sizeX()),
  unservicedPops(game.sizeY(), game.sizeX()),
  servicedPops(game.sizeY(), game.sizeX()),
  terrain(game.sizeY(), game.sizeX()),
  plantLocs(game.sizeY(), game.sizeX())
{

}

void RLState::update(const MARS::Game& game) {
  PopulationMatrix pm = game.popMatrixCopy();
  totalPops = pm.totalPopMatrix();
  unservicedPops = pm.unservicedPopMatrix();
  servicedPops = pm.servicedPopMatrix();

  terrain = game.terrainCopy().getMatrixCopy();

  plantLocs.resetToDefault();
  for (Coord& c : game.plantLocations()) {
    plantLocs.at(c.x, c.y) = true;
  }

}