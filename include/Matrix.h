#ifndef MARS_MATRIX_H
#define MARS_MATRIX_H

#include <memory>
#include <cstring>
#include <iostream>

namespace MARS {
	/*
	 * Matrix - a 2D matrix of elements
	 */
	template <class T>
	class Matrix {
	private:
		int num_rows; // Number of rows
		int num_cols; // Number of columns
		T* matrix; // Matrix memory

	public:

		/*
		 * Constructor
		 * Takes in number of rows and columns
		 */ 
		Matrix(int rows, int cols):
			num_rows(rows),
			num_cols(cols),
			matrix(new T[rows * cols]) 
		{
		  resetToDefault();
		};

		/** Copy constructor */
		Matrix(const Matrix& other):
			num_rows(other.num_rows),
			num_cols(other.num_cols),
			matrix(new T[other.num_rows * other.num_cols]) 
		{
		  unsigned int items = other.num_rows * other.num_cols;
			for (unsigned int i = 0; i < items; i++) {
				matrix[i] = T(other.matrix[i]);
			}
		}

		/** Move constructor */
		Matrix(Matrix&& other):
			num_rows(other.num_rows),
			num_cols(other.num_cols),
			matrix(other.matrix)
		{
			other.matrix = nullptr;
		}

		/** Copy assignment operator */
		Matrix& operator=(const Matrix& other)
		{
		  if (this != &other) { // Avoid deleting ourselves
        num_rows = other.num_rows;
        num_cols = other.num_cols;
        delete[] matrix;
        unsigned int items = num_rows * num_cols;
        matrix = new T[items];
        for (unsigned int i = 0; i < items; i++) {
          matrix[i] = T(other.matrix[i]);
        }
      }
			return *this;
		}

		/** Move assignment operator */
		Matrix& operator==(Matrix&& other) {
      num_rows = other.num_rows;
      num_cols = other.num_cols;
      delete[] matrix;
      matrix = other.matrix;
      other.matrix = nullptr;
      return *this;
		}
		/**
		 * Access reference to data at location
		 */
		T& at(int r, int c) {
			#ifdef FLAG_MATRIX_BOUNDS_CHECKING
			if (r < 0 || r >= num_rows) 
				throw -1;
			if (c < 0 || c >= num_cols)
				throw -1;
			#endif
			return matrix[r * num_cols + c];
		}

		/**
		 * Access const reference to data at location
		 */
		const T& at(int r, int c) const {
      #ifdef FLAG_MATRIX_BOUNDS_CHECKING
      if (r < 0 || r >= num_rows)
				throw -1;
			if (c < 0 || c >= num_cols)
				throw -1;
      #endif
      return matrix[r * num_cols + c];
    }


    T* ptr() const {
		  return matrix;
		}
	  
		int numberRows() const {
			return num_rows;
		}

		int numberCols() const {
			return num_cols;
		}

		void resetToDefault() {
      std::fill(matrix, matrix + (num_rows*num_cols), T());
		}

		~Matrix() {
			delete[] matrix;
		}
	};
}

#endif
