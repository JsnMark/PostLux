// image.hpp

#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "matrix.hpp"

class Image{
    private:
        size_t rows;
        size_t cols;
        size_t channels;
        Matrix<int> data;

    public:
         Image(size_t m, size_t n, size_t c);
//         ~Image();
//         Image(const Image& other);
//         Image& operator=(const Image& other);
};

#endif
