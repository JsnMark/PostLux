// image.hpp

// TODO
// Opencv used as a placeholder until I decide to deal with loading and saving image file formats like .png and .jpg

#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string>
#include <exception>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>

#include "matrix.hpp"

using namespace cv;

enum ImageDataType{
    UCHAR,
    DOUBLE,
    INT
};


template <typename T>
struct Image{
    size_t rows;
    size_t cols;
    size_t channels;
    Matrix<T> data;
    ImageDataType datatype;

    Image();
    Image(size_t m, size_t n, size_t c, ImageDataType dt=UCHAR);
    Image(std::string filepath, ImageDataType dt=UCHAR);
    Image(const Image& other);
    Image& operator=(const Image& other);

    bool operator==(const Image<T>& other) const;
    bool operator!=(const Image<T>& other) const;

    T pixel(size_t i, size_t j, size_t c) const;
    T* ptr(size_t i, size_t j, size_t c);
    void saveImage(std::string filepath) const;
    void fill(T value, int channel=-1);
    void fill(std::vector<T> vec, int channel=-1);
};

/* 
 * Default constructor with no parameters
 */
template <typename T>
Image<T>::Image()
    : rows{0}
    , cols{0}
    , channels{0}
    , data{}
    , datatype{UCHAR}
{}


/*
 * Constructor. Constructs image from its dimensions
 *
 * @param m number of rows
 * @param n number of columns
 * @param c number of channels
 */
template <typename T>
Image<T>::Image(size_t m, size_t n, size_t c, ImageDataType dt)
    : rows{m}
    , cols{n}
    , channels{c}
    , data{Matrix<T> {m, n, c}}
    , datatype{dt}
{}

/*
 * Consructs an image given its filename.
 *
 * @param filepath path to the image
 */
template <typename T>
Image<T>::Image(std::string filepath, ImageDataType dt){
    Mat img = imread(filepath);
    if(img.empty())
        throw std::runtime_error("Empty image. Failed to load image.");
    rows = img.rows;
    cols = img.cols;
    channels = img.channels();
    data = Matrix<T> {rows, cols, channels};
    datatype = dt;

    // Must read as unsigned char from img
    unsigned char* imgPtr = img.ptr<unsigned char>(0);
    T* dataPtr = data.ptr(0,0,0);
    int len = rows * cols * channels;
    for(int i = 0; i < len; ++i){
        *dataPtr = static_cast<T>(*imgPtr);
        ++dataPtr;
        ++imgPtr;
    }
}


/*
 * Copy Constructor
 */
template<typename T>
Image<T>::Image(const Image<T>& other):
    rows{other.rows},
    cols{other.cols},
    channels{other.channels},
    data{other.data},
    datatype{other.datatype}
{}

/* Copy assignment
 */
template <typename T>
Image<T>& Image<T>::operator=(const Image<T>& other)
{
    rows = other.rows;
    cols = other.cols;
    channels = other.channels;
    data = other.data;
    datatype = other.datatype;
    return *this;
}

/**
 * Returns the value of the pixel at position (i,j,k)
 *
 * @param i row index
 * @param j col index
 * @param k channel index
 * @return value at position (i,j,k)
 */
template <typename T>
T Image<T>::pixel(size_t i, size_t j, size_t k)const{
    return data.pixel(i,j,k);
}

/*
 * Returns the pointer to the value at position (i,j,k)
 *
 * @param i row index
 * @param j col index
 * @param k channel index
 * @return pointer to the value
 */
template <typename T>
T* Image<T>::ptr(size_t i, size_t j, size_t k){
    return data.ptr(i,j,k);
}


/*
 * Saves the image
 * For OpenCV, Assume all pixels are 8 bits with 3 channels for rgb
 *
 * @param filepath filepath of the destination
 */
template <typename T>
void Image<T>::saveImage(std::string filepath)const{

    Mat mat(static_cast<int>(rows), static_cast<int>(cols), CV_8UC3);
    for(size_t i = 0; i < rows; ++i){
        unsigned char* Mi = mat.ptr<unsigned char>(i);
        for(size_t j = 0; j < cols; ++j){
            for(size_t k = 0; k < channels; ++k){
                Mi[j * channels + k] = pixel(i,j,k); 
            }
          
        }
    }

    imwrite(filepath, mat);
}


/*
 * Equality operator. True when contents are same valued
 *
 * @param other other image to be compared
 * @return true if equal, false if not equal
 */
template <typename T>
bool Image<T>::operator==(const Image<T>& other)const{
    if(other.rows != rows 
       || other.cols != cols 
       || other.channels != channels){
        return false; 
    } 

    for(size_t i = 0; i < rows; ++i){
        for(size_t j = 0; j < cols; ++j){
            for(size_t k = 0; k < channels; ++k){
                if(other.pixel(i,j,k) != this->pixel(i,j,k)){
                    return false;
                }
            }
        }
    }
    return true;
}

/*
 * Inequality operator
 *
 * @param other
 * @return true if not equal, false if equal
 */
template <typename T>
bool Image<T>::operator!=(const Image<T>& other)const{
    return !(*this == other);
}

/*
 * Fills image with a value at specified channel with a value
 * 
 * @param value 
 * @param channel if -1, then fill all channels
 */
template <typename T>
void Image<T>::fill(T value, int channel){
    data.fill(value, channel);    
}

/*
 * Fills image at specified channel with a vector
 * 
 * @param value
 * @param channel if -1 fill all channels
 */
template<typename T>
void Image<T>::fill(std::vector<T> vec, int channel){
    data.fill(vec, channel);
}




#endif
