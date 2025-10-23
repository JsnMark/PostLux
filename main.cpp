#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <vector>


using namespace cv;

typedef std::vector<std::vector<std::vector<int>>> image_vector;


int main(int argc, char** argv){
    Mat I = imread("cind.jpg");
    std::cout << I.rows << " " << I.cols << " " << I.channels() << std::endl; 
    image_vector vec3d(I.rows, std::vector<std::vector<int>>(I.cols, std::vector<int>(I.channels())));
   

    for(int i = 0; i < I.rows; ++i){
        std::cout << vec3d[i][50][0];
    }
//    std::cout << vec3d[123][521][1] << std::endl;
    return 0;    
}
