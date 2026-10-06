CXX = clang++
CXXFLAGS = -std=c++23 -Wall -Iinclude -I/opt/homebrew/include
LDFLAGS = -L/opt/homebrew/lib -Wl,-rpath,/opt/homebrew/lib -lSDL3

# This automatically finds main.cpp and everything inside your src/ folder
SRCS = main.cpp $(wildcard src/*.cpp)

all: main

main: $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o main $(LDFLAGS)

clean:
	rm -f main
