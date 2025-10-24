#include <iostream>
#include <string>
#include <gtest/gtest.h>
#include <iostream>

#include "matrix.hpp"



int test_init(){    
    Matrix<int> m(3, 4);
    return 0;
}





int main(){
    test_init(); 

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
