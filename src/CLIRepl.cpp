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
  this->inifile = inifile;
  this->initializeGame(inifile);
}

void CLIRepl::initializeGame(std::string inifile) {
  INIReader ini(inifile);
  int dx = ini.GetInteger("Default", "SizeX", 16);
  int dy = ini.GetInteger("Default", "SizeY", 16);
  int number_of_turns = ini.GetInteger("Default", "NumberOfTurns", 1000000);
  int default_capacity = ini.GetInteger("Default", "PlantCapacity", 2500);
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
  auto size = game->sizeXY();
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
  std::cout << "kmeans x s k /path/to/file.csv - steps x times, clusters (with k-means) every s steps, logs output as CSV" << std::endl;
  std::cout << "kmedians x s k /path/to/file.csv - steps x times, clusters (with k-medians) every s steps, logs output as CSV" << std::endl;
  std::cout << "random x s /path/to/file.csv - steps x times, places plants randomly every s steps, logs output as CSV" << std::endl;
  std::cout << "trace /path/to/trace.txt - run trace file" << std::endl;  
}

void CLIRepl::printStats() {

}

void CLIRepl::stepWithKMeans(int k) {
  std::pair<bool, Coord> res = Clustering::placePlantKMeans(game->popMatrixCopy(), k);
  game->step(res.first, res.second);
}

void CLIRepl::stepWithKMedians(int k) {
  std::pair<bool, Coord> res = Clustering::placePlantKMedians(game->popMatrixCopy(), k);
  game->step(res.first, res.second);
}

void CLIRepl::stepWithRandom() {
  std::pair<bool, Coord> res = Clustering::placePlantRandom(game->popMatrixCopy());
  game->step(res.first, res.second);
}

void CLIRepl::placePlantLoop(std::string method, int steps, int decision_interval, int k, std::string path) {
  
  if(method != "kmeans" && method != "kmedians" && method != "random") {
    std::cout << "Invalid method. Please use either kmeans, kmedians, or random." << std::endl;
  }

  this->initializeGame(this->inifile);
  std::ofstream out;

  out.open(path, std::ios_base::app);
  out << "Time,Ran Algorithm?,Number of Plants,Objective" << std::endl;

  for(int i = 0; i < steps; i++) {
    bool run_clustering = i != 0 && i % decision_interval == 0;

    if(!run_clustering) {
      game->step(false, Coord(0, 0));
    }
    else {
      if(method == "kmeans") {
        this->stepWithKMeans(k);
      }
      else if(method == "kmedians") {
        this->stepWithKMedians(k);
      }
      else if(method == "random") {
        this->stepWithRandom();
      }    
    }

    std::string ran_clustering_s = run_clustering ? "yes" : "";

    std::string data = std::to_string(i) + "," + ran_clustering_s + "," 
      + std::to_string(game->numberPlantsInService()) + "," 
      + std::to_string(game->calculateObjective());
    out << data << std::endl;

  }
}

void CLIRepl::runTrace(std::string params_path) {
  std::ifstream in;
  in.open(params_path);

  std::string line;

  while(std::getline(in, line)) {
    std::cout << line << std::endl;
    std::vector<std::string> tokens = this->tokenize(line);
    this->doCommand(tokens);
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
    this->stepWithKMeans(k);
  } else if(command == "exit" || command == "quit") {
    exit(0);
  }
  else if(command == "trace" && tokens.size() == 2) {
      std::string trace_path = tokens[1];
      this->runTrace(trace_path);
  } else if(command == "kmeans" && tokens.size() == 5) {
    int x = std::stoi(tokens[1]);
    int s = std::stoi(tokens[2]);
    int k = std::stoi(tokens[3]);
    std::string path = tokens[4];
    this->placePlantLoop("kmeans", x, s, k, path);
  } else if(command == "kmedians" && tokens.size() == 5) {
    int x = std::stoi(tokens[1]);
    int s = std::stoi(tokens[2]);
    int k = std::stoi(tokens[3]);
    std::string path = tokens[4];
    this->placePlantLoop("kmedians", x, s, k, path);
  } else if(command == "random" && tokens.size() == 4) {
    int x = std::stoi(tokens[1]);
    int s = std::stoi(tokens[2]);
    std::string path = tokens[3];
    this->placePlantLoop("random", x, s, 0, path);
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