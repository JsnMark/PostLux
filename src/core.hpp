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




#endif
