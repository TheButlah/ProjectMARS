#ifndef MARS_RLSTATE_H
#define MARS_RLSTATE_H

#include "Matrix.h"
#include "Game.h"

namespace MARS {

  class RLState {
  public:
    Matrix<int> totalPops;
    Matrix<int> unservicedPops;
    Matrix<int> serviecdPops;
    Matrix<float> terrain;
    Matrix<bool> plantLocs;

    RLState(const Game&);
    RLState(const RLState&) = delete;
    RLState& operator=(const RLState&) = delete;

    void update(const Game& game);
  };
}

#endif
