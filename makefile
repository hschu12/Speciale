CC=g++
CFLAGS= -std=c++14 -O2

make: src/CreateRandomHyperNoCycles.cpp src/CycleTest.cpp src/DeadEndTest.cpp src/PaperTest.cpp \
	src/ThesisTest.cpp src/ThesisTestWithCycles.cpp
	$(CC) $(CFLAGS) src/CreateRandomHyperNoCycles.cpp -o createRandom
	$(CC) $(CFLAGS) src/CycleTest.cpp -o CycleTest
	$(CC) $(CFLAGS) src/DeadEndTest.cpp -o DeadEndTest
	$(CC) $(CFLAGS) src/PaperTest.cpp -o PaperTest
	$(CC) $(CFLAGS) src/ThesisTest.cpp -o ThesisTest
	$(CC) $(CFLAGS) src/ThesisTestWithCycles.cpp -o ThesisTestWithCycles


clean: 
	rm CreateRandom
	rm CycleTest
	rm DeadEndTest
	rm PaperTest
	rm ThesisTest
	rm ThesisTestWithCycles
	rm *.gv