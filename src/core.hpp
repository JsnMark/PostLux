// core.hpp

#ifndef CORE_HPP
#define CORE_HPP

#include "image.hpp"


/* Splits image into many single channeled images. Caller must allocated appropriate space.
 * Pass by result
 *
 * @param src Image to be split
 * @param destBegin Beginning of the array containing the split images 
 */
template <typename T>
void split(const Image<T>& src, Image<T>* destBegin){
    size_t rows = src.getRows();
    size_t cols = src.getCols();
    size_t channels = src.getCha();

    for(int c = 0; c < channels; ++c){
        Image<T>* destPtr = destBegin + c;
        T* dataPtr = destPtr->getPtr(0,0,0);
        for(int i = 0; i < rows; ++i){
            for(int j = 0; j < cols; ++j){
                *dataPtr = src.getPixel(i,j,c);
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
    size_t rows = srcBegin->getRows();
    size_t cols = srcBegin->getCols();
    for(int curIm = 1; curIm < numImages; ++curIm){
        if((srcBegin + curIm)->getRows() != rows ||
           (srcBegin + curIm)->getCols() != cols ||
           (srcBegin + curIm)->getCha() != 1){
            throw std::runtime_error("Images have mismatched dimensions");
        }
    }

    Image<T> result {rows, cols, numImages}; 
    for(int i = 0; i < rows; ++i){
        for(int j = 0; j < cols; ++j){
            for(int k = 0; k < numImages; ++k){
                *(result.getPtr(i,j,k)) = (srcBegin + k)->getPixel(i,j,0); 
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

    size_t rows = image.getRows() + top + bottom;
    size_t cols = image.getCols() + left + right;
    size_t channels = image.getCha();
    Image<T> newImage {rows, cols, channels};

    size_t bottomIndex = rows - bottom;
    size_t rightIndex = cols - right;
    T* elementPtr = newImage.getPtr(0,0,0);
    for(size_t i = 0; i < rows; ++i){
        for(size_t j = 0; j < cols; ++j){
            for(size_t k = 0; k < channels; ++k){
                if(i < top || i >= bottomIndex || j < left || j >= rightIndex){
                    *elementPtr = value;    
                } else{
                    *elementPtr = image.getPixel(i - left, j - right, k); 
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
    size_t x = filter.getRow() / 2;
    size_t y = filter.getCol() / 2;
    if(i < x)
        return false;
    if(j < y)
        return false;
    if((image.getRows() - i) < (x + 1))
        return false;
    if((image.getCols() - j) < (y + 1))
        return false;
    return true;
}

/*
 * Returns value of the pixel after applying filter to image. 
 *
 * @param x row number
 * @param y col number
 * @param z channel number
 * @param image
 * @param filter
 * @return sum value of the pixel.
 */
template <typename T>
double applyFilter(size_t x, size_t y, size_t z, const Image<T>& image, const Matrix<T>& filter){
    int a = (int) filter.getRow() / 2;
    int b = (int) filter.getCol() / 2;
    T sum = 0;
    for(int i = - a; i <= a; ++i){
        for(int j = -b; j <= b; ++j){
           sum += filter.getPixel(i,j,0) * image.getPixel(x-i,y-j,z);  
        }
    } 
    return sum;
}


/*
 * Applies a filter to an image using convolution. Caller must pad image if required
 *
 * @param image Image to be convolved. May be modified if inplace=true 
 * @param filter Matrix filter used in convolution
 */
template <typename T>
void filter(Image<T>& image, Matrix<T>& filter){
    size_t mI = image.getRows();
    size_t nI = image.getCols();
    size_t cI = image.getCha();
    size_t mF = filter.getRow();
    size_t nF = filter.getCol();
    size_t cF = filter.getCha();

    // Filter smaller than image, single channeled, odd length and width
    if((mF > mI) || (nF > nI) || (cF != 1) || ((mF % 2) == 0) || ((nF % 2) == 0)){
        throw std::runtime_error("Incorrect filter dimensions");
    }

    for(size_t i = 0; i < mI; ++i){
        for(size_t j = 0; j < nI; ++j){
            if (filterValidate(i, j, image, filter)){
                for(size_t k = 0; k < cI; ++k){
                    applyFilter(i,j,k, image, filter);
                }
            }
        }
    } 
} 


#endif
