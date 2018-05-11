#ifndef MARS_GAME_DISPLAY_H
#define MARS_GAME_DISPLAY_H

#include "Game.h"
#include "CImg.h"

namespace MARS {
  class GameDisplay {
  private:
    double box_size;
    double pop_scale;
    
    Game* game;
    cimg_library::CImg<unsigned char>* img;
    cimg_library::CImgDisplay* display;
  public:
    GameDisplay();
    GameDisplay(Game* game, double box_size, double pop_scale);
    ~GameDisplay();

    void updateDisplay();
  };
}

#endif