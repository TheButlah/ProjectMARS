#ifndef MARS_GAMEDISPLAY_H
#define MARS_GAMEDISPLAY_H

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
  
    void drawAxis();
    void drawUnserviced();
    void drawTerrain();
    void drawServiced();    
    void drawStats();
  public:
    GameDisplay(Game* game, double box_size, double pop_scale);
    ~GameDisplay();

    void updateDisplay();
  };
}

#endif