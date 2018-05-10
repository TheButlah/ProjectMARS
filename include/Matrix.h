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
			for (int i = 0; i < rows * cols; i++) {
				matrix[i] = T();
			}
		};

		Matrix(const Matrix& other):
			num_rows(other.num_rows),
			num_cols(other.num_cols),
			matrix(new T[other.num_rows * other.num_cols]) 
		{
			for (int i = 0; i < other.num_rows * other.num_cols; i++) {
				matrix[i] = T(other.matrix[i]);
			}
		}

		Matrix(Matrix& other):
			num_rows(other.num_rows),
			num_cols(other.num_cols),
			matrix(new T[other.num_rows * other.num_cols]) 
		{
			for (int i = 0; i < other.num_rows * other.num_cols; i++) {
				matrix[i] = T(other.matrix[i]);
			}
		}

		Matrix(const Matrix&& other):
			num_rows(other.num_rows),
			num_cols(other.num_cols),
			matrix(new T[other.num_rows * other.num_cols]) 
		{
			for (int i = 0; i < other.num_rows * other.num_cols; i++) {
				matrix[i] = T(other.matrix[i]);
			}
		}

		Matrix(Matrix&& other):
			num_rows(other.num_rows),
			num_cols(other.num_cols),
			matrix(new T[other.num_rows * other.num_cols]) 
		{
			for (int i = 0; i < other.num_rows * other.num_cols; i++) {
				matrix[i] = T(other.matrix[i]);
			}
		}

		Matrix& operator=(const Matrix& other)
		{
			num_rows = other.num_rows;
			num_cols = other.num_cols;
			delete[] matrix;
			matrix = new T[num_rows * num_cols];
			for (int i = 0; i < other.num_rows * other.num_cols; i++) {
				matrix[i] = T(other.matrix[i]);
			}
			return *this;
		}

		/*
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
	  
		int numberRows() {
			return num_rows;
		}

		int numberCols() {
			return num_cols;
		}

		~Matrix() {
			delete[] matrix;
		}
	};
}

#endif
