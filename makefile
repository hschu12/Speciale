CC=g++-7
CFLAGS= -std=c++14 -O3 -lsqlite3

make: src/CreateRandomHyperNoCycles.cpp src/CycleTest.cpp src/DeadEndTest.cpp src/PaperTest.cpp \
	src/ThesisTest.cpp src/ThesisTestWithCycles.cpp
	$(CC) $(CFLAGS) src/CreateRandomHyperNoCycles.cpp -o CreateRandom
	$(CC) $(CFLAGS) src/CycleTest.cpp -o CycleTest
	$(CC) $(CFLAGS) src/DeadEndTest.cpp -o DeadEndTest
	$(CC) $(CFLAGS) src/PaperTest.cpp -o PaperTest
	$(CC) $(CFLAGS) src/ThesisTest.cpp -o ThesisTest
	$(CC) $(CFLAGS) src/ThesisTestWithCycles.cpp -o ThesisTestWithCycles
	$(CC) $(CFLAGS) src/Beilstein.cpp -o runBeilstein



clean: 
	rm CreateRandom
	rm CycleTest
	rm DeadEndTest
	rm PaperTest
	rm ThesisTest
	rm ThesisTestWithCycles
	rm runBeilstein
	rm *.gv