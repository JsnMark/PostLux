// core.hpp

#ifndef CORE_HPP
#define CORE_HPP

#include <functional>

#include "image.hpp"
#include "helpers.hpp"

/* Splits image into many single channeled images. Caller must allocated appropriate space.
 * Pass by result
 *
 * @param src Image to be split
 * @param destBegin Beginning of the array containing the split images 
 */
template <typename T>
void split(const Image<T>& src, Image<T>* destBegin){
    size_t rows = src.rows;
    size_t cols = src.cols;
    size_t channels = src.channels;

    for(size_t c = 0; c < channels; ++c){
        Image<T>* destPtr = destBegin + c;
        T* dataPtr = destPtr->ptr(0,0,0);
        for(size_t i = 0; i < rows; ++i){
            for(size_t j = 0; j < cols; ++j){
                *dataPtr = src.pixel(i,j,c);
                ++dataPtr;
            }
        }
    }
}

/*
 * Merges multiple single channeled images into one multi channelled image
 *  
 * @param pointer to beginning of array of images to be merged
 * @param numImages number of images to be merged
 * @return result merged image
 */
template <typename T>
Image<T> merge(Image<T>* srcBegin, size_t numImages = 3){
    if(numImages <= 1){
        throw std::invalid_argument("numImages must be greater than 1");
    } 
    // Ensure all correct dimensions
    size_t rows = srcBegin->rows;
    size_t cols = srcBegin->cols;
    for(size_t curIm = 1; curIm < numImages; ++curIm){
        if((srcBegin + curIm)->rows != rows ||
           (srcBegin + curIm)->cols != cols ||
           (srcBegin + curIm)->channels != 1){
            throw std::runtime_error("Images have mismatched dimensions");
        }
    }

    Image<T> result {rows, cols, numImages}; 
    for(size_t i = 0; i < rows; ++i){
        for(size_t j = 0; j < cols; ++j){
            for(size_t k = 0; k < numImages; ++k){
                *(result.ptr(i,j,k)) = (srcBegin + k)->pixel(i,j,0); 
            }    
        }
    }
    return result;
}


enum Padding_type {
    Constant
};

/*
 * Pads the image
 *
 * @param image Image to be padded
 * @param paddingType how the image will be padded {Constant}
 * @param top rows padded on top
 * @param bottom rows padded on bottom
 * @param left cols padded on left
 * @param right cols padded on right
 * @param value value to be padded with. May not be used depending on paddingType
 * @return newImage padded image
 */
template <typename T>
Image<T> pad(const Image<T>& image, Padding_type paddingType, 
             size_t top, size_t bottom, size_t left, size_t right,
             T value=0){

    size_t rows = image.rows + top + bottom;
    size_t cols = image.cols + left + right;
    size_t channels = image.channels;
    Image<T> newImage {rows, cols, channels};

    size_t bottomIndex = rows - bottom;
    size_t rightIndex = cols - right;
    T* elementPtr = newImage.ptr(0,0,0);
    for(size_t i = 0; i < rows; ++i){
        for(size_t j = 0; j < cols; ++j){
            for(size_t k = 0; k < channels; ++k){
                if(i < top || i >= bottomIndex || j < left || j >= rightIndex){
                    *elementPtr = value;    
                } else{
                    *elementPtr = image.pixel(i - left, j - right, k); 
                }
                elementPtr++; 
            } 
        }
    }
    return newImage;
}


/*
 * Helper function for filter. Returns true if can apply filter at that pixel
 *
 * @param i row index of pixel
 * @param j col index of pixel
 * @param image
 * @param filter
 * @return bool true if valid pixel for filtering. Else false
 */
template <typename T>
bool filterValidate(const size_t& i, const size_t& j, const Image<T>& image, Matrix<T>& filter){
    size_t x = filter.rows / 2;
    size_t y = filter.cols / 2;
    if(i < x)
        return false;
    if(j < y)
        return false;
    if((image.rows - i) < (x + 1))
        return false;
    if((image.cols - j) < (y + 1))
        return false;
    return true;
}

/*
 * Returns value of the pixel after applying filter to image. X,Y coordinates correspond to middle of the filter 
 *
 * @param x row number
 * @param y col number
 * @param z channel number
 * @param image
 * @param filter
 * @return sum value of the pixel.
 */
template <typename T>
double applyFilter(int x, int y, int z, const Image<T>& image, const Matrix<T>& filter){
    int a = (int) filter.rows / 2;
    int b = (int) filter.cols / 2;
    T sum = 0;

    for(int i = - a; i <= a; ++i){
        for(int j = -b; j <= b; ++j){
           sum += filter.pixel(i+a,j+b,0) * image.pixel(x-i,y-j,z);  
        }
    } 
    return sum;
}


/*
 * Applies a filter to an image using convolution. Caller must pad image if required
 * Won't explicitly handle types
 *
 * @param image Image to be convolved. May be modified if inplace=true 
 * @param filter Matrix filter used in convolution
 */
template <typename T>
void filter(Image<T>& image, Matrix<T>& filter){
    size_t mI = image.rows;
    size_t nI = image.cols;
    size_t cI = image.channels;
    size_t mF = filter.rows;
    size_t nF = filter.cols;
    size_t cF = filter.channels;

    // Filter smaller than image, single channeled, odd length and width
    if((mF > mI) || (nF > nI) || (cF != 1) || ((mF % 2) == 0) || ((nF % 2) == 0)){
        throw std::runtime_error("Incorrect filter dimensions");
    }
    Image<T> imageCpy {image};
    for(size_t i = 0; i < mI; ++i){
        for(size_t j = 0; j < nI; ++j){
            if (filterValidate(i, j, image, filter)){
                for(size_t k = 0; k < cI; ++k){
                    *(image.ptr(i,j,k)) = applyFilter(i,j,k, imageCpy, filter);
                }
            }
        }
    } 
} 

/*
 * Converts an image from one data type to another
 *
 * @param srcImage source Image with datatype T
 * @param destImage destination of Image with datatype U
 */
template <typename T, typename U>
void convert(Image<T>& srcImage, Image<U>& destImage){
    if(srcImage.rows != destImage.rows 
            || srcImage.cols != destImage.cols
            || srcImage.channels != destImage.channels){
        throw std::runtime_error("Incompatible images");
    }

    std::function<U(T, double)> func;    
    if(srcImage.datatype == UCHAR && destImage.datatype == DOUBLE){
        func = [](unsigned char a, double b){
            return static_cast<double>(a) / b;
        };
    } else if (srcImage.datatype == DOUBLE && destImage.datatype == UCHAR){
        func = [](double a, double b){
            double c = a * b;
            if(c < 0){
                return static_cast<unsigned char>(0);
            }
            if(c > 255){
                return static_cast<unsigned char>(255);
            }
            return static_cast<unsigned char> (c);
        };
    }

    double high = 255.0;
    size_t length = srcImage.rows * srcImage.cols;
    T* tptr = srcImage.ptr(0,0,0);
    U* uptr = destImage.ptr(0,0,0);
    for(size_t i = 0; i < length; ++i){
        *uptr = static_cast<U>(func(*tptr, high));
        ++tptr;
        ++uptr;
    }
}


/*
 * Converts grayscale image to RGB
 * 
 * @param image
 * @return returnImage
 */
template <typename T>
Image<T> grayscaleToRGB(Image<T> image){
    if(image.channels != 1){
        throw std::runtime_error("image must be grayscale");
    }
    Image<T> returnImage {image.rows, image.cols, 3, image.datatype};
    for(size_t i = 0; i < image.rows; ++i){
        for(size_t j = 0; j <image.cols; ++j){
            for(size_t k = 0; k < returnImage.channels; ++k){
                *returnImage.ptr(i,j,k) = image.pixel(i,j,0);
            }
        }
    }
    return returnImage;
}

/*
 * Converts image to single channeled grayscale image
 *
 * @param image colored image
 * @return returnImage grayscale image
 */
template<typename T>
Image<T> RGBToGrayscale(Image<T> image){
    if(image.channels !=3){
        throw std::runtime_error("Image must be colored");
    }
    Image<T> returnImage {image.rows, image.cols, 1, image.datatype};
    for(size_t i = 0; i < image.rows; ++i){
        for(size_t j = 0; j < image.cols; ++j){
           *(returnImage.ptr(i,j,0)) = static_cast<unsigned char> (0.2989 * static_cast<double>(*image.ptr(i,j,0))
                                                                 + 0.5870 * static_cast<double>(*image.ptr(i,j,1))
                                                                 + 0.1140 * static_cast<double>(*image.ptr(i,j,2)));
        }
    }
    return returnImage;
}

/* 
 * Applies sobel operator to a grayscale image
 *
 * @param srcImage
 * @param destImage
 */
template <typename T>
void sobel(Image<T>& srcImage, Image<T>& destImage){
    if(srcImage.rows != destImage.rows || srcImage.cols != destImage.cols || srcImage.channels != destImage.channels || srcImage.channels != 1){
        throw std::runtime_error("Image dimensions don't match");
    }
    Matrix<T> fx {3,3,1};
    fx.fill(std::vector<double>{-1, 0, 1,
                        -2, 0, 2,
                        -1, 0, 1});
    Matrix<T> fy {3,3,1};
    fy.fill(std::vector<double>{-1, -2, -1,
                         0,  0,  0,
                         1,  2,  1});

    Image<T> gx {srcImage};
    Image<T> gy {srcImage};

    Image<T> tmp {gx};

    filter(gx, fx);
    filter(gy, fy);

    T* destPtr = destImage.ptr(0,0,0);
    T* gxPtr = gx.ptr(0,0,0); 
    T* gyPtr = gy.ptr(0,0,0);
    for(size_t i = 0; i < destImage.rows; ++i){
        for(size_t j = 0; j < destImage.cols; ++j){
            *destPtr = *gxPtr + *gyPtr;
            ++destPtr;
            ++gxPtr;
            ++gyPtr; 
        }
    }
}

// template <typename T>
// Image<T> removeVerticalSeam(Image<T> image){
//     Image<T> newImage {image.rows, image.cols - 1, image.channels, image.datatype}; 
//     std::vector<size_t> path = 
// }


#endif
