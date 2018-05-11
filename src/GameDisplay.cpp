#include "../include/GameDisplay.h"

using namespace MARS;
using namespace cimg_library;

GameDisplay::GameDisplay(Game* game, double box_size, double pop_scale) {
  this->game = game;
  this->box_size = box_size;
  this->pop_scale = pop_scale;
  
  std::pair<int, int> size = game->getSize();
  int size_x = size.first;
  int size_y = size.second;
  double BOX_SIZE = box_size;
  double POP_SCALE = pop_scale;
 
  img = new CImg<unsigned char>(13+2*size_y*BOX_SIZE, 13+2*size_x*BOX_SIZE, 1, 3, 0);
  display = new CImgDisplay(*img, "Project MARS");
}

GameDisplay::GameDisplay() {
  
}

GameDisplay::~GameDisplay() {
  delete img;
  delete display;
}

void GameDisplay::updateDisplay() {
  std::pair<int, int> size = game->getSize();
  int size_x = size.first;
  int size_y = size.second;
  double BOX_SIZE = box_size;
  double POP_SCALE = pop_scale;
  img->fill(0);
  for (int i = 0; i < size_x; i++) {
    unsigned char text_color[3];
    text_color[0] = 255;
    text_color[1] = 255;
    text_color[2] = 0;
    if (i % 5 == 0) {
      img->draw_text(
        0,
        (i)*BOX_SIZE+13,
        std::to_string(i).c_str(),
        text_color,
        0,
        1.0
      );
    }
  }

  for (int i = 0; i < size_y; i++) {
    unsigned char text_color[3];
    text_color[0] = 255;
    text_color[1] = 255;
    text_color[2] = 0;
    if (i % 5 == 0) {
      img->draw_text(
        (i)*BOX_SIZE+13,
        0,
        std::to_string(i).c_str(),
        text_color,
        0,
        1.0
      );
    }
  }
  
  Terrain terrain = game->getTerrain();
  for (int i = 0; i < terrain.sizeX(); i++) {
    for (int j = 0; j < terrain.sizeY(); j++) {
      unsigned char color[3];
      if (terrain.weightAtXY(i, j) == GRASSLAND_WEIGHT) {
        color[0] = 0;
        color[1] = 255;
        color[2] = 0;    
      } else if (terrain.weightAtXY(i, j) == MOUNTAIN_WEIGHT) {
        color[0] = 128;
        color[1] = 128;
        color[2] = 128;    
      
      } else {
        color[0] = 0;
        color[1] = 0;
        color[2] = 255;    
      }
      img->draw_rectangle(
        13+(j+size_y)*BOX_SIZE+1, 
        13+i*BOX_SIZE+1, 
        0,
        13+(j+size_y+1)*BOX_SIZE-2, 
        13+(i+1)*BOX_SIZE-2, 
        0, 
        color, 
        1.0);
    }
  }

  Matrix<int> unserviced_pop_matrix = game->unservicedPopMatrix();
  for (int i = 0; i < unserviced_pop_matrix.numberRows(); i++) {
    for (int j = 0; j < unserviced_pop_matrix.numberCols(); j++) {
      unsigned char color[3];
      color[0] = 0;
      color[1] = 255 * (terrain.weightAtXY(i, j) == 1.0);
      color[2] = 0;
      img->draw_rectangle(
        13+(j)*BOX_SIZE, 
        13+i*BOX_SIZE, 
        0, 
        13+(j+1)*BOX_SIZE, 
        13+(i+1)*BOX_SIZE, 
        0, 
        color,
        1.0);
      color[0] = (unsigned char) (255.0*std::min(1.0, unserviced_pop_matrix.at(i, j)/POP_SCALE));
      color[1] = 0;
      color[2] = (unsigned char) (255.0*std::min(1.0, unserviced_pop_matrix.at(i, j)/POP_SCALE));
      img->draw_rectangle(
        13+(j)*BOX_SIZE+1, 
        13+i*BOX_SIZE+1, 
        0, 
        13+(j+1)*BOX_SIZE-2, 
        13+(i+1)*BOX_SIZE-2, 
        0, 
        color, 
        1.0);
      color[0] = 0;
      color[1] = 128 * (terrain.weightAtXY(i, j) == 1.0);
      color[2] = 0;
      int smol = (int)(0.4*BOX_SIZE);
      img->draw_rectangle(
        13+(j)*BOX_SIZE+smol, 
        13+i*BOX_SIZE+smol, 
        0, 
        13+(j+1)*BOX_SIZE-smol-1, 
        13+(i+1)*BOX_SIZE-smol-1, 
        0, 
        color,
        1.0);
    }
  }
  
  std::vector<Coord> locs = game->getPlantLocations();
  for (Coord c : locs) {
    unsigned char color[3];
    color[0] = 255;
    color[1] = 0;
    color[2] = 0;
    int i = c.x;
    int j = c.y;
    img->draw_rectangle(
      13+(j)*BOX_SIZE, 
      13+(i+size_x)*BOX_SIZE, 
      0,
      13+(j+1)*BOX_SIZE, 
      13+(i+size_x+1)*BOX_SIZE, 
      0, 
      color,
      1.0);
  }

  Matrix<int> serviced_pop_matrix = game->servicedPopMatrix();
  for (int i = 0; i < serviced_pop_matrix.numberRows(); i++) {
    for (int j = 0; j < serviced_pop_matrix.numberCols(); j++) {
      unsigned char color[3];
      color[0] = (unsigned char) (255.0*std::min(1.0, serviced_pop_matrix.at(i, j)/20.0));
      color[1] = 0;
      color[2] = (unsigned char) (255.0*std::min(1.0, serviced_pop_matrix.at(i, j)/20.0));
      img->draw_rectangle(
        13+(j)*BOX_SIZE+1, 
        13+(i+size_x)*BOX_SIZE+1, 
        0, 
        13+(j+1)*BOX_SIZE-2, 
        13+(i+size_x+1)*BOX_SIZE-2, 
        0, 
        color, 
        1.0);
    }
  }

  unsigned char text_color[3];
  text_color[0] = 255;
  text_color[1] = 255;
  text_color[2] = 0;
  std::string time_string = "Time: " + std::to_string(game->getCurrentTime());
  std::string funds_string = "Objective value: " + std::to_string(game->calculateObjective());
  std::string plants_string = "Plants in service: " + std::to_string(game->getNumberPlantsInService());
  std::string unserviced_string = "Number of unserviced pop: " + std::to_string(game->getNumberPopUnserviced());
  std::string serviced_string = "Number of serviced pop: " + std::to_string(game->getNumberPopServiced());
  
  img->draw_text(
    23+size_y*BOX_SIZE,
    23+size_x*BOX_SIZE,
    time_string.c_str(),
    text_color,
    0,
    1.0
  );
  img->draw_text(
    23+size_y*BOX_SIZE,
    23+size_x*BOX_SIZE+20,
    funds_string.c_str(),
    text_color,
    0,
    1.0
  );
  img->draw_text(
    23+size_y*BOX_SIZE,
    23+size_x*BOX_SIZE+40,
    plants_string.c_str(),
    text_color,
    0,
    1.0
  );
  img->draw_text(
    23+size_y*BOX_SIZE,
    23+size_x*BOX_SIZE+60,
    unserviced_string.c_str(),
    text_color,
    0,
    1.0
  );
  img->draw_text(
    23+size_y*BOX_SIZE,
    23+size_x*BOX_SIZE+80,
    serviced_string.c_str(),
    text_color,
    0,
    1.0
  );

  img->display(*display);

}