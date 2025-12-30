// matrix.hpp

#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <cstddef>
#include <vector>
#include <string>
#include <format>
#include <algorithm>

// Declaration
template <typename T>
struct Matrix{
    size_t rows; 
    size_t cols;
    size_t channels;
    std::vector<T> buf;

    Matrix ();
    Matrix (size_t m, size_t n, size_t c);
    Matrix& operator=(const Matrix& other);
    Matrix (const Matrix& other);
    ~Matrix();

    T pixel(size_t i, size_t j, size_t c) const;
    T* ptr(size_t i, size_t j, size_t c);
    void fill(T value, int c=-1);
    void fill(std::vector<T> vec, int c=-1);
};



/**
 * Constructor. Constructs an empty matrix
 */
template <typename T>
Matrix<T>::Matrix():
    rows{0},
    cols{0},
    channels{0},
    buf{}
{}


/**
 * Constructor. Constructs a matrix with shape mxnxc. Order in mem is (0,0,0), ..., (0,0,c-1), (0,1,0), ..., (0, n-1, c-1), (1, 0, 0), ...
 *
 * ex: Matrix(m(3, 4, 1)); -> constructs a 3x4x1  matrix that is continuous in memory
 * @param m number of rows
 * @param n number of columns
 * @param c number of channels
 */
template <typename T>
Matrix<T>::Matrix(size_t m, size_t n, size_t c):
    rows{m},
    cols{n},
    channels{c},
    buf{}
{
    buf.reserve(m*n*c);
    buf.resize(m*n*c);
}
/** Copy Constructor. Constructs a matrix given another matrix
 *
 * ex: Matrix<int> m1 = m2;
 * @param other reference to a Matrix
 */
template <typename T>
Matrix<T>::Matrix(const Matrix<T>& other):
    rows{other.rows},
    cols{other.cols},
    channels{other.channels},
    buf{other.buf}
{}

/** Copy Assignment. Assigns the matrix data given another Matrix
 *  
 *  ex: m1 = m2;
 *  @param other reference to a Matrix
 *  @return pointer to this object
 */
template <typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix& other){
    rows = other.rows;
    cols = other.cols;
    channels = other.channels;
    buf = other.buf; 
    return *this;
}



/**
 * Destructs Matrix at the end of its lifecycle. Deallocates heap mem
 *  - Automatically called
 */
template <typename T>
Matrix<T>::~Matrix() = default;


/**
 * Returns a pointer to the element at a given index.
 *
 * @param i row index
 * @param j col index
 * @param k channel index
 * @return pointer to the element
 */
template <typename T>
T * Matrix<T>::ptr(size_t i, size_t j, size_t k){
    return buf.data() + i * cols * channels + j * channels + k;
}

/*
 * Returns the pixel at the specified location
 * @param i row index
 * @param j col index
 * @param k channel index
 * @return value at the pixel
 */
template <typename T> T Matrix<T>::pixel(size_t i, size_t j, size_t k) const{
    return *(buf.data() + i * cols * channels + j * channels + k);
}




/* 
 * Fills matrix with a value at channel c. If c == -1, fills everything
 * Assume less than INT_MAX channels
 * 
 * @param value
 * @param c
 */
template <typename T>
void Matrix<T>::fill(T value, int c){
    if(c >= static_cast<int>(channels)){// assume channels will not overflow
        throw(std::runtime_error("Channel index too big"));
    }
    if(c < -1){
        throw(std::runtime_error("Channel index too small"));
    }

    if(c == -1){
        std::fill(buf.begin(), buf.end(), value);
        return;
    }

    // no std algorithm for fill based on condition
    T* ptr = this->ptr(0,0,0);
    size_t c_t = static_cast<size_t>(c);
    for(size_t i = 0; i < rows; ++i){
        for(size_t j = 0; j < cols; ++j){
            for(size_t k = 0; k < channels; ++k){
                if(k == c_t){
                    *ptr = value;
                }
                ++ptr;
            }
        }
    }
}

/*
 * Fills matrix with a vector at specified channel
 *
 * @param vec vector of elements
 * @param c channel index. if -1, then fill entire matrix
 */
template <typename T>
void Matrix<T>::fill(std::vector<T> vec, int c){
    if(c > static_cast<int>(channels)){
        throw(std::runtime_error("Channel index too big"));
    }
    if(c < -1){
        throw(std::runtime_error("Channel index too small"));
    }
    if(c == -1 && vec.size() != (rows * cols * channels)){
        throw(std::runtime_error("Invalid vector size"));
    }
    if(c != -1 && vec.size() != (rows * cols)){
        throw(std::runtime_error("Invalid vector size"));
    }

    if(c == -1){
        buf = vec;
        return;
    }

    T* vec_ptr = vec.data();
    T* p = this->ptr(0,0,0);
    size_t c_t = static_cast<size_t>(c);
    for(size_t i = 0; i < rows; ++i){
        for(size_t j = 0; j < cols; ++j){
            for(size_t k = 0; k < channels; ++k){
                if(k == c_t){
                    *p = *vec_ptr;
                    ++vec_ptr;
                } 
                ++p;
            }
        }
    }
}




#endif
