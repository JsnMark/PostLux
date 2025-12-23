// test_core.cpp

#include <iostream>
#include <gtest/gtest.h>
#include <vector>

#include "../src/image.hpp"
#include "../src/core.hpp"

#define TEST_IMAGE "../data/test/black_and_white.png"

TEST(SplitTest, SplitTestWorks){
    Image<unsigned char> im {TEST_IMAGE};
    size_t m = im.getRows();
    size_t n = im.getCols();
    size_t c = im.getCha();
    // Make first channel of image all 10
    int val = 10;
    for(int i = 0; i < m; ++i){
        for(int j = 0; j < n; ++j){
            *(im.getPtr(i,j,0)) = val;
        }
    }
    
    Image<unsigned char>* imageArr = new Image<unsigned char>[c];

    for(int i = 0; i < c; ++i){
        imageArr[i] = Image<unsigned char> {m,n,1};
    }

    split(im, imageArr);
    // Check same width and height, but single channeled
    for(int i = 0; i < c; ++i){
        EXPECT_EQ((imageArr + i)->getRows(), m);
        EXPECT_EQ((imageArr + i)->getCols(), m);
        EXPECT_EQ((imageArr + i)->getCha(), 1);
    }
    // Check correct values for first channel
    for(int i = 0; i < m; ++i){
        for(int j = 0; j < n; ++j){
            EXPECT_EQ(imageArr->getPixel(i,j,0), val);
        }
    }
    // Check correct values for rest of channels
    for(int k = 1; k < c; ++k){
        for(int i = 0; i < m; ++i){
            for(int j = 0; j < n; ++j){
                EXPECT_EQ((imageArr + k)->getPixel(i,j,0), im.getPixel(i,j,k)); 
            }
        }
    }
    delete[] imageArr;
}

int main(int argc, char** argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
