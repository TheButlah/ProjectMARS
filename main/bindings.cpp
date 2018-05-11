#include <pybind11/pybind11.h>
#include "../include/Coord.h"
#include "../include/Game.h"

namespace py = pybind11;

PYBIND11_PLUGIN(project_mars) {
  py::module m("project_mars");

  py::class_<MARS::Coord>(m, "Coord")
		.def(py::init<int,int>(),
      "Initializer for Coord",
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
      "Initializer for Game",
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
		  "Advance the game's progress by one time step",
		  py::arg("add_plant"),
		  py::arg("plant_coord"));


	return m.ptr();
}