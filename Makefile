CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -MMD -I .

TARGET = test
SOURCES = main.cpp

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET) *.d
