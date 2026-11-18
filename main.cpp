#include <cstdio>
#include <vector>
#include <stdexcept>
#include <iostream>



template <typename T>
class Matrix {
private:
    size_t rows_;
    size_t cols_;
    std::vector<T> data_;

public:
    Matrix(size_t rows, size_t cols, T initial_value = T())
    : rows_(rows), cols_(cols), data_(rows * cols, initial_value) {}

    T& operator()(size_t row, size_t col) {
        return data_[row * cols_ + col];
    }
    const T& operator()(size_t row, size_t col) const {
        return data_[row * cols_ + col];
    }
    size_t rows() const { return rows_; }
    size_t cols() const { return cols_; }
    Matrix& operator+=(const Matrix& a);
    Matrix& operator-=(const Matrix& a);
    Matrix& operator=(const std::vector<std::vector<T>>);
    Matrix transp() const;
    Matrix input();

};

template <typename T>
std::ostream& operator<<(std::ostream& os, Matrix<T>& a) {
    for (size_t i = 0; i < a.rows(); ++i) {
        for (size_t j = 0; j < a.cols(); ++j) {
            os << a(i, j) << ' ';
        }
        os << '\n';
        }
    return os;
}

template <typename T>
Matrix<T>& Matrix<T>::operator+=(const Matrix<T>& a){
    if (a.rows() != this->rows() || a.cols() != this->cols()) {
        throw std::invalid_argument("Разные подряки");

    }
    else{
        for (size_t i = 0; i < a.rows(); i++){
            for (size_t j = 0; j < a.cols(); j++){
                (*this)(i, j) += a(i, j);
            }
        }
        return *this;
    }
}


template <typename T>
Matrix<T> operator+(const Matrix<T>&a, const Matrix<T>&b){

    if (a.rows() != b.rows() || a.cols() != b.cols()){
        throw std::invalid_argument("Разные подряки");

    }
    else{
        Matrix<T> c(a.rows(), a.cols());
        for (size_t i = 0; i < a.rows(); i++){
            for (size_t j = 0; j < a.cols(); j++){
                c(i, j) = a(i, j) + b(i, j);
            }
        }
        return c;
    }
}
template <typename T>
Matrix<T>& Matrix<T>::operator-=(const Matrix<T>& a){
    if (a.rows() != this->rows() || a.cols() != this->cols()) {
        throw std::invalid_argument("Разные подряки");
    }
    else{
        for (size_t i = 0; i < a.rows(); i++){
            for (size_t j = 0; j < a.cols(); j++){
                (*this)(i, j) -= a(i, j);
            }
        }
        return *this;
    }
}

template <typename T>
Matrix<T> operator-(const Matrix<T>&a, const Matrix<T>&b){
    if (a.rows() != b.rows() || a.cols() != b.cols()){
        throw std::invalid_argument("Разные подряки");
    }
    else{
        Matrix<T> c(a.rows(), a.cols());
        for (size_t i = 0; i < a.rows(); i++){
            for (size_t j = 0; j < a.cols(); j++){
                c(i, j) = a(i, j) - b(i, j);
            }
        }
        return c;
    }

}

template <typename T>
Matrix<T> operator*(const Matrix<T>&a, const Matrix<T>&b){
    if (a.rows() != b.cols()){
        throw std::invalid_argument("Не подходящие порядки");
    }
    else{
        Matrix<T> c(a.rows(), b.cols());
        for (size_t i = 0; i < a.rows(); ++i){
            for (size_t j = 0; j < b.rows(); ++j){
                T sm = T();
                for (size_t k = 0; k < a.rows(); ++k){
                    sm += a(i, k)*b(k, j);
                }
                c(i, j) = sm;
                sm = 0;

            }
        }
        return c;
    }
}

template <typename T>
bool operator==(const Matrix<T>&a, const Matrix<T>&b){
    if (a.rows() != b.rows() || a.cols() != b.cols()){
        return false;
    }
    else{
        for (size_t i = 0; i < a.rows(); ++i){
            for (size_t j = 0; j < a.cols(); ++j){
                if (a(i, j) != b(i, j)) {
                    return false;
                }
            }
        }
        return true;
    }
}




template <typename T>
Matrix<T> Matrix<T>::transp() const{
    Matrix b(this->cols(), this->rows());
    for (size_t i = 0; i < this->cols(); ++i){
        for (size_t j = 0; j < this->rows(); ++j){
            b(i, j) = (*this)(j, i);
        }
    }
    return b;
}

template <typename T>
Matrix<T> Matrix<T>::input(){
    for (size_t i = 0; i < this->rows(); ++i){
        for (size_t j = 0; j < this->cols(); ++j){
            std::cin >> (*this)(i, j);
        }
    }
    return (*this);
}

template <typename T>
class Matrix_kv : public Matrix<T> {

};



int main(){
    Matrix<int> a(2, 2);
    // Matrix<int> b(2, 2, 2);
    Matrix<int> b(2, 2);
    a.input();
    b.input();
    Matrix<int> c = a*b;
    std::cout << (a == b) << "\n";


    std::cout << "вывод\n" << a << b << c;

}
