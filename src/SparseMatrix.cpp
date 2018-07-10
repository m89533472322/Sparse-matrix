#include "SparseMatrix.h"
#include <unordered_map>
#include <iostream>


template<typename T>
static inline void appendVector(vector<T>& dst, const vector<T>& src)
{
	dst.insert(dst.end(), src.begin(), src.end());
}


SparseMatrix::SparseMatrix(uint32_t rows, uint32_t columns) :
	rowsCount(rows),
	colsCount(columns),
	rowsPointers(rows + 1, 0)
{
	if (rowsCount == 0 || colsCount == 0)
		throw SparseMatrixException("Wrong dimensions.");
}


void SparseMatrix::print()
{
	for (uint32_t row = 0; row < rowsCount; row++)
	{
		for (uint32_t i = beginIndex(row); i < endIndex(row); i++)
		{
			cout << '[' << row << ',' << columns[i] << ']' << " = " << values[i] << endl;
		}
	}
}


void SparseMatrix::input()
{
	uint32_t row = 0;
	uint32_t col = 0;
	int value = 0;

	while (true)
	{
		cout << "Input format: \"row column value\"\n";
		cin >> row >> col >> value;

		if (cin.fail())
		{
			cin.clear();
			cin.ignore();
			break;
		}

		if (row >= rowsCount || col >= colsCount)
			throw SparseMatrixException("Coordinates out of range.");

		setItem(row, col, value);
	}
}


SparseMatrix SparseMatrix::transpose() const
{
	SparseMatrix result(colsCount, rowsCount);
	vector<vector<uint32_t>> rowsVectors;
	vector<vector<int>> valuesVectors;

	rowsVectors.resize(colsCount);
	valuesVectors.resize(colsCount);

	for (uint32_t row = 0; row < rowsCount; row++)
	{
		for (uint32_t i = beginIndex(row); i < endIndex(row); i++)
		{
			uint32_t col = columns[i];
			rowsVectors[col].push_back(row);
			valuesVectors[col].push_back(values[i]);
		}
	}

	for (uint32_t col = 0; col < colsCount; col++)
	{
		appendVector(result.columns, rowsVectors[col]);
		appendVector(result.values, valuesVectors[col]);
		result.rowsPointers[col + 1] = result.rowsPointers[col] + rowsVectors[col].size();
	}

	return result;
}


SparseMatrix SparseMatrix::operator+(const SparseMatrix& right) const
{
	if (rowsCount != right.rowsCount || colsCount != right.colsCount)
		throw SparseMatrixException("Wrong dimensions.");

	SparseMatrix result(*this);

	for (uint32_t row = 0; row < rowsCount; row++)
	{
		// Save columns with nonzero values from the right matrix.
		unordered_map<uint32_t, int> columnValues;

		for (uint32_t i = right.beginIndex(row); i < right.endIndex(row); i++)
		{
			columnValues[right.columns[i]] = right.values[i];
		}

		// Sum saved values with values in the corresponding columns of the left matrix.
		// Remove the handled value from the container.
		for (uint32_t i = beginIndex(row); i < endIndex(row); i++)
		{
			uint32_t column = columns[i];

			if (columnValues.find(column) != columnValues.end())
			{
				//result.values[i] += columnValues[column];
				result.setItem(row, column, values[i] + columnValues[column]);
				columnValues.erase(column);
			}
		}

		// Add remaining values to the result matrix.
		for (const auto& item : columnValues)
			result.setItem(row, item.first, item.second);
	}

	return result;
}


SparseMatrix SparseMatrix::operator*(const SparseMatrix& right) const
{
	if (colsCount != right.rowsCount)
		throw SparseMatrixException("Wrong dimensions.");

	SparseMatrix result(rowsCount, right.colsCount);
	SparseMatrix rightT = right.transpose();

	for (uint32_t row = 0; row < rowsCount; row++)
	{
		// Save columns with nonzero values from the left matrix.
		unordered_map<uint32_t, int> columnValues;

		for (uint32_t i = beginIndex(row); i < endIndex(row); i++)
		{
			columnValues[columns[i]] = values[i];
		}

		// Multiply saved values with corresponding values of the transposed right matrix.
		for (uint32_t col = 0; col < rightT.rowsCount; col++)
		{
			int multiplicationResult = 0;

			for (uint32_t i = rightT.beginIndex(col); i < rightT.endIndex(col); i++)
			{
				uint32_t column = rightT.columns[i];

				if (columnValues.find(column) != columnValues.end())
					multiplicationResult += columnValues[column] * rightT.values[i];
			}

			if (multiplicationResult != 0)
			{
				result.setItem(row, col, multiplicationResult);
			}
		}
	}

	return result;
}


void SparseMatrix::setItem(uint32_t row, uint32_t col, int value)
{
	uint32_t index = 0;

	for (index = beginIndex(row); index < endIndex(row); index++)
	{
		if (columns[index] == col)
		{
			// Element(row, col) already exist. Just assign new value.
			values[index] = value;
			return;
		}
		else if (columns[index] > col)
		{
			break;
		}
	}

	values.insert(values.begin() + index, value);
	columns.insert(columns.begin() + index, col);

	for (uint32_t i = row + 1; i < rowsPointers.size(); i++)
		rowsPointers[i] += 1;
}



uint32_t SparseMatrix::beginIndex(uint32_t row) const
{
	if (rowsPointers.size() != rowsCount + 1)
		throw SparseMatrixException("Wrong matrix representation: invalid rows pointers.");

	if (row >= rowsCount)
		throw SparseMatrixException("Row out of range.");
	
	return rowsPointers[row];
}



uint32_t SparseMatrix::endIndex(uint32_t row) const
{
	if (rowsPointers.size() != rowsCount + 1)
		throw SparseMatrixException("Wrong matrix representation: invalid rows pointers.");

	if (row >= rowsCount)
		throw SparseMatrixException("Row out of range.");

	return rowsPointers[row + 1];
}
