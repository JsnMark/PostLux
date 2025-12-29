#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <vector>

// #include "../src/image.hpp"
#include "../src/core.hpp"

using namespace cv;


int main(int argc, char** argv){
    Image<int> image {"../data/input/cind.jpg"};
    std::cout << "Image has dimensions of: " << image.getRows() << ", " << image.getCols() << ", "<< image.getCha()<<std::endl;
     Matrix<int> kernel {5,5,1};
     kernel.fill(std::vector<int> {1,4,7,4,1,
                                   4,16,26,16,4,
                                   7,26,41,26,7,
                                   4,16,26,16,4,
                                   1,4,7,4,1,});
     Image<int> imageCpy {image};
     for(size_t i = 0; i < image.getRows(); ++i){
         for(size_t j = 0; j < image.getCols(); ++j){
             if (filterValidate(i, j, image, kernel)){
                 for(size_t k = 0; k < image.getCha(); ++k){
                     *(image.getPtr(i,j,k)) = applyFilter(i,j,k, imageCpy, kernel) / 273;
                 }
             }
         }
     }
    image.saveImage("../data/output/imagef.jpg"); 
    imageCpy.saveImage("../data/output/imageg.jpg");

    return 0;    
}
