// image.hpp

#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string>
#include <exception>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include "matrix.hpp"

using namespace cv;

template <typename T>
class Image{
    private:
        size_t rows;
        size_t cols;
        size_t channels;
        Matrix<T> data;

    public:
         Image(size_t m, size_t n, size_t c);
         Image(std::string filepath);
         T getPixel(size_t i, size_t j, size_t c);
         T* getPtr(size_t i, size_t j, size_t c);
         void saveImage(std::string filepath);

//         ~Image();
//         Image(const Image& other);
//         Image& operator=(const Image& other);
};

/*
 * Constructor. Constructs image from its dimensions
 *
 * @param m number of rows
 * @param n number of columns
 * @param c number of channels
 */
template <typename T>
Image<T>::Image(size_t m, size_t n, size_t c)
    : rows{m}
    , cols{n}
    , channels{c}
    , data{Matrix<T> {m, n, c}}
{}

/*
 * Opencv used as a placeholder until I decide to deal with loading and saving image file formats like .png and .jpg
 *
 * Consructs an image given its filename.
 *
 * @param filepath path to the image
 */
template <typename T>
Image<T>::Image(std::string filepath){
    Mat img = imread(filepath);
    if(img.empty())
        throw std::runtime_error("Empty image. Failed to load image.");
    rows = img.rows;
    cols = img.cols;
    channels = img.channels();

    data = Matrix<T> {rows, cols, channels};
    T* imgPtr = img.ptr<T>(0);
    T* dataPtr = data.ptr(0,0,0);
    int len = rows * cols * channels;
    for(int i = 0; i < len; ++i){
        *dataPtr = *imgPtr;
        ++dataPtr;
        ++imgPtr;
    }
}

/**
 * Returns the value of the pixel at position (i,j,c)
 *
 * @param i row index
 * @param j col index
 * @param c channel index
 * @return value at position (i,j,c)
 */
template <typename T>
T Image<T>::getPixel(size_t i, size_t j, size_t c){
    return *(data.ptr(i, j, c));
}

/*
 * Returns the pointer to the value at position (i,j,c)
 *
 * @param i row index
 * @param j col index
 * @param c channel index
 * @return pointer to the value
 */
template <typename T>
T* Image<T>::getPtr(size_t i, size_t j, size_t c){
    return data.ptr(i,j,c);
}


/*
 * Saves the image
 *
 * Uses opencv as a placeholder. 16 = 8unsignedbits 3 channels
 *
 * @param filepath filepath of the destination
 */
template <typename T>
void Image<T>::saveImage(std::string filepath){
    Mat mat{rows, cols, CV_8UC3};
    T* imgPtr = getPtr(0,0,0);
    for(int i = 0; i < rows; ++i){
        unsigned char* Mi = mat.ptr<unsigned char>(i);
        for(int j = 0; j < cols; ++j){
            for(int k = 0; k < channels; ++k){
                Mi[j * channels + k] = *imgPtr; 
                ++imgPtr;
            }
          
        }
    }

    std::cout << "Saving image" <<std::endl;
    imwrite(filepath, mat);
}




#endif
