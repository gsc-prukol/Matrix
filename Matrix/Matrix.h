#pragma once

#include "Macros.h"
#include <stdexcept>

class Matrix
{
public:
	explicit Matrix(int height, int widht);
	Matrix(std::initializer_list<std::initializer_list<double>>);
	Matrix(const Matrix& m);
	Matrix(Matrix&&) noexcept;

	~Matrix();

private:
	int _height, _width, _size;
	double* _values;

public:
	Matrix& operator = (const Matrix&);
	Matrix& operator = (Matrix&&) noexcept;

	double get_value(int i, int j) const;
	double set_value(const double value, int i, int j);
	int get_height() const { return _height; }
	int get_width() const { return _width; }

	std::string to_string();
private:
	void set_height(int h);
	void set_width(int w);
	void allocate_memory() { _values = new double[_size]; }
	void update_size() { _size = _height * _width; }

	void copy_values(double* v)
	{
		for (int k = 0; k < _size; k++)
		{
			_values[k] = v[k];
		}
	}

	void validation_i(int i) const { if (i <= 0 || i > _height) { throw std::out_of_range(nameof(i)); } }
	void validation_j(int j) const { if (j <= 0 || j > _width) { throw std::out_of_range(nameof(j)); } }
};

double multiply_row_by_column(const Matrix& left, const int i, const Matrix& right, const int j);
Matrix operator * (const Matrix& left, const Matrix& right);