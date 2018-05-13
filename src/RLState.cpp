//
// Created by ryan on 5/12/18.
//

#include "RLState.h"

using namespace MARS;

RLState::RLState(const Game& game) :
  totalPops(game.sizeY(), game.sizeX()),
  unservicedPops(game.sizeY(), game.sizeX()),
  serviecdPops(game.sizeY(), game.sizeX()),
  terrain(game.sizeY(), game.sizeX()),
  plantLocs(game.sizeY(), game.sizeX())
{

}

void RLState::update(const Game& game) {
  PopulationMatrix pm = game.popMatrixCopy();
  totalPops = pm.totalPopMatrix();
  unservicedPops = pm.unservicedPopMatrix();
  serviecdPops = pm.servicedPopMatrix();

  terrain = game.terrainCopy().getMatrixCopy();

  plantLocs.resetToDefault();
  for (Coord& c : game.plantLocations()) {
    plantLocs.at(c.x, c.y) = true;
  }

}