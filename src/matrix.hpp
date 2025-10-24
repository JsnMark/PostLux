#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <initializer_list>
#include <exception>

template <typename T>
class Matrix{
    private:
        T** buf; // The actual matrix of size mxn. An array of an array of T
        int rows; // m
        int cols; // n
    public:
        /**
         * Constructs a matrix with shape mxn
         *
         * ex: Matrix(m(3, 4)); -> constructs a 3x4 matrix
         * @param m number of rows
         * @param n number of columns
         */
        Matrix(size_t m, size_t n){
            this->rows = m;
            this->cols = n;
            this->buf = new T*[m];
            T** tmp = this->buf;
            for(int i = 0; i < m; ++i){
                *tmp = new T[n];
                tmp++;
            }
            tmp = nullptr;
        }

        /**
         * Constructs a matrix from a list of list of elements
         *
         * ex Matrix m1{{1,2,3}, {4,5,6}} -> creates a 2x3 matrix with elements [[1,2,3], [4,5,6]]
         * @param L list of list of values to initialize our vector
         *      - L.size() gives the list's size
         *      - Can iterate through with iterator
         */
        Matrix(std::initializer_list<std::initializer_list<T>> L){
            // Initializes row and cols
            this->rows = L.size();
            if(L.size() > 0){
                // All rows must have same amount of elements;
                this->cols = L[0].size();
                for (int i = 1; i < this->rows; ++i){
                    if(L[i].size() != this->cols){
                        throw std::length_error("Column lengths are not all the same");
                    }
                }
            } else {
                this->cols = 0;
            }
            
            // Empty Matrix 0x0
            if(this->rows == 0){
                return;
            }

            // Create the actual matrix
            int m = this->rows;
            int n = this->cols;
            this->buf = new T*[m];
            T* tmp_row = this->buf[0];
            for(int i = 0; i < m; ++i){
                tmp_row = new T[n]; 
                ++tmp_row;
            }
            tmp_row = nullptr;
            // Fill it up
            T** tmp_outer = buf;
            for(std::initializer_list<T> r: L){
                T* tmp_inner = *tmp_outer;
                for(T item: r){
                    *tmp_inner = item;
                    ++tmp_inner;
                }
                ++tmp_outer;
            }
            tmp_outer = nullptr;
        }
        
        /**
         * Destructs Matrix at the end of its lifecycle
         *  - Automatically called
         *  Deallocate buf
         */
        ~Matrix(){
            for(int i = 0; i< this->rows; ++i){
                delete[] (this->buf)[i];
            }
            delete[] this->buf;
        }


};



#endif
