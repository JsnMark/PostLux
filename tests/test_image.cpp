// test_image.cpp

#include <iostream>
#include <gtest/gtest.h>

#include "../src/image.hpp"

#define TEST_IMAGE "../data/test/black_and_white.png"

// class ImageTest : public testing::Test {
 //    protected:

// };




// Must run this with memory checker like Valgrind or leaks
TEST(ImageTest, NoLeaks){
    Image<int> i {1,2,3};
    EXPECT_EQ(0, 0);
}

TEST(ImageTest, LoadImageAndCheckDimensions){
    Image<unsigned char> i {TEST_IMAGE};
    EXPECT_EQ(i.rows, 8);
    EXPECT_EQ(i.cols, 8);
    EXPECT_EQ(i.channels, 3);
}

TEST(ImageTest, PtrWorks){
    Image<unsigned char> i {TEST_IMAGE};
    EXPECT_EQ(*(i.ptr(0,0,0)), 0); 
    EXPECT_EQ(*(i.ptr(0,0,1)), 0); 
    EXPECT_EQ(*(i.ptr(0,0,2)), 0); 

    EXPECT_EQ(*(i.ptr(7,7,0)), 255); 
    EXPECT_EQ(*(i.ptr(7,7,1)), 255); 
    EXPECT_EQ(*(i.ptr(7,7,2)), 255); 
}

TEST(ImageTest, GetPixelWorks){
    Image<unsigned char> i {TEST_IMAGE};
    EXPECT_EQ(i.pixel(0,0,0), 0); 
    EXPECT_EQ(i.pixel(0,0,1), 0); 
    EXPECT_EQ(i.pixel(0,0,2), 0); 

    EXPECT_EQ(i.pixel(7,7,0), 255); 
    EXPECT_EQ(i.pixel(7,7,1), 255); 
    EXPECT_EQ(i.pixel(7,7,2), 255); 
}

TEST(ImageTest, SaveImageWorks){
    std::string dest {"../data/test/result.png"};
    Image<unsigned char> image {TEST_IMAGE};
    unsigned char* image_ptr = image.ptr(0,0,0);
    for(size_t i = 0; i < image.rows; ++i){
        for(size_t j = 0; j < image.cols; ++j){
            for(size_t c = 0; c < image.channels; ++c){
                *image_ptr = 128;
                ++image_ptr;
            }
        }
    }
    image.saveImage(dest);

    Image<unsigned char> result {dest};
    for(size_t i = 0; i < result.rows; ++i){
        for(size_t j = 0; j < result.cols; ++j){
            for(size_t c = 0; c < result.channels; ++c){
                EXPECT_EQ(result.pixel(i,j,c), 128); 
            }
        }
    }
}

TEST(ImageTest, CopyConstructorWorks){
    Image<unsigned char> image {TEST_IMAGE};
    Image<unsigned char> copyImage (image);
    for(size_t i = 0; i < image.rows; ++i){
        for(size_t j = 0; j < image.cols; ++j){
            for(size_t k = 0; k < image.channels; ++k){
                EXPECT_EQ(image.pixel(i,j,k), copyImage.pixel(i,j,k));
            }
        }
    }
}

TEST(ImageTest, EqualityWorks){
    Image<unsigned char> image1 {TEST_IMAGE};
    Image<unsigned char> image2 {TEST_IMAGE};
    EXPECT_EQ(image1==image2, true);

    *(image1.ptr(1,1,1)) = image1.pixel(1,1,1) + 1;
    EXPECT_EQ(image1==image2, false);
}

TEST(ImageTest, InequalityWorks){
    Image<unsigned char> image1 {TEST_IMAGE};
    Image<unsigned char> image2 {TEST_IMAGE};
    EXPECT_EQ(image1!=image2, false);

    *(image1.ptr(1,1,1)) = image1.pixel(1,1,1) + 1;
    EXPECT_EQ(image1!=image2, true);
}

TEST(ImageTest, FillWorks){
    Image<int> image {3,3,3};
    int val = 1;
    image.fill(val);
    for(size_t i = 0; i < image.rows; ++i){
        for(size_t j = 0; j < image.cols; ++j){
            for(size_t k = 0; k < image.channels; ++k){
                EXPECT_EQ(image.pixel(i,j,k), val);
            }
        }
    }
    EXPECT_THROW(image.fill(3, 100), std::runtime_error);

    Image<int> image1{2,2,1};
    std::vector<int> vec {1,2,3,4};
    image1.fill(vec);
    int* p = image1.ptr(0,0,0);
    for(int i = 0; i < 4; ++i){
        EXPECT_EQ(vec.at(i), *p);
        ++p;
    }
}

int main(int argc, char** argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
