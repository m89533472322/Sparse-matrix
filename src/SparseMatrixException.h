#ifndef SPARSE_MATRIX_EXCEPTION_H
#define SPARSE_MATRIX_EXCEPTION_H

#include <exception>
#include <string>

using namespace std;


class SparseMatrixException : public exception
{
	string message;

public:

	explicit SparseMatrixException(const string& message) :
		exception(),
		message(message)
	{}

	string getMessage(void) const
	{
		return message;
	}
};


#endif // SPARSE_MATRIX_EXCEPTION_H