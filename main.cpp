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
Matrix<T>& Matrix<T>::to_stup(){


    // НАХОДИМ НЕНУЛЕВОЙ СТОЛБЕЦ
    size_t piv_i = 0; size_t piv_j = 0;
    bool fl = true;
    for (size_t j = 0; j < this->cols(); ++j){
        for (size_t i = 0; i < this->rows(); ++i){
            if ((*this)(i, j) != 0){
                if (fl){
                    fl = false;
                    piv_i = i;
                    piv_j = j;
                }
            }
        }}
        if (piv_i != 0) {
            this->el_1(0, piv_i);
        }

        // ПРЯМОЙ ХОД
        size_t start = 1;
        while (piv_j < this->cols()){;
        for (size_t i = start; i < this->rows(); ++i){
            if ((*this)(i, piv_j) != 0){
            float k = static_cast<float>((*this)(i, piv_j))/-(*this)(start-1, piv_j);
            // std::cout << k;
            // return *this;
            this->el_3(i, start-1, k);
            // std::cout << *this << k << " " << start << " " << piv_j << "\n";
            // std::cout << *this;
            // return *this;
            }
        }
        start++;
        piv_j++;
    }

    return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::to_glstup(){
    this->to_stup();

    // ОБРАТНЫЙ ХОД
    
    // делаем все единицами
    for (size_t i = 0; i < this->rows(); ++i){
        bool fl = true;
        for (size_t j = 0; j < this->cols(); ++j){
            if (((*this)(i, j) != 0) && fl){
                // std::cout << (*this)(i, j) << "\n";
                this->el_2(i, 1/static_cast<float>((*this)(i, j)));
                fl = false;
                // break;
            }
        }
    }
    // std::cout << *this;

    // ОБРАТНЫЙ ХОД

    for (size_t i = this->rows()-1; i > 0; --i){
        for (size_t j = 0; j < this->cols(); ++j){
            if ((*this)(i, j) != 0){
                for (int k = i-1; k >= 0; --k){
                    // std::cout << k << "\n";
                    if ((*this)(k, j) != 0){
                        float kf = -(*this)(k, j);
                        this->el_3(k, i, kf);
                    }   
                }
            }
        }
        // std::cout << *this;
    }

    // доп проверка на нулевые строки
    std::vector<size_t> nen;
    std::vector<size_t> nul;

    for (size_t i = 0; i < this->rows(); ++i){
        bool fl = true;
        for (size_t j = 0; j < this->cols(); ++j){
            if ((*this)(i, j) != 0){
                fl = false;
            }
        }
        if (fl) nul.push_back(i);
        else nen.push_back(i);
    }

    return *this;
}


template <typename T>
class Matrix_kv : public Matrix<T> {

};



int main(){
    Matrix <float> a({
        {0, 5, 3, 2, 9},
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {0, 1, 4, 2, 3}
    });
    std::cout << a << a.to_stup() << a.to_glstup();
}
