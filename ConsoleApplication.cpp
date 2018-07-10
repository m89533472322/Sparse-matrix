#include "src/SparseMatrix.h"


int main()
{
	// Usage
	SparseMatrix matrixA(3, 3);
	SparseMatrix matrixB(3, 3);
	matrixA.input();
	matrixB.input();

	auto matrixC = matrixA + matrixB;
	auto matrixD = matrixA * matrixB;
	matrixC.print();
	matrixD.print();

	return 0;
}

