# Makefile for monte_carlo simulation

CXX = g++
CXXFLAGS = -std=c++11 -g -Wall -march=native -fopenmp -fno-omit-frame-pointer -O2
CAIROFLAGS = `pkg-config --cflags --libs cairo`
BINS = monte_carlo_vis

$(BINS): $(BINS).cpp
	$(CXX) $(CXXFLAGS) $(CAIROFLAGS) -o $(BINS) $(BINS).cpp
	./monte_carlo_vis
	convert -delay 5 -loop 0 frames/*.png frames/animation.gif

clean:
	rm -Rf $(BINS)

