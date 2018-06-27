CC=g++
CFLAGS= -std=c++14 -O3

make: 
	$(CC) $(CFLAGS) src/CreateRandomHyperNoCycles.cpp -o CreateRandom
	$(CC) $(CFLAGS) src/CycleTest.cpp -o CycleTest
	$(CC) $(CFLAGS) src/DeadEndTest.cpp -o DeadEndTest
	$(CC) $(CFLAGS) src/PaperTest.cpp -o PaperTest
	$(CC) $(CFLAGS) src/ThesisTest.cpp -o ThesisTest
	$(CC) $(CFLAGS) src/ThesisTestWithCycles.cpp -o ThesisTestWithCycles
	$(CC) $(CFLAGS) src/TestHoffmanExtracts.cpp -o HoffmanTests	


clean: 
	rm CreateRandom
	rm CycleTest
	rm DeadEndTest
	rm PaperTest
	rm ThesisTest
	rm ThesisTestWithCycles
	rm HoffmanTests
	rm *.gv
