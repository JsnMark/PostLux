// test_matrix.cpp

#include <iostream>
#include <gtest/gtest.h>

#include "../src/matrix.hpp"

class MatrixTest : public testing::Test {
    protected:
        MatrixTest(){
            int* ptr = m1_.ptr(0,0,0);
            for(int i = 0; i < 5*2*3; ++i){
                *ptr = i;
                ++ptr;
            }
            ptr = nullptr;
        }

        Matrix<int> m0_{5,2,3}; // Empty 5x2x3
        Matrix<int> m1_{5,2,3}; // Nonempty 5x2x3
};

TEST_F(MatrixTest, PtrWorks){
    EXPECT_EQ(0, *(m1_.ptr(0,0,0)));
    EXPECT_EQ(6, *(m1_.ptr(1,0,0)));
    EXPECT_EQ(10, *(m1_.ptr(1,1,1))); 
}

TEST_F(MatrixTest, CopyAssignmentWorks){
    m0_ = m1_;
    EXPECT_EQ(0, *(m0_.ptr(0,0,0)));
    EXPECT_EQ(6, *(m0_.ptr(1,0,0)));
    EXPECT_EQ(10, *(m0_.ptr(1,1,1))); 
}

TEST_F(MatrixTest, CopyConstructorWorks){
    Matrix<int> tmp = m1_;
    EXPECT_EQ(0, *(tmp.ptr(0,0,0)));
    EXPECT_EQ(6, *(tmp.ptr(1,0,0)));
    EXPECT_EQ(10, *(tmp.ptr(1,1,1))); 
}

TEST_F(MatrixTest, GetWorks){
    EXPECT_EQ(5, m1_.getRow());
    EXPECT_EQ(2, m1_.getCol());
    EXPECT_EQ(3, m1_.getCh());
}


int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

