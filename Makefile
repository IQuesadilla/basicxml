CXX = g++
CFLAGS = -std=c++17 -fPIC -Wall
TESTFLAGS = -o $@ $^
OBJFLAGS = -o $@ -c $<
TARGET = basicxml.o
TESTS = tests/bin/test1

all: $(TARGET)

tests: $(TESTS)

tests/bin/test1: tests/test1.cpp basicxml.o
	$(CXX) $(TESTFLAGS) $(CFLAGS)

basicxml.o: basicxml.cpp basicxml.h
	$(CXX) $(OBJFLAGS) $(CFLAGS)

clean:
	-rm *.o
	-rm tests/bin/*