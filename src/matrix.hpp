// matrix.hpp

#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <cstddef>

// Declaration
template <typename T>
class Matrix{
    private:
        size_t rows; 
        size_t cols;
        size_t channels;
        T* buf;

    public:
        Matrix (size_t m, size_t n, size_t c);
        Matrix& operator=(const Matrix& other);
        Matrix (const Matrix& other);
        ~Matrix();

        T* ptr(size_t i, size_t j, size_t c);
        size_t getRow();
        size_t getCol();
        size_t getCh();
};


// Definitions

/**
 * Constructor. Constructs a matrix with shape mxnxc. Order in mem is (0,0,0), ..., (0,0,c-1), (0,1,0), ..., (0, n-1, c-1), (1, 0, 0), ...
 *
 * ex: Matrix(m(3, 4, 1)); -> constructs a 3x4x1  matrix that is continuous in memory
 * @param m -> number of rows
 * @param n -> number of columns
 * @param c -> number of channels
 */
template <typename T>
Matrix<T>::Matrix(size_t m, size_t n, size_t c)
    : rows{m}
    , cols{n}
    , channels{c}
{
    this->buf = new T[m*n*c];
}
/** Copy Constructor. Constructs a matrix given another matrix
 *
 * ex: Matrix<int> m1 = m2;
 * @param other -> reference to a Matrix
 */
template <typename T>
Matrix<T>::Matrix(const Matrix<T>& other)
    : rows{other.rows}
    , cols{other.cols}
    , channels{other.channels}
{
    this->buf = new T[rows*cols*channels];
    size_t mnc = rows*cols*channels;
    for(size_t i = 0; i < mnc; ++i){
       buf[i] = other.buf[i];
    }
}

/** Copy Assignment. Assigns the matrix data given another Matrix
 *  
 *  ex: m1 = m2;
 *  @param other -> reference to a Matrix
 */
template <typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix& other){
    if(buf != nullptr){
        delete[] buf;
        buf = nullptr;
    }
    this->rows = other.rows;
    this->cols = other.cols;
    this->channels = other.channels;
    size_t mnc = this->rows * this->cols * this->channels;
    this->buf = new T[mnc];
    for(size_t i = 0; i < mnc; ++i){
        buf[i] = other.buf[i];
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
 * Returns a pointer to the element at a given index. No bounds checking.
 *
 * @param i -> row index
 * @param j -> col index
 * @param c -> channel index
 */
template <typename T>
T * Matrix<T>::ptr(size_t i, size_t j, size_t c){
    return buf + i * cols * channels + j * channels + c;
}

/** 
 * Returns Number of Rows
 */
template <typename T>
size_t Matrix<T>::getRow(){return rows;}

/**
 * Returns Number of Columns
 */
template <typename T>
size_t Matrix<T>::getCol(){return cols;}

/**
 * Returns Number of Channels
 */
template <typename T>
size_t Matrix<T>::getCh(){return channels;}


#endif
