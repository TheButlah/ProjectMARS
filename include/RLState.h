#ifndef MARS_RLSTATE_H
#define MARS_RLSTATE_H

#include "Matrix.h"
#include "Game.h"

namespace MARS {

  /**
   * Provides the RL agent a way to interpret the state.
   */
  class RLState {
  public:
    Matrix<int> totalPops;
    Matrix<int> unservicedPops;
    Matrix<int> serviecdPops;

    Matrix<float> terrain;

    // Not using BitMatrix because I care more about speed than memory usage
    Matrix<bool> plantLocs;

    RLState(const Game&);
    RLState(const RLState&) = delete;
    RLState& operator=(const RLState&) = delete;

    /** Update this object to reflect the latest state of the game */
    void update(const Game& game);
  };
}

#endif
