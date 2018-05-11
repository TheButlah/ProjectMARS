#ifndef MARS_CLIREPL_H
#define MARS_CLIREPL_H

#include <string>
#include <vector>

#include "CImg.h"
#include "Game.h"
#include "Clustering.h"

namespace MARS {
  class CLIRepl {
  private:
    std::string inifile;
    int size_x;
    int size_y;
    Game *game;
    cimg_library::CImg<unsigned char>* img;
    cimg_library::CImgDisplay* display;
    void kmeansLoop(int steps, int decision_interval, int k, std::string path);
    void runTrace(std::string trace_path);
    void stepWithClustering(int k);
    void stepWithRandom();
    void initializeGame(std::string inifile);
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