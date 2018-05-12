# Python 2-3 compatibility
from __future__ import print_function
from __future__ import division
from __future__ import unicode_literals
from __future__ import absolute_import

import numpy as np
import tensorflow as tf
import project_mars as pm

from model import QMap


def main():
  state_shape = (3, 3, 10)
  model = QMap(state_shape, 2)
  initial_state = np.zeros((1,)+state_shape)
  q = model.predict_q(initial_state, np.zeros((1, 3)))
  print('Q:', q)
  q, selected = model.predict_q(initial_state)
  print('Q:', q, 'Selected:', selected)


def main1():
  # Game initialization

  game = pm.Game(
    dx=16,
    dy=16,
    number_of_turns=1000,
    default_capacity=25,
    servable_distance=3.0,
    initial_cost=50.0,
    operating_cost=25.0,
    profit_margin=5.0,
    unserviced_penalty=1.0
  )
  tmp = pm.Coord(0, 0)
  print(game)
  cli = pm.CLIRepl(game)
  cli.startCLI()
  print('WILL NEVER REACH THIS')
  for i in range(100000000):
      game.step(False, tmp)
  print(cli)


if __name__ == "__main__":
  main()
