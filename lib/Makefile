CXX=clang++
CXX_FLAGS=-std=c++14 -Weverything -Wno-c++98-compat -Wno-shadow -Wno-padded -Wno-sign-conversion -g
BIN=array_test

build: array2d.h main.cpp
	$(CXX) $(CXX_FLAGS) main.cpp -o $(BIN)

run: build
	./$(BIN)

clean:
	rm -f $(BIN)
