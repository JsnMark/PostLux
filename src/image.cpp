// image.cpp

#include "image.hpp"

Image::Image(size_t m, size_t n, size_t c)
    : rows{m}
    , cols{n}
    , channels{c}
    , data{Matrix<int> {m, n, c}}
{
}

int Image::getPixel(size_t m, size_t n, size_t c){
    return *(data.ptr(m, n, c));
}

