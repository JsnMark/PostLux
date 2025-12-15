#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <vector>

#include "../src/image.hpp"


using namespace cv;

//typedef std::vector<std::vector<std::vector<int>>> image_vector;

int main(int argc, char** argv){
    Mat mat = imread("../data/input/face.jpg");
    std::cout << mat.rows << " " << mat.cols << " " << mat.channels() << std::endl; 
    std::cout << mat.rows * mat.cols * mat.channels() << std::endl;
    std::cout << mat.type() << std::endl;
    int count = 0;
    for(int i = 0; i < mat.rows; ++i){
        unsigned char* Mi = mat.ptr<unsigned char>(i);
        int c = 0;
        for(int j = 0; j < mat.cols; ++j){
 //           Mi[j * mat.channels() + 0] = 0;
//            Mi[j * mat.channels() + 1] = 0;
            Mi[j * mat.channels() + 2] = 255;
            count++;
        }
    }
    std::cout<<"Count: " << count<<std::endl;
    imwrite("../data/output/tmp.jpg", mat); 

    Image<unsigned char> img {"../data/output/tmp.jpg"};
    int irows = mat.rows;
    int icols = mat.cols;
    int icha = mat.channels();
    for(int i = 0; i < irows; ++i){
        for(int j = 0; j < icols; ++j){
            *img.getPtr(i, j, 0) = 0;
        }
    }
    img.saveImage("../data/output/tmp_img.jpg");

    

    return 0;    
}
