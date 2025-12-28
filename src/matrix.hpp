// matrix.hpp

#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <cstddef>
#include <vector>

// Declaration
template <typename T>
class Matrix{
    private:
        size_t rows; 
        size_t cols;
        size_t channels;
        T* buf;

    public:
        Matrix ();
        Matrix (size_t m, size_t n, size_t c);
        Matrix& operator=(const Matrix& other);
        Matrix (const Matrix& other);
        ~Matrix();

        T pixel(size_t i, size_t j, size_t c) const;
        T* ptr(size_t i, size_t j, size_t c);
        size_t getRow() const;
        size_t getCol() const;
        size_t getCh() const;
        void fill(T value, int c=-1);
        void fill(std::vector<T> vec, int c=-1);
};


// Definitions

/**
 * Constructor. Constructs an empty matrix
 */
template <typename T>
Matrix<T>::Matrix(): 
    rows{0},
    cols{0},
    channels{0},
    buf{nullptr}
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
 * @param other reference to a Matrix
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
 *  @param other reference to a Matrix
 *  @return pointer to this object
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
 * @param i row index
 * @param j col index
 * @param c channel index
 * @return pointer to the element
 */
template <typename T>
T * Matrix<T>::ptr(size_t i, size_t j, size_t c){
    return buf + i * cols * channels + j * channels + c;
}
/*
 * Returns the pixel at the specified location
 * @param i row index
 * @param j col index
 * @param c channel index
 * @return value at the pixel
 */
template <typename T> T Matrix<T>::pixel(size_t i, size_t j, size_t c) const{
    return *(buf + i * cols * channels + j * channels + c);
}

/** 
 * Returns Number of Rows
 */
template <typename T>
size_t Matrix<T>::getRow()const{return rows;}

/**
 * Returns Number of Columns
 */
template <typename T>
size_t Matrix<T>::getCol()const{return cols;}

/**
 * Returns Number of Channels
 */
template <typename T>
size_t Matrix<T>::getCh()const{return channels;}

/* 
 * Fills matrix with a value at channel c. If c == -1, fills everything
 * 
 * @param value
 * @param c
 */
template <typename T>
void Matrix<T>::fill(T value, int c){
    if(c >= (int) channels){// assume channels will not overflow
        throw(std::runtime_error("C is too large"));
    }
    if(c < -1){
        throw(std::runtime_error("C is too small"));
    }

    for(size_t i = 0; i < rows; ++i){
        for(size_t j = 0; j < cols; ++j){
            for(size_t k = 0; k < channels; ++k){
                if(c == -1 || k == c){
                    *(this->ptr(i,j,k)) = value;
                }
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
    if(c > (int) channels)
        throw(std::runtime_error("c is too large"));
    if(c < -1)
        throw(std::runtime_error("c is too small"));
    if(c == -1 && vec.size() != (rows * cols * channels))
        throw(std::runtime_error("size of vector incorrect"));
    if(c != -1 && vec.size() != (rows * cols))
        throw(std::runtime_error("size of vector incorrect"));

    size_t vec_idx = 0;
    for(int i = 0; i < rows; ++i){
        for(int j = 0; j < cols; ++j){
            for(int k = 0; k < channels; ++k){
                if(c == -1 || k == c){
                    *(this->ptr(i,j,k)) = vec.at(vec_idx);
                    ++vec_idx;
                } 
            }
        }
    }
}




#endif
