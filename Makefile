main: main.cpp
	g++ -std=c++20 main.cpp -o main `pkg-config --cflags --libs opencv4`

test_matrix: 
	g++ -std=c++20 test_matrix.cpp -o test_matrix
