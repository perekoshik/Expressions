CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
TARGET = test_expr
SOURCES = main.cpp

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)
