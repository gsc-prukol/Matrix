#include <iostream>
#include "Matrix.h"

using namespace std;

int main()
{
	Matrix left {
		{1, 1, 1},
		{2, 2, 2},
		{3, 3, 3}
	};

	Matrix right {
		{1, 2, 3},
		{1, 2, 3},
		{1, 2, 3}
	};

	cout << left.to_string() << "\n\n";
	cout << right.to_string() << "\n\n";

	Matrix multipl = left * right;

	cout << multipl.to_string() << "\n\n";
}