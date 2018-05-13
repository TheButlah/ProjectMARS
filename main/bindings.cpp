#include <pybind11/pybind11.h>
#include <string>

#include "Coord.h"
#include "Game.h"
#include "CLIRepl.h"
#include "Matrix.h"

namespace py = pybind11;



PYBIND11_PLUGIN(project_mars) {
  py::module m("project_mars");

  py::class_<MARS::CLIRepl>(m, "CLIRepl")
    .def(py::init<MARS::Game*>(),
      "Initializer for CLIRepl.",
      py::arg("game"))
    .def(py::init<std::string>(),
      "Initializer for CLIRepl.",
      py::arg("inifile"))
    .def("startCLI", &MARS::CLIRepl::startCLI,
      "Starts the CLI.");


  /*py::class_<MARS::Game::RLState>(m, "RLState")
    .def("");*/


  py::class_<MARS::Coord>(m, "Coord")
		.def(py::init<int,int>(),
      "Initializer for Coord.",
      py::arg("x"),
      py::arg("y"));


	py::class_<MARS::Game>(m, "Game")
		.def(py::init<
        int,
        int,
        int,
        int,
        double,
        double,
        double,
        double,
        double>(),
      "Initializer for Game.",
      py::arg("dx"),
      py::arg("dy"),
      py::arg("number_of_turns"),
      py::arg("default_capacity"),
      py::arg("servable_distance"),
      py::arg("initial_cost"),
      py::arg("operating_cost"),
      py::arg("profit_margin"),
      py::arg("unserviced_penalty"))
		.def("step", &MARS::Game::step,
		  "Advance the game's progress by one time step.",
		  py::arg("add_plant"),
		  py::arg("plant_coord"))
    .def("get_reward", &MARS::Game::calculateObjective,
      "Get the reward value for the current state of the game.")
    .def("get_env_state" &MARS::Game::getRLState,
      "Gets the state for the RL agent.");

	return m.ptr();
}