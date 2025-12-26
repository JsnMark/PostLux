// test_core.cpp

#include <iostream>
#include <gtest/gtest.h>
#include <vector>

#include "../src/image.hpp"
#include "../src/core.hpp"

#define TEST_IMAGE "../data/test/black_and_white.png"


class PadTest : public testing::Test {
    protected:
        PadTest(){
            for(size_t i = 0; i < rows_; ++i){
                for(size_t j = 0; j < cols_; ++j){
                    for(size_t k = 0; k < channels_; ++k){
                        *(im.getPtr(i,j,k)) = val_;
                    }
                }
            }
        } 
        size_t rows_ = 4;
        size_t cols_ = 4;
        size_t channels_ = 3;
        unsigned char val_ = 255;
        Image<unsigned char> im {rows_, cols_, channels_};
};




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

TEST(MergeTest, MergeWorks){
    Image<unsigned char> im {TEST_IMAGE};
    size_t m = im.getRows();
    size_t n = im.getCols();
    size_t c = im.getCha();
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
    EXPECT_EQ(true, im == merge(imageArr));
}

TEST(MergeTest, MergeHandlesInvalidMerges){
    Image<unsigned char> im1 {4,3,1};
    Image<unsigned char> im2 {3,3,1};
    Image<unsigned char> im3 {3,3,1};
    Image<unsigned char> imageArr[3] {im1, im2, im3};

    EXPECT_THROW(merge(imageArr, 0), std::invalid_argument);
    EXPECT_THROW(merge(imageArr), std::runtime_error);
}

TEST_F(PadTest, EvenConstantPaddingAll){
    unsigned char val = im.getPixel(0,0,0); // 4x4 image with 255 as all values
    Image<unsigned char> paddedImage = pad(im, Constant, 2, 2, 2, 2); // 8x8 image
    
    EXPECT_EQ(paddedImage.getRows(), 8);
    EXPECT_EQ(paddedImage.getCols(), 8);
    EXPECT_EQ(paddedImage.getCha(), 3);

    for(size_t i = 0; i < 8; ++i){
        for(size_t j = 0; j < 8; ++j){
            for(size_t k = 0; k < 3; ++k){
                int abc = 0;
                if(i < 2){
                    EXPECT_EQ(paddedImage.getPixel(i,j,k), 0);
                } else if(i >= 6){
                    EXPECT_EQ(paddedImage.getPixel(i,j,k), 0);
                } else if(j < 2){
                    EXPECT_EQ(paddedImage.getPixel(i,j,k), 0);
                } else if(j >= 6){
                    EXPECT_EQ(paddedImage.getPixel(i,j,k), 0);
                } else {
                    EXPECT_EQ(paddedImage.getPixel(i,j,k), 255);
                }
            }
        }
    } 
}

TEST(FilterValidateTest, FilterValidateWorks){
    Image<int> image {10,10,3};
    Matrix<int> filter {5,5,1};
    EXPECT_FALSE(filterValidate(0, 0, image, filter));
    EXPECT_FALSE(filterValidate(1, 1, image, filter));
    EXPECT_FALSE(filterValidate(2, 1, image, filter));
    EXPECT_TRUE(filterValidate(2, 2, image, filter));

    EXPECT_FALSE(filterValidate(9, 9, image, filter));
    EXPECT_FALSE(filterValidate(8, 8, image, filter));
    EXPECT_FALSE(filterValidate(8, 7, image, filter));
    EXPECT_TRUE(filterValidate(7, 7, image, filter));

    Image<int> image1{11, 11, 3};
    Matrix<int> filter1{5,5,1};
    EXPECT_FALSE(filterValidate(0, 0, image1, filter1));
    EXPECT_FALSE(filterValidate(1, 1, image1, filter1));
    EXPECT_FALSE(filterValidate(2, 1, image1, filter1));
    EXPECT_TRUE(filterValidate(2, 2, image1, filter1));

    EXPECT_FALSE(filterValidate(10, 10, image1, filter1));
    EXPECT_FALSE(filterValidate(9, 9, image1, filter1));
    EXPECT_FALSE(filterValidate(9, 8, image1, filter1));
    EXPECT_TRUE(filterValidate(8, 8, image1, filter1));
}

//TEST(FilterTest, FilterThrowsException){
//
//     Image<int> i0{0,0,0};
//     Matrix<int> f0{3,3,1};
//     EXPECT_THROW(filterValidate(0, 0, i0, f0), std::runtime_error);
//     Image<int> i1{5,5,3};
//     Matrix<int> f1{3,3,2};
//     EXPECT_THROW(filterValidate(0, 0, i1, f1), std::runtime_error);
//     Image<int> i2{5,5,3};
//     Matrix<int> f2{2,2,1};
//     EXPECT_THROW(filterValidate(0, 0, i2, f2), std::runtime_error);
//}

int main(int argc, char** argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
