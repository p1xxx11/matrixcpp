#include <cstdio>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <cmath>
#include <type_traits>

template <typename T>
class Matrix {
private:
    size_t rows_;
    size_t cols_;
    std::vector<T> data_;

public:
    Matrix(size_t rows, size_t cols, T initial_value = T())
    : rows_(rows), cols_(cols), data_(rows * cols, initial_value) {}

    Matrix(const std::vector<std::vector<T>> &a);

    T& operator()(size_t row, size_t col) {
        if (row >= rows_ || col >= cols_) {
        throw std::out_of_range("Индекс матрицы вне границ");
        }
        return data_[row * cols_ + col];
    }
    const T& operator()(size_t row, size_t col) const {
        if (row >= rows_ || col >= cols_) {
        throw std::out_of_range("Индекс матрицы вне границ");
        }
        return data_[row * cols_ + col];
    }
    size_t rows() const { return rows_; }
    size_t cols() const { return cols_; }
    Matrix& operator+=(const Matrix& a);
    Matrix& operator-=(const Matrix& a);
    Matrix& operator=(const std::vector<std::vector<T>> a);
    Matrix transp() const;
    Matrix input();
    Matrix expans(const Matrix& a);
    Matrix& el_1(size_t i, size_t j);
    Matrix& el_2(size_t i, float l);
    Matrix& el_3(size_t i, size_t j, float l);
    Matrix& to_stup();
    Matrix& to_glstup();
    int rank();
};


template <typename T>
Matrix<T>::Matrix(const std::vector<std::vector<T>> &a)
    : rows_(a.size()),
    cols_(a.empty() ? 0 : a[0].size()),
    data_(rows_ * cols_)
    {
        for (size_t i = 0; i < rows_; ++i){
            if (a[i].size() != cols_){
                throw std::invalid_argument("Разная длина строк");
            }
            
            for (size_t j = 0; j < cols_; ++j){
                (*this)(i, j) = a[i][j];
            }
        }
    }


template <typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& a) {
    os << "-----------------\n";
    for (size_t i = 0; i < a.rows(); ++i) {

        for (size_t j = 0; j < a.cols(); ++j) {
            os << a(i, j) << ' ';
        }
        os << '\n';
        }
    os << "-----------------\n";
    return os;
}

template <typename T>
Matrix<T>& Matrix<T>::operator+=(const Matrix<T>& a){
    if (a.rows() != this->rows() || a.cols() != this->cols()) {
        throw std::invalid_argument("Разные подряки");
    }
    for (size_t i = 0; i < a.rows(); i++){
        for (size_t j = 0; j < a.cols(); j++){
            (*this)(i, j) += a(i, j);
        }
    }
    return *this;

}


template <typename T>
Matrix<T> operator+(const Matrix<T>&a, const Matrix<T>&b){

    if (a.rows() != b.rows() || a.cols() != b.cols()){
        throw std::invalid_argument("Разные подряки");
    }
    Matrix<T> c(a.rows(), a.cols());
    for (size_t i = 0; i < a.rows(); i++){
        for (size_t j = 0; j < a.cols(); j++){
            c(i, j) = a(i, j) + b(i, j);
        }
    }
    return c;
}

template <typename T>
Matrix<T>& Matrix<T>::operator-=(const Matrix<T>& a){
    if (a.rows() != this->rows() || a.cols() != this->cols()) {
        throw std::invalid_argument("Разные подряки");
    }
    for (size_t i = 0; i < a.rows(); i++){
        for (size_t j = 0; j < a.cols(); j++){
            (*this)(i, j) -= a(i, j);
        }
    }
    return *this;
}

template <typename T>
Matrix<T> operator-(const Matrix<T>&a, const Matrix<T>&b){
    if (a.rows() != b.rows() || a.cols() != b.cols()){
        throw std::invalid_argument("Разные подряки");
    }
    Matrix<T> c(a.rows(), a.cols());
    for (size_t i = 0; i < a.rows(); i++){
        for (size_t j = 0; j < a.cols(); j++){
            c(i, j) = a(i, j) - b(i, j);
        }
    }
    return c;

}

template <typename T>
Matrix<T> operator*(const Matrix<T>&a, const Matrix<T>&b){
    if (a.cols() != b.rows()){
        throw std::invalid_argument("Не подходящие порядки");
    }
    Matrix<T> c(a.rows(), b.cols());
    for (size_t i = 0; i < a.rows(); ++i){
        for (size_t j = 0; j < b.cols(); ++j){
            T sm = T();
            for (size_t k = 0; k < a.cols(); ++k){
                sm += a(i, k)*b(k, j);
            }
            c(i, j) = sm;
            sm = 0;
        }
    }
    return c;
}

template <typename T>
bool operator==(const Matrix<T>&a, const Matrix<T>&b){
    if (a.rows() != b.rows() || a.cols() != b.cols()){
        return false;
    }
    for (size_t i = 0; i < a.rows(); ++i){
        for (size_t j = 0; j < a.cols(); ++j){
            if (a(i, j) != b(i, j)) {
                return false;
            }
        }
    }
    return true;
}

template <typename T>
Matrix<T>& Matrix<T>::operator=(const std::vector<std::vector<T>> a){
    if (a.size() != this->rows() || a[0].size() != this->cols()) {
        throw std::invalid_argument("Разные подряки");
    }

    for (size_t i = 0; i < a.size(); ++i){
        for (size_t j = 0; j < a[0].size(); ++j){
            (*this)(i, j) = a[i][j];
        }
    }
    return *this;
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
Matrix<T> Matrix<T>::expans(const Matrix<T>& a){
    if (this->rows() != a.rows()) {
        throw std::invalid_argument("Кол-во строк должно совпадать");
    }
    Matrix<T> b(a.rows(), this->cols() + a.cols());
    for (size_t i = 0; i < a.rows(); ++i){
        for (size_t j = 0; j < this->cols()+a.cols(); j++){
            if (j < this->cols())b(i, j) = (*this)(i, j);
            else b(i, j) = a(i, j-this->cols());
        }
    }
    return b;
    
}

Matrix<int> cr_E(size_t rows){
    Matrix<int> a(rows, rows, 0);
    for (size_t i = 0; i < rows; ++i){
        a(i, i) = 1;
    }
    return a;
}

template <typename T>
Matrix<T> cr_el_1(size_t rows, size_t ik, size_t jk){

    // ik--; jk--;
    if (ik > rows || jk > rows){
        std::cout << ik << rows << jk;
        throw std::invalid_argument("Индекс должен быть меньше кол-ва строк!!!!");
    }
    Matrix<T> a(rows, rows, 0);
    for (size_t i = 0; i < rows; ++i){
        if (i != ik && i != jk) a(i, i) = 1;
    }
    a(ik, jk) = 1;
    a(jk, ik) = 1;
    return a;
    
    
}

template <typename T>
Matrix<T> cr_el_2(size_t rows, size_t ik, float l){
    if (!l){
    throw std::invalid_argument("l != 0");
    }
    if (ik>= rows){
    throw std::invalid_argument("индекс должен быть меньше кол-ва строк");
    }
    Matrix<T> a(rows, rows, 0);
    for (size_t i = 0; i < rows; ++i){
    if (i != ik) a(i, i) = 1;
    else a(i, i) = l;
    }
    return a;

}

template <typename T>
Matrix<T> cr_el_3(size_t rows, size_t ik, size_t jk, float l){
    if (!l){
        throw std::invalid_argument("l != 0");
    }
    if (ik>=rows){
        throw std::invalid_argument("Индекс должен быть меньше кол-ва строк");
    }
    Matrix<T> a(rows, rows, 0);
    for (size_t i = 0; i < rows; ++i){
        if (i != ik) a(i, i) = 1;
        else a(i, i) = 1; a(ik, jk) = l;
    }
    return a;
}

template <typename T>
Matrix<T>& Matrix<T>::el_1(size_t i, size_t j){
    (*this) = cr_el_1<T>(this->rows(), i, j)*(*this);
    return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::el_2(size_t i, float l){
    (*this) = cr_el_2<T>(this->rows(), i, l)*(*this);
    return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::el_3(size_t i, size_t j, float l){
    (*this) = cr_el_3<T>(this->rows(), i, j, l)*(*this);
    return *this;
}


template <typename T>
Matrix<T>& Matrix<T>::to_stup() {
    static_assert(std::is_floating_point<T>::value,
                  "Use Matrix<float> or Matrix<double>");
    const T eps = T(1e-6);
    size_t pivot_row = 0;

    for (size_t col = 0; col < cols_ && pivot_row < rows_; ++col) {
        size_t best = pivot_row;
        for (size_t row = pivot_row + 1; row < rows_; ++row) {
            if (std::abs((*this)(row, col)) > std::abs((*this)(best, col))) {
                best = row;
            }
        }

        // Нулевой столбец пропускаем, опорная строка остаётся прежней.
        if (std::abs((*this)(best, col)) <= eps) {
            continue;
        }
        if (best != pivot_row) {
            this->el_1(pivot_row, best);
        }

        for (size_t row = pivot_row + 1; row < rows_; ++row) {
            if (std::abs((*this)(row, col)) > eps) {
                const float factor = static_cast<float>(
                    -(*this)(row, col) / (*this)(pivot_row, col));
                this->el_3(row, pivot_row, factor);
            }
        }
        ++pivot_row;
    }
    return *this;
}


template <typename T>
Matrix<T>& Matrix<T>::to_glstup() {
    this->to_stup();
    const T eps = T(1e-6);

    // Обход снизу вверх безопасен и для пустой матрицы.
    for (size_t row = rows_; row > 0;) {
        --row;
        size_t pivot_col = 0;
        while (pivot_col < cols_ &&
               std::abs((*this)(row, pivot_col)) <= eps) {
            ++pivot_col;
        }
        if (pivot_col == cols_) {
            continue;
        }

        this->el_2(row, static_cast<float>(T(1) / (*this)(row, pivot_col)));

        // Работаем только со столбцом первого ненулевого элемента строки.
        for (size_t upper = 0; upper < row; ++upper) {
            if (std::abs((*this)(upper, pivot_col)) > eps) {
                const float factor = static_cast<float>(
                    -(*this)(upper, pivot_col) / (*this)(row, pivot_col));
                this->el_3(upper, row, factor);
            }
        }
    }
    return *this;
}


template <typename T>
int Matrix<T>::rank(){
    std::cout << *this;
    int rk = 0;
    for (size_t i = 0; i < this->rows(); ++i){
        bool fl = true;
        for (size_t j = 0; j < this->cols(); ++j){
            if ((*this)(i, j) != 0) fl = false;
        }
        if (!fl) rk++;
    }
    return rk;
}

template <typename T>
class sq_Matrix : private Matrix<T> {


    public:
    explicit sq_Matrix(size_t rows, T initial_value = T())
    : Matrix<T>(size, size, initial_value) {}

    explicit sq_Matrix(const std::vector<std::vector<T>> &a)
    : Matrix<T>()
    // TODO: норм наследовать методы
    using Matrix<T>::operator();
    using Matrix<T>::rows;
    using Matrix<T>::operator+=;
    using Matrix<T>::operator-=;
    using Matrix<T>::transp;
    using Matrix<T>::input;
    using Matrix<T>::expans;
    using Matrix<T>::el1;
    using Matrix<T>::el2;
    using Matrix<T>::el3;
    using Matrix<T>::to_stup;
    using Matrix<T>::to_glstup;
    using Matrix<T>::rank;

    sq_Matrix inverse();

};

template <typename T>
sq_Matrix<T> sq_Matrix<T>::inverse(){
    (this->expans(cr_E(this->rows())))->to_glstup();
    std::cout << *this;
}



int main(){
    Matrix <float> a({
        {7, 5, -6, 11},
        {0, 67, 99, 9},
        {4, 0, 1, 0},
        {1, 0, 0.5, 8},
        {0, 0, 0, 25}
    });
    // std::cout << a << a.to_stup() << a.to_glstup();
    std::cout << a.rank() << "\n";
}
