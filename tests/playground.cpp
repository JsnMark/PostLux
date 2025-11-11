#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <vector>


using namespace cv;

//typedef std::vector<std::vector<std::vector<int>>> image_vector;

int main(int argc, char** argv){
    Mat mat = imread("../data/input/face.jpg");
    std::cout << mat.rows << " " << mat.cols << " " << mat.channels() << std::endl; 
    uchar arr[mat.rows * mat.cols];
    int count = 0;
    for(int i = 0; i < mat.rows; ++i){
        unsigned char* Mi = mat.ptr<unsigned char>(i);
        int c = 0;
        for(int j = 0; j < mat.cols; ++j){
            Mi[j*3] = 0;
            count++;
        }
    }
    std::cout<<"Count: " << count<<std::endl;
   imwrite("../data/output/tmp.jpg", mat); 

    return 0;    
}
