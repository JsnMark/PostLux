main_debug: main.cpp
	g++ -std=c++20 main.cpp -g -o build/main `pkg-config --cflags --libs opencv4`

test_matrix: test_matrix.cpp 
	g++ -std=c++20 test_matrix.cpp -g -fsanitize=address -o build/test_matrix
