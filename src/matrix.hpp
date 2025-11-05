// matrix.hpp

#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <initializer_list>
#include <exception>
#include <vector>
#include <iostream>

enum ImageColor {
    RGB,
    GRAYSCALE
};


// Declaration
template <typename T>
class Matrix{
    private:
        T* buf; // The actual matrix of size mxn. Represented as an array with lenght mxn
        int rows; // m
        int cols; // n
        ImageColor imageColor; // image type ie. RGB/Grayscale
    public:
        Matrix (size_t m, size_t n, ImageColor imageColor = RGB); 
        ~Matrix();
        // T* operator [] (const int i);
        // T & operator [] (const int i, const int j);
        // T operator [] (const int i, const int j) const;
        // T max();
};

// Definitions



/**
* Constructs a matrix with shape mxn
 *
 * ex: Matrix(m(3, 4)); -> constructs a 3x4x3 matrix that is continuous in memory
 * @param m -> number of rows
 * @param n -> number of columns
 * @param imageColor = RGB -> color type of image
 */
template <typename T>
Matrix<T>::Matrix(size_t m, size_t n, ImageColor imageColor){
    this->rows = m;
    this->cols = n;
    this->imageColor = imageColor;
    this->buf = new T[m*n*imageColor];
}


/**
 * Destructs Matrix at the end of its lifecycle
 *  - Automatically called
 *  Deallocate buf
 */
template <typename T>
Matrix<T>::~Matrix(){
    delete[] buf;
}

// /*
//  * Overloads [] and returns pointer to the first element in row i.
//  * WARNING - If working with a row, must manually check the bounds to be within j
//  * @param i 0-indexed row to be selected
//  */
// template <typename T>
// T* &operator [] (const i){ 
//     if(i < 0 || i > cols){
//         throw::std_out_of_range("Out of range");
//     }
//     return buf[i * cols];
// }
// 
// /*
//  * Overloads [] and returns reference to value at [i,j]. Used in LHS
//  * @param i row of element
//  * @param j col of element
//  */
// T operator [] (const int i) const{
//     
// } 



/**
 * Returns max value of the matrix
 */
// template <typename T>
// T Matrix<T>::max(){
//     T maximum;
// }


#endif
