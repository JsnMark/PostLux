#include <iostream>
#include <string>
//#include <gtest/gtest.h>

#include "../src/matrix.hpp"
#include <vector>


int test_init(){    
    Matrix<int> m1(3, 4);
    return 0;
}





int main(){
    test_init(); 

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
