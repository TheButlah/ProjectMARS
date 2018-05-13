#include "../include/BitMatrix.h"

using namespace MARS;

BitMatrix::BitMatrix(int rows, int cols): 
  num_rows(rows),
  num_cols(cols),
  matrix(rows, cols / (8 * sizeof(int)) + 1) 
{
}

bool BitMatrix::get(int r, int c) const {
  int cd = c / (8 * sizeof(int));
  int cr = c % (8 * sizeof(int));

  return (matrix.at(r, cd) & (1 << cr)) != 0;
}

void BitMatrix::set(int r, int c, bool val) {
  int cd = c / (8 * sizeof(int));
  int cr = c % (8 * sizeof(int));

  if (val) {
    matrix.at(r, cd) |= (1 << cr);
  } else {
    matrix.at(r, cd) &= ~(1 << cr);
  }
}

int BitMatrix::numRows() const {
  return num_rows;
}

int BitMatrix::numCols() const {
  return num_cols;
}
