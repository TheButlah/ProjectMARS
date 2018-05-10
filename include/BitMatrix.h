#ifndef MARS_BITMATRIX_H
#define MARS_BITMATRIX_H

#include "Matrix.h"

namespace MARS {
  /*
   * BitMatrix - 2D matrix of binary values, compressed in space
   */
  class BitMatrix {
  private:
    int num_rows; // Number of rows
    int num_cols; // Number of columns
    Matrix<int> matrix; // Matrix of integers to hold bit state
  public: 
    /*
     * Constructor
     * Takes in number of rows and columns
     */
    BitMatrix(int rows, int cols);

    /*
     * Get the value at a row and column
     */
    bool get(int r, int c);
    /*
     * Set the value at a row an column
     */
    void set(int r, int c, bool val);

    int numRows();
    int numCols();
  };
}

#endif
