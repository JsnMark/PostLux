// test_helpers.cpp

#include <iostream>
#include <gtest/gtest.h>
#include <vector>

#include "../src/helpers.hpp"

TEST(VerticalShortestPath, VSPWorks){
    std::vector<int> costs = {1,0,1,1,
                              1,1,0,1,
                              1,1,1,0,
                              1,1,1,0,
                              1,1,0,1};
    size_t r = 5;
    size_t c = 4; 

    auto v = shortestVerticalPath(costs, r, c);
    std::vector<size_t> valid = {1,2,3,3,2};
    EXPECT_TRUE(v == valid);
}

TEST(VerticalShortestPath, AntiNaiveImplementationAndFirstMinIsLastItem){
    std::vector<int> costs = {  0,  0,  9,  8,
                                0, 10, 10,  0,
                                1,  1,  5,  0,
                              999,999,999,  0};
    size_t r = 4, c = 4;
    auto v = shortestVerticalPath(costs, r, c);
    std::vector<size_t> valid = {3,3,3,3};
    EXPECT_TRUE(v == valid);
}

TEST(VerticalShortestPath, VSPNegatives){
    std::vector<int> costs = {  -1,  1,  -1,  1,
                                1, -1, 1,  1,
                                -1,  1,  -1,  1,
                              999,999,999,  1};
    size_t r = 4, c = 4;
    auto v = shortestVerticalPath(costs, r, c);
    std::vector<size_t> valid = {0,1,2,3};
    EXPECT_TRUE(v == valid);
}

TEST(VerticalShortestPath, AntiNaiveImplementation){
    std::vector<int> costs = {0,5,5,5,
                              0,5,5,5,
                              0,5,5,5,
                              999,999,999,100};
    size_t r = 4, c = 4;
    auto v = shortestVerticalPath(costs, r, c);
    std::vector<size_t> valid = {0,1,2,3};
    EXPECT_TRUE(v == valid);
}

TEST(VerticalShortestPath, VSPThrows){
    std::vector<int> costs = {0,0,0,0};
    size_t r = 0, c = 0;
    EXPECT_THROW(shortestVerticalPath(costs, r, c), std::runtime_error);

    r = 3;
    c = 3;
    EXPECT_THROW(shortestVerticalPath(costs, r, c), std::runtime_error); 
}


int main(int argc, char** argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
