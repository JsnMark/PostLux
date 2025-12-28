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
    EXPECT_EQ(i.getRows(), 8);
    EXPECT_EQ(i.getCols(), 8);
    EXPECT_EQ(i.getCha(), 3);
}

TEST(ImageTest, GetPtrWorks){
    Image<unsigned char> i {TEST_IMAGE};
    EXPECT_EQ(*(i.getPtr(0,0,0)), 0); 
    EXPECT_EQ(*(i.getPtr(0,0,1)), 0); 
    EXPECT_EQ(*(i.getPtr(0,0,2)), 0); 

    EXPECT_EQ(*(i.getPtr(7,7,0)), 255); 
    EXPECT_EQ(*(i.getPtr(7,7,1)), 255); 
    EXPECT_EQ(*(i.getPtr(7,7,2)), 255); 
}

TEST(ImageTest, GetPixelWorks){
    Image<unsigned char> i {TEST_IMAGE};
    EXPECT_EQ(i.getPixel(0,0,0), 0); 
    EXPECT_EQ(i.getPixel(0,0,1), 0); 
    EXPECT_EQ(i.getPixel(0,0,2), 0); 

    EXPECT_EQ(i.getPixel(7,7,0), 255); 
    EXPECT_EQ(i.getPixel(7,7,1), 255); 
    EXPECT_EQ(i.getPixel(7,7,2), 255); 
}

TEST(ImageTest, SaveImageWorks){
    std::string dest {"../data/test/result.png"};
    Image<unsigned char> image {TEST_IMAGE};
    unsigned char* image_ptr = image.getPtr(0,0,0);
    for(int i = 0; i < image.getRows(); ++i){
        for(int j = 0; j < image.getCols(); ++j){
            for(int c = 0; c < image.getCha(); ++c){
                *image_ptr = 128;
                ++image_ptr;
            }
        }
    }
    image.saveImage(dest);

    Image<unsigned char> result {dest};
    for(int i = 0; i < result.getRows(); ++i){
        for(int j = 0; j < result.getCols(); ++j){
            for(int c = 0; c < result.getCha(); ++c){
                EXPECT_EQ(result.getPixel(i,j,c), 128); 
            }
        }
    }
}

TEST(ImageTest, CopyConstructorWorks){
    Image<unsigned char> image {TEST_IMAGE};
    Image<unsigned char> copyImage (image);
    for(int i = 0; i < image.getRows(); ++i){
        for(int j = 0; j < image.getCols(); ++j){
            for(int k = 0; k < image.getCha(); ++k){
                EXPECT_EQ(image.getPixel(i,j,k), copyImage.getPixel(i,j,k));
            }
        }
    }
}

TEST(ImageTest, EqualityWorks){
    Image<unsigned char> image1 {TEST_IMAGE};
    Image<unsigned char> image2 {TEST_IMAGE};
    EXPECT_EQ(image1==image2, true);

    *(image1.getPtr(1,1,1)) = image1.getPixel(1,1,1) + 1;
    EXPECT_EQ(image1==image2, false);
}

TEST(ImageTest, InequalityWorks){
    Image<unsigned char> image1 {TEST_IMAGE};
    Image<unsigned char> image2 {TEST_IMAGE};
    EXPECT_EQ(image1!=image2, false);

    *(image1.getPtr(1,1,1)) = image1.getPixel(1,1,1) + 1;
    EXPECT_EQ(image1!=image2, true);
}

TEST(ImageTest, FillWorks){
    Image<int> image {3,3,3};
    int val = 1;
    image.fill(val);
    for(int i = 0; i < image.getRows(); ++i){
        for(int j = 0; j < image.getCols(); ++j){
            for(int k = 0; k < image.getCha(); ++k){
                EXPECT_EQ(image.getPixel(i,j,k), val);
            }
        }
    }
    EXPECT_THROW(image.fill(3, 100), std::runtime_error);

    Image<int> image1{2,2,1};
    std::vector<int> vec {1,2,3,4};
    image1.fill(vec);
    int* p = image1.getPtr(0,0,0);
    for(int i = 0; i < 4; ++i){
        EXPECT_EQ(vec.at(i), *p);
        ++p;
    }
}

int main(int argc, char** argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
