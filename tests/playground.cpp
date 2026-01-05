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
    Image<unsigned char> image {"../data/input/face.jpg"};
    Image<unsigned char> grayU = RGBToGrayscale(image);

    Image<double> grayD {grayU.rows, grayU.cols, 1, DOUBLE};
    convert(grayU, grayD);
    
    Image<double> dest {grayD};
    sobel(grayD, dest);    
    if(dest == grayD){
        std::cout << "EQUAL GRAYD" << std::endl;
    }
    Image<unsigned char> destU {dest.rows, dest.cols, dest.channels, UCHAR};
    convert(dest, destU);
    Image<unsigned char> result = grayscaleToRGB(destU);
    result.saveImage("../data/output/sobel1.jpg");

    return 0;    
}
