#ifndef MARS_CLIREPL_H
#define MARS_CLIREPL_H

#include <string>
#include <vector>

#include "GameDisplay.h"
#include "Game.h"
#include "Clustering.h"

namespace MARS {
  class CLIRepl {
  private:
    int size_x;
    int size_y;
    Game *game;
    GameDisplay *game_display;

    void loggingLoop(int steps, int decision_interval, int k, std::string path);
    void loggingFromFile(std::string params_path, std::string output_dir_path);
    void stepWithClustering(int k);
    void stepWithRandom();
  public:
    CLIRepl(std::string inifile);
    CLIRepl(Game* game);
    ~CLIRepl();

    std::vector<std::string> tokenize(std::string s);
    void draw();
    void printUsage();
    void printStats();
    void doCommand(std::vector<std::string> tokens);
    void startCLI();
  };
}

#endif