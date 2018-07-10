#ifndef SPARSE_MATRIX_H
#define SPARSE_MATRIX_H

#include "SparseMatrixException.h"
#include <vector>

using namespace std;


class SparseMatrix
{
public:
	SparseMatrix(uint32_t rows, uint32_t columns);

	void print();
	void input();

	SparseMatrix transpose() const;
	SparseMatrix operator+(const SparseMatrix& other) const;
	SparseMatrix operator*(const SparseMatrix& right) const;

private:
	vector<int> values;
	vector<uint32_t> rowsPointers;
	vector<uint32_t> columns;
	const uint32_t rowsCount;
	const uint32_t colsCount;

	void setItem(uint32_t row, uint32_t col, int value);
	uint32_t beginIndex(uint32_t row) const;
	uint32_t endIndex(uint32_t row) const;

};


#endif // SPARSE_MATRIX_H
