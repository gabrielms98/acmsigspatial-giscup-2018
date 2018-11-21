all: test2.cpp
	g++ test2.cpp -O3 -std=c++11 -o teste -ljsoncpp
allomp: testeBoost.cpp
	g++ test2omp.cpp -O3 -std=c++11 -o testeomp -fopenmp -ljsoncpp
remove:
	rm *.o *.out *.dot *.png
exec: teste
	time ./teste "SampleDataset1/SampleDataset1.json" "SampleDataset1/startingpoints.txt"
	dot -Tpng SampleDataset1.dot > nomeQualquer.png
