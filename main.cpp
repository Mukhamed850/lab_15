#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <thread>

class Matrix {
private:
    int num_rows;
    int num_columns;
    double ** data = nullptr;

    double p_get_det(int start_row, int end_row, int end_col, std::unordered_set<int>& excluded_cols) const;
    Matrix p_get_inverse() const;

    double p_get_det_thread() const;

public:
    Matrix();
    Matrix(int rows, int columns);
    Matrix(int rows, int columns, double ** data_input);
    Matrix(int rows, int columns, std::string namefile);
    ~Matrix();
    int     get_num_rows() const;
    int     get_num_columns() const;
    double  get_det() const;
    Matrix  get_transposition() const;

    static void print(Matrix& other);
    static Matrix ZeroMatrix(int order);
    static Matrix UnitMatrix(int order);

    double& operator()(int i, int j);
    Matrix& operator=(const Matrix& other);
    Matrix& operator=(Matrix other);
    Matrix  operator+(const Matrix& other) const;
    Matrix  operator-(const Matrix& other) const;
    Matrix  operator*(double scalar) const;
    Matrix  operator/(double scalar) const;
    bool    operator==(const Matrix& other) const;
    bool    operator==(int zero_or_unit) const;
    Matrix  operator!() const {  // get inverse matrix
        if(num_rows != num_columns) {
            std::cerr << "ERROR: It is impossible to calculate the inverse matrix, this matrix is not square!" << std::endl;
            exit(1);
        }
        return p_get_inverse();
    }

    friend Matrix operator*(double scalar, const Matrix& other);
    friend Matrix operator*(Matrix& other, double scalar) { return scalar * other; }
    friend Matrix operator*(Matrix& first, Matrix& second);

    // THREADS //

    double get_det_thread() const;

};






int main() {
    int r, c;
    std::cin >> r >> c;
    Matrix matrix(r, c);
    for(int i = 0; i < r; i++) {
        for(int j = 0; j < c; j++)
            std::cin >> matrix(i, j);
    }
    std::cout << std::endl;
    Matrix result(Matrix::UnitMatrix(5));
    std::cout << (result == 1) << std::endl;

    return 0;
}



//-----------------------------//
//--CONSTRUCTORS & DESTRUCTOR--//
//-----------------------------//

Matrix::Matrix() : num_rows(0), num_columns(0) {}

Matrix::Matrix(int rows, int columns) : num_rows(rows), num_columns(columns) {
    if (rows < 1 || columns < 1) {
        std::cerr << "ERROR: numbers of rows and columns must be positive!" << std::endl;
        exit(1);
    }
    data = new double *[num_rows];
    for (int i = 0; i < num_rows; i++)
        data[i] = new double[num_columns];
}

Matrix::Matrix(int rows, int columns, double ** data_input) : num_rows(rows), num_columns(columns) {
    if(rows < 1 || columns < 1) {
        std::cerr << "ERROR: numbers of rows and columns must be positive!" << std::endl;
        exit(1);
    }
    data = new double*[num_rows];
    for(int i = 0; i < num_rows; i++) {
        data[i] = new double[num_columns];
        for(int j = 0; j < num_columns; j++) {
            data[i][j] = data_input[i][j];
        }
    }
}

Matrix::Matrix(int rows, int columns, std::string namefile) : num_rows(rows), num_columns(columns) {
    if(rows < 1 || columns < 1) {
        std::cerr << "ERROR: numbers of rows and columns must be positive!" << std::endl;
        exit(1);
    }
    std::ifstream in(namefile);
    if(in.is_open()) {
        data = new double*[num_rows];
        for(int i = 0; i < num_rows; i++) {
            data[i] = new double[num_columns];
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

Matrix::~Matrix() {
    for(int i = 0; i < num_rows; i++)
        delete [] data[i];
    delete [] data;
    data = nullptr;
}


//-----------//
//--METHODS--//
//-----------//

int Matrix::get_num_rows() const { return num_rows; }
int Matrix::get_num_columns() const { return num_columns; }

double Matrix::get_det() const {
    if(num_rows != num_columns) {
        std::cerr << "ERROR: it is impossible to calculate the determinant, the matrix is not square!" << std::endl;
        exit(1);
    }
    std::unordered_set<int> excluded_cols = {};
    return p_get_det(0, num_rows - 1, num_columns - 1, excluded_cols);
}

Matrix Matrix::get_transposition() const {
    Matrix result(num_columns, num_rows);
    for(int i = 0; i < num_rows; i++) {
        for(int j = 0; j < num_columns; j++)
            result.data[j][i] = this->data[i][j];
    }
    return result;
}

void Matrix::print(Matrix& other) {
    for(int i = 0; i < other.num_rows; i++) {
        for(int j = 0; j < other.num_columns; j++)
            std::cout << other.data[i][j] << " ";
        std::cout << std::endl;
    }
}
Matrix Matrix::UnitMatrix(int order) {
    Matrix result(order, order);
    for(int i = 0; i < order; i++) {
        for(int j = i; j < order; j++) {
            result.data[i][j] = (i == j) ? 1 : 0;
            result.data[j][i] = result.data[i][j];
        }
    }
    return result;
}
Matrix Matrix::ZeroMatrix(int order) {
    Matrix result(order, order);
    for(int i = 0; i < order; i++) {
        for(int j = i; j < order; j++) {
            result.data[i][j] = 0;
            result.data[j][i] = 0;
        }
    }
    return result;
}

double Matrix::p_get_det(int start_row, int end_row, int end_col, std::unordered_set<int>& excluded_cols) const {
    if(start_row == end_row) {
        for(int j = 0; j <= end_col; j++) {
            if(!excluded_cols.count(j))
                return data[start_row][j];
        }
    }

    double result  = 0;
    int col_parity = 0;
    for(int j = 0; j <= end_col; j++) {
        if(!excluded_cols.count(j)) {
            excluded_cols.insert(j);
            result += ((col_parity++ % 2 == 0) ? 1 : -1) * this->data[start_row][j] * p_get_det(start_row + 1, end_row, end_col, excluded_cols);
            excluded_cols.erase(j);
        }
    }
    return result;
}
Matrix Matrix::p_get_inverse() const {
    double det = get_det();
    if(!det) {
        std::cout << "ERROR: it is impossible to calculate the inverse matrix, the matrix is degenerate (det = 0)!" << std::endl;
        exit(1);
    }
    Matrix transpose(this->get_transposition());
    Matrix union_matrix(transpose);
    for(int i = 0; i < union_matrix.num_rows; i++) {
        for(int j = 0; j < union_matrix.num_columns; j++) {
            Matrix lower(union_matrix.num_rows - 1, union_matrix.num_columns - 1);
            int l_i = 0;
            for(int ii = 0; ii < union_matrix.num_rows; ii++) {
                int l_j = 0;
                bool flag = false;
                for(int jj = 0; jj < union_matrix.num_columns; jj++) {
                    if(ii != i && jj != j) {
                        flag = true;
                        lower(l_i, l_j++) = transpose(ii, jj);
                    }
                }
                if(flag)
                    l_i++;
            }
            union_matrix(i, j) = (((i + j) % 2 == 0) ? 1 : -1) * lower.get_det();
        }
    }
    return union_matrix / det;
}




//-------------//
//--OPERATORS--//
//-------------//

double& Matrix::operator()(int i, int j) { return data[i][j]; }

Matrix& Matrix::operator=(const Matrix &other) {
    if(this->num_rows == other.num_rows && this->num_columns == other.num_columns) {
        for(int i = 0; i < num_rows; i++) {
            for(int j = 0; j < num_columns; j++)
                this->data[i][j] = other.data[i][j];
        }
    }
    else {
        for(int i = 0; i < this->num_rows; i++) {
            delete [] data[i];
        }
        delete [] data;
        this->num_rows = other.num_rows;
        this->num_columns = other.num_columns;
        data = new double*[num_rows];
        for(int i = 0; i < num_rows; i++) {
            data[i] = new double[num_columns];
            for(int j = 0; j < num_columns; j++) {
                this->data[i][j] = other.data[i][j];
            }
        }
    }
    return *this;
}

Matrix& Matrix::operator=(Matrix other) {
    if(this->num_rows == other.num_rows && this->num_columns == other.num_columns) {
        for(int i = 0; i < num_rows; i++) {
            for(int j = 0; j < num_columns; j++)
                this->data[i][j] = other.data[i][j];
        }
    }
    else {
        for(int i = 0; i < this->num_rows; i++) {
            delete [] data[i];
        }
        delete [] data;
        this->num_rows = other.num_rows;
        this->num_columns = other.num_columns;
        data = new double*[num_rows];
        for(int i = 0; i < num_rows; i++) {
            data[i] = new double[num_columns];
            for(int j = 0; j < num_columns; j++) {
                this->data[i][j] = other.data[i][j];
            }
        }
    }
    return *this;
}

Matrix Matrix::operator+(const Matrix& other) const {
    if(this->num_rows != other.num_rows || this->num_columns != other.num_columns) {
        std::cerr << "ERROR: sum operation is not possible, matrices of different sizes!" << std::endl;
        exit(1);
    }
    Matrix result(num_rows, num_columns);
    for(int i = 0; i < num_rows; i++) {
        for(int j = 0; j < num_columns; j++)
            result.data[i][j] = this->data[i][j] + other.data[i][j];
    }
    return result;
}

Matrix Matrix::operator-(const Matrix &other) const {
    if(this->num_rows != other.num_rows || this->num_columns != other.num_columns) {
        std::cerr << "ERROR: sum operation is not possible, matrices of different sizes!" << std::endl;
        exit(1);
    }
    Matrix result(num_rows, num_columns);
    for(int i = 0; i < num_rows; i++) {
        for(int j = 0; j < num_columns; j++)
            result.data[i][j] = this->data[i][j] - other.data[i][j];
    }
    return result;
}

Matrix Matrix::operator*(double scalar) const {
    Matrix result(num_rows, num_columns);
    for(int i = 0; i < num_rows; i++) {
        for(int j = 0; j < num_columns; j++)
            result.data[i][j] = data[i][j] * scalar;
    }
    return result;
}

Matrix Matrix::operator/(double scalar) const {
    Matrix result(num_rows, num_columns);
    for(int i = 0; i < num_rows; i++) {
        for(int j = 0; j < num_columns; j++)
            result.data[i][j] = data[i][j] / scalar;
    }
    return result;
}

bool Matrix::operator==(const Matrix& other) const {
    if(this->num_rows != other.num_rows || this->num_columns != other.num_columns)
        return false;
    for(int i = 0; i < num_rows; i++) {
        for(int j = 0; j < num_columns; j++) {
            if(this->data[i][j] != other.data[i][j])
                return false;
        }
    }
    return true;
}

bool Matrix::operator==(int zero_or_unit) const {
    if(zero_or_unit == 0)
        return *this == Matrix::ZeroMatrix(num_rows);
    if(zero_or_unit == 1)
        return *this == Matrix::UnitMatrix(num_rows);
    std::cerr << "ERROR: The matrix is not comparable to a scalar (exception 0 and 1 are zero and unit matrices, respectively)!" << std::endl;
    exit(1);
}

Matrix operator*(double scalar, const Matrix& other) {
    Matrix result(other.num_rows, other.num_columns);
    for(int i = 0; i < other.num_rows; i++) {
        for(int j = 0; j < other.num_columns; j++)
            result.data[i][j] = other.data[i][j] * scalar;
    }
    return result;
};

Matrix operator*(Matrix& first, Matrix& second) {
    if(first.num_columns != second.num_rows) {
        std::cerr << "ERROR: the multiplication operation is not possible, the number of columns of the first matrix and the number of rows of the second matrix do not match!" << std::endl;
        exit(1);
    }
    Matrix result(first.num_rows, second.num_columns);
    for(int i = 0; i < result.num_rows; i++) {
        for(int j = 0; j < result.num_columns; j++) {
            double sum = 0;
            for(int k = 0; k < first.num_columns; k++)
                sum += first.data[i][k] * second.data[k][j];
            result.data[i][j] = sum;
        }
    }
    return result;
}


double Matrix::get_det_thread() const {
    if(num_rows != num_columns) {
        std::cerr << "ERROR: it is impossible to calculate the determinant, the matrix is not square!" << std::endl;
        exit(1);
    }
    return p_get_det_thread();
}

double Matrix::p_get_det_thread() const {

}