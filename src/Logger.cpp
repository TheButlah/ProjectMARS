#include "../include/Logger.h"

using namespace MARS;

Logger::Logger(Game* game, std::string filename):
  game(game),
  log_file(filename)
{
  log_file << "step,unserviced,serviced,ratio,plants,funds,objective,step,px,py" << std::endl;
}

void Logger::log(bool step, Coord plant_loc) 
{
  int total = game->numberServicedPop() + game->numberUnservicedPop();
  int serviced = game->numberServicedPop();
  log_file << game->currentTime() << ",";
  log_file << game->numberUnservicedPop() << ",";
  log_file << game->numberServicedPop() << ",";
  log_file << (total == 0 ? 0.0 : (double)serviced/total) << ",";  
  log_file << game->numberPlantsInService() << ",";      
  log_file << game->currentFunds() << ",";
  log_file << game->calculateObjective() << ",";
  log_file << step << ",";
  log_file << plant_loc.x << ",";  
  log_file << plant_loc.y << ",";      
  log_file << std::endl;  
}