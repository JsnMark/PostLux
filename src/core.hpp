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






#endif
