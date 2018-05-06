#include "../include/CLIRepl.h"
#include "../include/INIReader.h"
#include "../include/Matrix.h"
#include "../include/Terrain.h"

#include <algorithm>
#include <iostream>
#include <sstream>


using namespace MARS;
using namespace cimg_library;

#define BOX_SIZE 20

CLIRepl::CLIRepl(std::string inifile)
{
    INIReader ini(inifile);
    int dx = ini.GetInteger("Default", "SizeX", 16);
    int dy = ini.GetInteger("Default", "SizeY", 16);
    int number_of_turns = ini.GetInteger("Default", "NumberOfTurns", 1000000);
    int default_capacity = ini.GetInteger("Default", "PlantCapacity", 25);
    double servable_distance = ini.GetInteger("Default", "PlantServableDistance", 3.0);
    double initial_cost = ini.GetInteger("Default", "PlantInitialCost", 50.0);
    double operating_cost = ini.GetInteger("Default", "PlantOperatingCost", 25.0);            
    double profit_margin = ini.GetInteger("Default", "PlantProfitMargin", 5.0);
    size_x = dx;
    size_y = dy;
    game = new Game(
        dx, 
        dy, 
        number_of_turns, 
        default_capacity, 
        servable_distance, 
        initial_cost, 
        operating_cost, 
        profit_margin); 
    img = new CImg<unsigned char>(13+2*size_y*BOX_SIZE, 13+2*size_x*BOX_SIZE, 1, 3, 0); 
    display = new CImgDisplay(*img, "Project MARS");
}

CLIRepl::~CLIRepl() {
    delete game;
    delete img;
    delete display;
}

std::vector<std::string> CLIRepl::tokenize(std::string s) {
    std::vector<std::string> result;
    std::istringstream iss(s);
    for(std::string s2; iss >> s2;)
        result.push_back(s2);
    return result;
}

void CLIRepl::draw() {
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
            color[0] = (unsigned char) (255.0*std::min(1.0, unserviced_pop_matrix.at(i, j)/20.0));
            color[1] = 0;
            color[2] = (unsigned char) (255.0*std::min(1.0, unserviced_pop_matrix.at(i, j)/20.0));
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

    img->display(*display);
}

void CLIRepl::printUsage() {
    std::cout << "Commands:" << std::endl;
    std::cout << "step - steps the game without making a plant" << std::endl;
    std::cout << "step r c - steps the game while making a plant at location (r, c)" << std::endl;
    std::cout << "stats - print statistics about game state" << std::endl;
    std::cout << "help - print this list of commands" << std::endl;
}

void CLIRepl::printStats() {

}


void CLIRepl::doCommand(std::vector<std::string> tokens) {
    if (tokens.size() == 0) return;
    std::string command = tokens[0];
    if (command == "help") {
        this->printUsage();
    } else if (command == "step") {
        if (tokens.size() == 1) {
            game->step(false, Coord(0, 0));
        } else if (tokens.size() == 3) {
            int r = std::stoi(tokens[1]);
            int c = std::stoi(tokens[2]);
            game->step(true, Coord(r, c));
        } else {
            std::cout << "Did not provide correct number of arguments" << std::endl;
        }
    } else if (command == "stats") {
        this->printUsage();
    } else {
        this->printUsage();
    }
    draw();
}

void CLIRepl::startCLI() {
    std::cout << "Welcome to Project MARS!" << std::endl;
    printUsage();
    draw();
    std::string input;
    while (!display->is_closed()) {
        std::cout << "> " << std::flush;
        std::getline(std::cin, input);
        std::vector<std::string> tokens = this->tokenize(input);
        this->doCommand(tokens);
    }
}