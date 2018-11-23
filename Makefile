test: test2.cpp
	g++ test2.cpp -O3 -std=c++11 -o teste.out -ljsoncpp
random: random.cpp
	g++ random.cpp -O3 -std=c++11 -o random.out
dottest:
	dot -Tpng sampleDataset1.dot -o sampleDataset1.png
dotrandom:
	dot -Tpng randomG.dot -o randomG.png
remove:
	rm *.o *.out *.dot *.png
