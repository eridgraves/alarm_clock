# 12/2/17
# Eric Graves
# Makefile for syl_test.cpp (includes syllable.cpp)

output: source.o
	g++ -std=c++11 -Wall source.o -o alarm # Use c++11

cource.o: source.cpp
	g++ -c source.cpp

clean:
	rm *.o alarm
