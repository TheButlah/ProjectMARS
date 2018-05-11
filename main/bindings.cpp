#include <pybind11/pybind11.h>
#include "../include/Game.h"

namespace py = pybind11;

PYBIND11_PLUGIN(project_mars) {
	py::module m("project_mars");

	py::class_<MARS::Game>(m, "Game")
		.def(py::init<int,
	      int,
	      int,
	      int,
	      double,
	      double,
	      double,
	      double,
	      double>())
		.def("step", &MARS::Game::step);
	return m.ptr();
}