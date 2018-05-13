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

}