#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <vector>

// #include "../src/image.hpp"
#include "../src/core.hpp"
#include "../src/helpers.hpp"

using namespace cv;


int main(){
    
    std::vector<int> costs = {1,0,1,1,
                              1,1,0,1,
                              1,1,1,0,
                              1,1,1,0,
                              1,1,0,1};
    size_t rows = 5;
    size_t cols = 4;

    auto v = shortestVerticalPath(costs, rows, cols);
    for(auto c : v){
        std::cout << c << " ";
    }
    std::cout<<std::endl;
    return 0;    
}
