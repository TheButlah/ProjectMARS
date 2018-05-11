#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <exception>

#include "CLIRepl.h"
#include "INIReader.h"
#include "Matrix.h"
#include "Terrain.h"
#include "Coord.h"
#include "Clustering.h"


using namespace MARS;
using namespace cimg_library;

#define BOX_SIZE 20
#define POP_SCALE 50.0

/**
 * Creates a CLI.
 *
 * @param inifile The file to initialze a new `Game` with.
 */
CLIRepl::CLIRepl(std::string inifile)
{
  INIReader ini(inifile);
  int dx = ini.GetInteger("Default", "SizeX", 16);
  int dy = ini.GetInteger("Default", "SizeY", 16);
  int number_of_turns = ini.GetInteger("Default", "NumberOfTurns", 1000000);
  int default_capacity = ini.GetInteger("Default", "PlantCapacity", 25);
  double servable_distance = ini.GetReal("Default", "PlantServableDistance", 3.0);
  double initial_cost = ini.GetReal("Default", "PlantInitialCost", 50.0);
  double operating_cost = ini.GetReal("Default", "PlantOperatingCost", 25.0);            
  double profit_margin = ini.GetReal("Default", "PlantProfitMargin", 5.0);
  double unserviced_penalty = ini.GetReal("Default", "UnservicedPenalty", 1.0);
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
    profit_margin,
    unserviced_penalty);
  game_display = new GameDisplay(game, 15, 50);
}

CLIRepl::CLIRepl(MARS::Game *game) {
  if (game == nullptr) {
    throw std::exception();
  }
  this->game = game;
  auto size = game->getSize();
  size_x = size.first;
  size_y = size.second;
  game_display = new GameDisplay(game, 15, 50);
}

CLIRepl::~CLIRepl() {
  delete game;
  delete game_display;
}

std::vector<std::string> CLIRepl::tokenize(std::string s) {
  std::vector<std::string> result;
  std::istringstream iss(s);
  for(std::string s2; iss >> s2;)
    result.push_back(s2);
  return result;
}

void CLIRepl::draw() {
  game_display->updateDisplay();
}

void CLIRepl::printUsage() {
  std::cout << "Commands:" << std::endl;
  std::cout << "step - steps the game without making a plant" << std::endl;
  std::cout << "step x - steps the game `x' times without making a plant" << std::endl;
  std::cout << "step plant r c - steps the game while making a plant at location (r, c)" << std::endl;
  std::cout << "cluster k - runs K-means clustering with k clusters to create new plant" << std::endl;
  std::cout << "help - print this list of commands" << std::endl;
  std::cout << "log x s k /path/to/file.csv - steps x times, clusters (with k clusters) every s steps, logs output as CSV" << std::endl;
  std::cout << "log /path/to/params.txt /path/to/dir - log using params.txt, writing output in dir" << std::endl;  
}

void CLIRepl::printStats() {

}

void CLIRepl::stepWithClustering(int k) {
  std::pair<bool, Coord> res = Clustering::placePlantKMeans(game->getPopMatrix(), k);
  game->step(res.first, res.second);
}

void CLIRepl::stepWithRandom() {
  std::pair<bool, Coord> res = Clustering::placePlantRandom(game->getPopMatrix());
  game->step(res.first, res.second);
}

void CLIRepl::loggingLoop(int steps, int decision_interval, int k, std::string path) {
  std::ofstream out;

  out.open(path, std::ios_base::app);
  out << "Time,Ran Clustering?,Number of Plants,Objective" << std::endl;

  for(int i = 0; i < steps; i++) {
    bool run_clustering = i != 0 && i % decision_interval == 0;

    if(run_clustering) {
      game->step(false, Coord(0, 0));
    }
    else {
      this->stepWithClustering(k);
    }

    std::string ran_clustering_s = run_clustering ? "yes" : "";

    std::string data = std::to_string(i) + "," + ran_clustering_s + "," 
      + std::to_string(game->getNumberPlantsInService()) + "," 
      + std::to_string(game->calculateObjective());
    out << data << std::endl;

  }
}

void CLIRepl::loggingFromFile(std::string params_path, std::string output_dir_path) {
  std::ifstream in;

  in.open(params_path);

  int x, s, k;

  while(in >> x >> s >> k) {
    std::string path = output_dir_path + "/" + "kmeans" + "_" 
      + std::to_string(x) + "_" + std::to_string(s) + "_" + std::to_string(k) + ".csv";
    std::cout << "Running: k-means x=" << x << " s=" << s << " k=" << k << std::endl;
    this->loggingLoop(x, s, k, path);
  }

}

void CLIRepl::doCommand(std::vector<std::string> tokens) {
  if (tokens.size() == 0) return;
  std::string command = tokens[0];
  if (command == "help") {
    this->printUsage();
  } else if (command == "step") {
    if (tokens.size() == 1) {
      game->step(false, Coord(0, 0));
    } else if (tokens.size() == 2) {
      int times = std::stoi(tokens[1]);
      for (int i = 0; i < times; i++) {
        game->step(false, Coord(0, 0));                
      }
    } else if (tokens.size() == 4 && tokens[1] == "plant") {
      int r = std::stoi(tokens[2]);
      int c = std::stoi(tokens[3]);
      game->step(true, Coord(r, c));
    } else {
      std::cout << "step: Did not provide correct number of arguments" << std::endl;
    }
  } else if (command == "stats") {
    this->printUsage();
  } else if (tokens.size() == 2 && tokens[0] == "cluster") {
    int k = std::stoi(tokens[1]);
    this->stepWithClustering(k);
  } else if(command == "exit" || command == "quit") {
    exit(0);
  }
  else if(command == "log") {
    if(tokens.size() == 3) {
      std::string params_path = tokens[1];
      std::string output_dir_path = tokens[2];
      this->loggingFromFile(params_path, output_dir_path);
    } else if(tokens.size() == 5) {
      int x = std::stoi(tokens[1]);
      int s = std::stoi(tokens[2]);
      int k = std::stoi(tokens[3]);
      std::string path = tokens[4];
      this->loggingLoop(x, s, k, path);
    } else {
      std::cout << "log: Did not provide correct number of arguments" << std::endl;
    }
  }
  else {
    this->printUsage();
  }
  draw();
}

void CLIRepl::startCLI() {
  std::cout << "Welcome to Project MARS!" << std::endl;
  printUsage();
  draw();
  std::string input;
  while (true) {
    std::cout << "> " << std::flush;
    std::getline(std::cin, input);
    std::vector<std::string> tokens = this->tokenize(input);
    this->doCommand(tokens);
  }
}