#pragma once
#ifndef _MATRIX_H_
#define _MATRIX_H_

template<class data_type>
class Matrix {
private:
    int num_rows;
    int num_columns;
    data_type ** data = nullptr;
public:
    Matrix();
    Matrix(int rows, int columns);
    Matrix(int rows, int columns, data_type ** data_input);
    Matrix(int rows, int columns, std::string namefile);
    ~Matrix();

    data_type&    operator()(int i, int j);
    Matrix&       operator=(Matrix& other);
    Matrix&       operator+(Matrix& other);
    Matrix&       operator-(Matrix& other);
    friend Matrix operator*(data_type scalar, Matrix& other) {
        for(int i = 0; i < other.num_rows; i++) {
            for(int j = 0; j < other.num_columns; j++)
                other.data[i][j] *= scalar;
        }
        return other;
    };
    friend Matrix operator*(Matrix& other, data_type scalar) {
        return scalar * other;
    };
    friend Matrix operator*(Matrix& first, Matrix& second) {
        if(first.num_columns != second.num_rows) {
            std::cerr << "ERROR: the multiplication operation is not possible, the number of columns of the first matrix and the number of rows of the second matrix do not match!" << std::endl;
            exit(1);
        }
        Matrix result(first.num_rows, second.num_columns);
        for(int i = 0; i < result.num_rows; i++) {
            for(int j = 0; j < result.num_columns; j++) {
                data_type sum = 0;
                for(int k = 0; k < first.num_columns; k++)
                    sum += first.data[i][k] * second.data[k][j];
                result.data[i][j] = sum;
            }
        }
        return result;
    }

    static Matrix ZeroMatrix(int order);
    static Matrix UnitMatrix(int order);
};









#endif