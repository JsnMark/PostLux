//helpers.hpp

#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <vector>
#include <exception>
#include <iostream>
#include <algorithm>




/* 
 * Takes a vector of costs and finds shortest path from top to bottom
 * which minimizes total cost of the path. Path should not shift more than
 * 1 spot between each row. 
 *
 * @param costs vector of costs
 * @return path vector of length H. path[i] contains column index of path for each row i
 */
template <typename T>
std::vector<size_t> shortestVerticalPath(std::vector<T> costs, size_t rows, size_t cols){
    if(cols < 3){
        throw std::runtime_error("Must have at least 3 columns");
    }
    if(costs.size() != rows * cols){
        throw std::runtime_error("Invalid size of cost vector");
    }

    // Use new to avoid using resize() or vector copy constructor which are O(n)
    // which could lead up to millions of iterations
    T* accPtr = new T[costs.size()];
    T* tmp = accPtr;
    T* costData = costs.data();

    // copy firstRow to accumulator
    for(size_t i = 0; i < cols; ++i){
        *tmp = *costData;
        ++tmp;
        ++costData;
    }
    size_t* choicesPtr = new size_t[costs.size()];

    // DP. Take node with least cost
    for(size_t i = 1; i < rows; ++i){
        size_t min_idx;
        T min_val;
        for(size_t j = 0; j < cols; ++j){
            min_idx = j;
            min_val = accPtr[(i-1)*cols + j];
            if(j >= 1 && accPtr[(i-1)*cols + j - 1] < min_val){
                min_idx = j-1;
                min_val = accPtr[(i-1)*cols + j - 1];
            }
            if(j + 1 < cols && accPtr[(i-1)*cols + j + 1] < min_val){
                min_idx = j+1;
                min_val = accPtr[(i-1)*cols + j + 1];
            }

            accPtr[i*cols + j] = costs[i*cols + j] + min_val;
            choicesPtr[i*cols + j] = min_idx;
        }
    }

    // Find the end of the path
    size_t minIndex = cols - 1;
    T minVal = accPtr[costs.size() -1];
    size_t lastRow = rows - 1;
    for(size_t j = 0; j < cols; ++j){
        if(accPtr[lastRow * cols + j] < minVal){
            minIndex = j;
            minVal = accPtr[lastRow * cols + j];
        }
    }

    // Add back the path, from back to front. Then reverse.
    std::vector<size_t> path {};
    path.emplace_back(minIndex);
    for(size_t i = rows - 1; i > 0; --i){
        minIndex = choicesPtr[i*cols + minIndex];
        path.emplace_back(minIndex);
    }
    std::reverse(path.begin(), path.end());
    return path;
}








#endif
