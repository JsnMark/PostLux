// matrix.hpp

#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <initializer_list>
#include <exception>
#include <vector>
#include <iostream>


// Declaration
template <typename T>
class Matrix{
    private:
        size_t rows; 
        size_t cols;
        T* buf;

    public:
        Matrix (size_t m, size_t n);
        Matrix& operator=(const Matrix& other);
        ~Matrix();
        T* at(size_t i, size_t j);
};


// Definitions

/**
 * Constructor. Constructs a matrix with shape mxn
 *
 * ex: Matrix(m(3, 4)); -> constructs a 3x4  matrix that is continuous in memory
 * @param m -> number of rows
 * @param n -> number of columns
 */
template <typename T>
Matrix<T>::Matrix(size_t m, size_t n){
    this->rows = m;
    this->cols = n;
    this->buf = new T[m*n];
}

/** Copy Assignment Constructor. Constructs a Matrix given another Matrix
 *  
 *  ex: Matrix m1 = m2;
 *  @param other -> reference to a Matrix
 */
template <typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix& other){
    this->rows = other.rows;
    this->cols = other.cols;
    size_t mn = this->rows * this->cols;
    this->buf = new T[mn];
    for(size_t i = 0; i < mn; ++i){
        buf[i] = other->buf[i];
    } 
    return *this;
}



/**
 * Destructs Matrix at the end of its lifecycle. Deallocates heap mem
 *  - Automatically called
 */
template <typename T>
Matrix<T>::~Matrix(){
    delete[] buf;
}


/**
 * Returns a pointer to the element at a given index
 * @param i -> row index
 * @param j -> col index
 */
template <typename T>
T * Matrix<T>::at(size_t i, size_t j){
    return buf + i*rows+j;
}



#endif
