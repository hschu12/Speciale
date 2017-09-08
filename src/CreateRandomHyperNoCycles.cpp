#include "include/HyperGraph.hpp"
#include <iostream>
#include <cstdlib>

int main(int argc, char const *argv[])
{

	HyperGraph h;
    srand(time(0));
  
    if( argc < 3 ){
        std::cout << "To few arguments. Must have 2 arguments: Number of Compounds and Number of Reactions" << std::endl;
        exit(1);
    }
    if (argc > 3) {
        std::cout << "To many arguments. Must have 2 arguments: Number of Compounds and Number of Reactions" << std::endl;
        exit(1);
    }

    int numberOfCompounds = atoi(argv[1]);
    int numberOfReactions = atoi(argv[2]);


    for(int r = 1; r <= numberOfReactions; r++) {
        int head1 = 0;
        int tail1 = 0;
        int tail2 = 0;
        do {
            head1 = rand() % numberOfCompounds + 1;
            tail1 = rand() % numberOfCompounds + 1;
            tail2 = rand() % numberOfCompounds + 1;
        } while (head1 == tail1 || head1 == tail2 || head1 < tail1 || head1 < tail2);
        double yield =  ((double) rand() / (RAND_MAX));
        std::vector<int> head;
        std::vector<int> tail;
        head.push_back(head1);
        tail.push_back(tail1);
        tail.push_back(tail2);
        h.addReaction(r, head, tail, yield);
    }

	auto goal = h.getCompound(numberOfCompounds);
	std::vector<int> startingCompound;
	for (int i = 1; i <= numberOfReactions/2; i++) {
        startingCompound.push_back(i);
    }

    std::cout << "Using Dynamic:" << std::endl;

    auto dBest = h.yenHypDynamic(*goal, startingCompound, 4);
    h.printResults(dBest);
  
    std::cout << "Using Nielsen:" << std::endl;

    dBest = h.yenHypNielsen(*goal, startingCompound, 4);
    h.printResults(dBest);

    h.graphToGraphviz();

	return 0;
}