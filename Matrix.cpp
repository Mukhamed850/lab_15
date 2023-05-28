#include <iostream>
#include "Matrix.h"

//-----------------------------//
//--CONSTRUCTORS & DESTRUCTOR--//
//-----------------------------//
template<class data_type>
Matrix<data_type>::Matrix() : num_rows(0), num_columns(0) {}

template<class data_type>
Matrix<data_type>::Matrix(int rows, int columns) : num_rows(rows), num_columns(columns) {
    if(rows < 1 || columns < 1) {
        std::cerr << "ERROR: numbers of rows and columns must be positive!" << std::endl;
        exit(1);
    }
    data = new data_type*[num_rows];
    for(int i = 0; i < num_rows; i++)
        data[i] = new data_type[num_columns];
}

template<class data_type>
Matrix<data_type>::Matrix(int rows, int columns, data_type ** data_input) : num_rows(rows), num_columns(columns) {
    if(rows < 1 || columns < 1) {
        std::cerr << "ERROR: numbers of rows and columns must be positive!" << std::endl;
        exit(1);
    }
    data = new data_type*[num_rows];
    for(int i = 0; i < num_rows; i++) {
        data[i] = new data_type[num_columns];
        for(int j = 0; j < num_columns; j++) {
            data[i][j] = data_input[i][j];
        }
    }
}

template<class data_type>
Matrix<data_type>::Matrix(int rows, int columns, std::string namefile) : num_rows(rows), num_columns(columns) {
    if(rows < 1 || columns < 1) {
        std::cerr << "ERROR: numbers of rows and columns must be positive!" << std::endl;
        exit(1);
    }
    std::ifstream in(namefile);
    if(in.is_open()) {
        data = new data_type*[num_rows];
        for(int i = 0; i < num_rows; i++) {
            data[i] = new data_type[num_columns];
            for(int j = 0; j < num_columns; j++)
                in >> data[i][j];
        }
        in.close();
    }
    else {
        in.close();
        std::cerr << "ERROR: file \"" << namefile << "\" not found!" << std::endl;
        exit(1);
    }
}

template<class data_type>
Matrix<data_type>::~Matrix() {
    for(int i = 0; i < num_rows; i++)
        delete [] data[i];
    delete [] data;
    data = nullptr;
}


//-------------//
//--OPERATORS--//
//-------------//
template<class data_type>
data_type& Matrix<data_type>::operator()(int i, int j) { return data[i][j]; }

template<class data_type>
Matrix<data_type>& Matrix<data_type>::operator=(Matrix &other) {
    if(this->num_rows == other.num_rows && this->num_columns == other.num_columns) {
        for(int i = 0; i < num_rows; i++) {
            for(int j = 0; j < num_columns; j++)
                this->data[i][j] = other.data[i][j];
        }
    }
    else {
        for(int i = 0; i < num_rows; i++)
            delete [] this->data[i];
        delete [] this->data;
        this->num_rows = other.num_rows; this->num_columns = other.num_columns;
        this->data = new data_type*[this->num_rows];
        for(int i = 0; i < this->num_rows; i++) {
            this->data[i] = new data_type[this->num_columns];
            for(int j = 0; j < this->num_columns; j++) {
                this->data[i][j] = other.data[i][j];
            }
        }
    }
    return *this;
}

template<class data_type>
Matrix<data_type>& Matrix<data_type>::operator+(Matrix& other) {
    if(this->num_rows != other.num_rows || this->num_columns != other.num_columns) {
        std::cerr << "ERROR: sum operation is not possible, matrices of different sizes!" << std::endl;
        exit(1);
    }
    for(int i = 0; i < num_rows; i++) {
        for(int j = 0; j < num_columns; j++)
            this->data[i][j] += other.data[i][j];
    }
    return *this;
}

template<class data_type>
Matrix<data_type>& Matrix<data_type>::operator-(Matrix &other) {
    if(this->num_rows != other.num_rows || this->num_columns != other.num_columns) {
        std::cerr << "ERROR: sum operation is not possible, matrices of different sizes!" << std::endl;
        exit(1);
    }
    for(int i = 0; i < num_rows; i++) {
        for(int j = 0; j < num_columns; j++)
            this->data[i][j] -= other.data[i][j];
    }
    return *this;
}

template<class data_type>
Matrix& Matrix::ZeroMatrix(int order) {

}
