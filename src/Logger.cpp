#include "../include/Logger.h"

using namespace MARS;

Logger::Logger(Game* game, std::string filename):
  game(game),
  log_file(filename),
  last_diff(0)
{
  log_file << "step,unserviced,serviced,diff,diff_slope,plants,funds,objective,step,px,py" << std::endl;
}

void Logger::log(bool step, Coord plant_loc) 
{
  int t = game->currentTime();
  int diff = game->numberServicedPop()-game->numberUnservicedPop();
  log_file << game->currentTime() << ",";
  log_file << game->numberUnservicedPop() << ",";
  log_file << game->numberServicedPop() << ",";
  log_file << diff << ",";
  log_file << (t == 1 ? 0 : diff - last_diff) << ",";  
  log_file << game->numberPlantsInService() << ",";      
  log_file << game->currentFunds() << ",";
  log_file << game->calculateObjective() << ",";
  log_file << step << ",";
  log_file << plant_loc.x << ",";  
  log_file << plant_loc.y << ",";      
  log_file << std::endl;  
  last_diff = diff;
}