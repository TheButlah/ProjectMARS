import numpy as np
import tensorflow as tf
import project_mars as pm


def main():
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
  print(game)


if __name__ == "__main__":
  main()