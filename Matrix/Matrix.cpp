#include "Matrix.h"
#include <stdexcept>
#include <string>
#include "Macros.h"

Matrix::Matrix(int h, int w)
{
	set_height(h);
	set_width(w);

	allocate_memory();
}

Matrix::Matrix(std::initializer_list<std::initializer_list<double>> list)
{
	int height = list.size();

	if (height == 0)
	{
		throw std::invalid_argument("The number of rows must be greater than 0");
	}

	set_height(static_cast<int>(height));

	_width = 0;
	int width = 0;

	int i = 1, j;
	for (auto iter = list.begin(); iter != list.end(); iter++)
	{
		j = 1;

		width = static_cast<int>(iter->size());

		if (width == 0)
		{
			throw std::invalid_argument("The number of columns must be greater than 0");
		}

		if (_width == 0)
		{	
			set_width(width);
			allocate_memory();
		}
		
		if (width != _width)
		{
			throw std::invalid_argument("The number of columns must be the same for all rows");
		}

		for (auto value = iter->begin(); value != iter->end(); value++)
		{
			set_value(*value, i, j++);
		}

		i++;
	}

}

Matrix::Matrix(const Matrix& m)
{
	set_height(m._height);
	set_width(m._width);

	allocate_memory();

	copy_values(m._values);
}

Matrix::Matrix(Matrix&& m) noexcept
{
	_height = m._height;
	_width = m._width;
	_size = m._size;
	_values = m._values;

	m._height = 0;
	m._width = 0;
	m._size = 0;
	m._values = nullptr;
}

Matrix::~Matrix()
{
	delete[] _values;
}

Matrix& Matrix::operator=(const Matrix& m)
{
	delete[] _values;
	_values = new double[m._size];
	
	set_height(m._height);
	set_width(m._width);

	copy_values(m._values);
	
	return *this;
}

Matrix& Matrix::operator=(Matrix&& m) noexcept
{
	delete[] _values;
	
	_height = m._height;
	_width = m._width;
	_size = m._size;
	_values = m._values;

	m._height = 0;
	m._width = 0;
	m._size = 0;
	m._values = nullptr;

	return *this;
}

double Matrix::get_value(int i, int j) const
{
	// 1  j  w
	//+-------+
	//|       |1
	//|   v   |i
	//|       |h
	//+-------+
	//

	validation_i(i);
	validation_j(j);

	return _values[--i * _width + --j];
}

double Matrix::set_value(const double value, int i, int j)
{
	validation_i(i);
	validation_j(j);

	_values[--i * _width + --j] = value;
	return value;
}

std::string Matrix::to_string()
{
	using namespace std;
	
	string result = ""s;
	
	for (int i = 1; i <= _height; i++)
	{
		for (int j = 1; j <= _width; j++)
		{
			result += std::to_string(get_value(i, j)) + "\t"s;
		}
		
		result += "\n";
	}

	return result;
}

void Matrix::set_height(int h)
{
	if (h <= 0)
	{
		throw std::out_of_range(nameof(h));
	}

	_height = h;
	update_size();
}

void Matrix::set_width(int w)
{
	if (w <= 0)
	{
		throw std::out_of_range(nameof(w));
	}

	_width = w;
	update_size();
}

//    L               R
// 1  j  w		// 1  j  w
//+-------+		//+-------+
//|       |1	//|       |1
//|   v   |i	//|   v   |i
//|       |h	//|       |h
//+-------+		//+-------+
//

double multiply_row_by_column(const Matrix& left, const int i, const Matrix& right, const int j)
{
	double result = 0.0;

	int sizeVector = left.get_height();

	for (int k = 1; k <= sizeVector; k++)
	{
		result += left.get_value(i, k) * right.get_value(k, j);
	}

	return result;
}

Matrix operator * (const Matrix& left, const Matrix& right)
{
	int lheight = left.get_height();
	int rwidth = right.get_width();


	if (lheight == 0)
	{
		throw std::invalid_argument(nameof(left));
	}

	if (rwidth == 0)
	{
		throw std::invalid_argument(nameof(right));
	}

	int rheight = right.get_height();
	int lwidth = left.get_width();

	if (rheight == 0 || rheight != lwidth)
	{
		using namespace std::string_literals;
		throw std::invalid_argument("Width left argument must be equal to height right argument"s);
	}

	Matrix result { lheight, rwidth };

	for (int i = 1; i <= rheight; i++)
	{
		for (int j = 1; j <= lwidth; j++)
		{
			result.set_value(
				multiply_row_by_column(left, i, right, j),
				i,
				j);
		}
	}

	return result;
}