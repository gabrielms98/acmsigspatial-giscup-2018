main: main.cpp
	g++ main.cpp -O3 -std=c++11 -fopenmp -o main

ordena: ordena.cpp
	g++ ordena.cpp -std=c++11 -o ord

remove:
	rm *.txt *.out
