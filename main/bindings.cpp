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
      "Starts the CLI.")
    .def("draw", &MARS::CLIRepl::draw,
      "Draws the current game state.");

  py::class_<MARS::GameDisplay>(m, "GameDisplay")
    .def(py::init<MARS::Game*, double, double>(),
      "Initializer for GameDisplay",
      py::arg("game"),
      py::arg("box_size"),
      py::arg("pop_scale"))
    .def("update", &MARS::GameDisplay::updateDisplay,
      "Update the display to reflect the current game state");

  py::class_<MARS::Coord>(m, "Coord")
		.def(py::init<int,int>(),
      "Initializer for Coord.",
      py::arg("x"),
      py::arg("y"));

  py::class_<MARS::Matrix<int>>(m, "IntMatrix", py::buffer_protocol())
    .def_buffer([](MARS::Matrix<int>& m) -> py::buffer_info {
      return py::buffer_info(
        m.ptr(),                              /* Pointer to buffer */
        sizeof(int),                          /* Size of one item */
        py::format_descriptor<int>::format(), /* Python struct-style format */
        2,                                    /* Number of dimensions */
        { m.numberRows(), m.numberCols() },   /* Buffer dimensions */
        { sizeof(int) * m.numberCols(),       /* Strides in bytes */
          sizeof(int) }
      );
    });

  py::class_<MARS::Matrix<bool>>(m, "BoolMatrix", py::buffer_protocol())
    .def_buffer([](MARS::Matrix<bool>& m) -> py::buffer_info {
      return py::buffer_info(
        m.ptr(),                               /* Pointer to buffer */
        sizeof(bool),                          /* Size of one item */
        py::format_descriptor<bool>::format(), /* Python struct-style format */
        2,                                     /* Number of dimensions */
        { m.numberRows(), m.numberCols() },    /* Buffer dimensions */
        { sizeof(bool) * m.numberCols(),       /* Strides in bytes */
          sizeof(bool) }
      );
    });

  py::class_<MARS::Matrix<float>>(m, "FloatMatrix", py::buffer_protocol())
    .def_buffer([](MARS::Matrix<float>& m) -> py::buffer_info {
      return py::buffer_info(
        m.ptr(),                                /* Pointer to buffer */
        sizeof(float),                          /* Size of one item */
        py::format_descriptor<float>::format(), /* Python struct-style format */
        2,                                      /* Number of dimensions */
        { m.numberRows(), m.numberCols() },     /* Buffer dimensions */
        { sizeof(float) * m.numberCols(),       /* Strides in bytes */
          sizeof(float) }
      );
    });


	py::class_<MARS::Game> game(m, "Game");
	game
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
    .def("calc_objective", &MARS::Game::calculateObjective,
      "Get the value of the objective fn the current state of the game.")
    .def("get_total_serviced", &MARS::Game::numberServicedPop,
      "Gets the total number of pops that are being serviced by our plants")
    .def_readonly("state", &MARS::Game::rlState);

  py::class_<MARS::Game::RLState> (m, "RLState", game)
    .def_readwrite("unserviced_pops", &MARS::Game::RLState::unservicedPops)
    .def_readwrite("serviced_pops", &MARS::Game::RLState::servicedPops)
    .def_readwrite("total_pops", &MARS::Game::RLState::totalPops)
    .def_readwrite("terrain", &MARS::Game::RLState::terrain)
    .def_readwrite("plants", &MARS::Game::RLState::plantLocs);



	return m.ptr();
}
