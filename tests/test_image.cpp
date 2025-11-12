// test_image.cpp

#include <iostream>
#include <gtest/gtest.h>

#include "../src/image.hpp"

// class ImageTest : public testing::Test {
 //    protected:

// };




// Must run this with memory checker like Valgrind or leaks
TEST(ImageTest, NoLeaks){
    Image i {1,2,3};
    EXPECT_EQ(0, 0);
}

TEST(ImageTest, LoadImageAndGetPixelWorks){
    Image i {1, 2, 3};
}

int main(int argc, char** argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
