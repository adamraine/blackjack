debug_or_optimize = -O3

CXX = g++
CXXFLAGS = -Wall -pedantic --std=c++17 $(debug_or_optimize)

SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:%.cpp=%.o)

EXECUTABLE = blackjack
DEBUG = $(EXECUTABLE)_debug

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $*.cpp

debug: EXECUTABLE := $(DEBUG)
debug: debug_or_optimize=-g3
debug: blackjack

blackjack: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(EXECUTABLE)

clean:
	rm -f $(OBJECTS) $(EXECUTABLE) $(DEBUG)

.PHONY: clean, debug
