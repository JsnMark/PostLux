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

TEST_F(MatrixTest, PixelWorks){
    *(m1_.ptr(0,0,0)) = 12;
    EXPECT_EQ(m1_.pixel(0,0,0), 12); 
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

TEST_F(MatrixTest, FillWorks){
    Matrix<int> mat1 {3,3,3};
    int val = 100;
    mat1.fill(val);
    for(int i = 0; i < mat1.getRow(); ++i){
        for(int j = 0; j < mat1.getCol(); ++j){
            for(int k = 0; k < mat1.getCh(); ++k){
                EXPECT_EQ(mat1.pixel(i,j,k), val);
            }
        }
    } 
    int val1 = 67;
    mat1.fill(val1, 1);
    for(int i = 0; i < mat1.getRow(); ++i){
        for(int j = 0; j < mat1.getCol(); ++j){
            for(int k = 0; k < mat1.getCh(); ++k){
                if(k == 1){
                    EXPECT_EQ(mat1.pixel(i,j,k), val1);
                } else{
                    EXPECT_EQ(mat1.pixel(i,j,k), val);
                }
            }
        }
    } 
}

TEST_F(MatrixTest, FillThrows){
    Matrix<int> mat {3,3,3};
    EXPECT_THROW(mat.fill(0, 5), std::runtime_error);
}

TEST_F(MatrixTest, FillVectorWorks){
    Matrix<int> mat {3,3,1};
    std::vector<int> vec {1,2,3,4,5,6,7,8,9};
    mat.fill(vec);
    int* ptr = mat.ptr(0,0,0);
    for(int i = 0; i < 9; ++i){
        EXPECT_EQ(*ptr, vec.at(i));
        ++ptr; 
    }

    Matrix<int> mat1 {2,3,2};
    std::vector<int> vec1{0,1, 2,3, 4,5,
                         6,7, 8,9, 10,11};
    mat1.fill(vec1);
    ptr = mat1.ptr(0,0,0);
    for(int i = 0; i < 12; ++i){
        EXPECT_EQ(*ptr, vec1.at(i));
        ++ptr; 
    }

    Matrix<int> mat2{3,3,3};
    mat2.fill(0);
    std::vector<int> vec2{1,1,1,
                         1,1,1,
                         1,1,1};
    mat2.fill(vec2, 1);
    for(int i = 0; i < mat2.getRow(); ++i){
        for(int j = 0; j < mat2.getCol(); ++j){
            for(int k = 0; k < mat2.getCh(); ++k){
                if(k == 1)
                    EXPECT_EQ(mat2.pixel(i,j,k), 1);
                else
                    EXPECT_EQ(mat2.pixel(i,j,k), 0);
            }
        }
    }
}

TEST_F(MatrixTest, FillVectorThrows){
    Matrix<int> mat {2,2,3};
    std::vector<int> vec0 {1,2,3,4,5,6,7,8,9,10,11,12};
    std::vector<int> vec1 {0};
    EXPECT_THROW(mat.fill(vec0, -3), std::runtime_error);
    EXPECT_THROW(mat.fill(vec0, 30), std::runtime_error);
    EXPECT_THROW(mat.fill(vec1), std::runtime_error);
    EXPECT_THROW(mat.fill(vec1, 0), std::runtime_error); 
}

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

