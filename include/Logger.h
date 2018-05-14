#ifndef MARS_LOGGER_H
#define MARS_LOGGER_H

#include <fstream>
#include <vector>
#include <unordered_map>

#include "Game.h"


namespace MARS {
  class Logger {
  private:
    Game* game;
    std::ofstream log_file;
    int last_diff;
  public:
    Logger(Game* game, std::string filename);
    void log(bool place, Coord plant_loc);
  };
}

#endif