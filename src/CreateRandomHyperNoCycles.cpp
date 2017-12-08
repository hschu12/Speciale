#include "include/HyperGraph.hpp"
#include <iostream>
#include <cstdlib>

int main(int argc, char const *argv[])
{

	HyperGraph h (atoi(argv[1]), atoi(argv[2]), atoi(argv[1]), atoi(argv[2]));
    srand(time(0));
  
    if( argc < 4 ){
        std::cout << "To few arguments. Must have 3 arguments: Number of Compounds, Number of Reactions and K" << std::endl;
        exit(1);
    }
    if (argc > 4) {
        std::cout << "To many arguments. Must have 3 arguments: Number of Compounds, Number of Reactions and K" << std::endl;
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

    std::vector<int> goalCompounds;
	goalCompounds.push_back(numberOfCompounds);
	std::vector<int> startingCompound;
	for (int i = 1; i <= numberOfReactions/2; i++) {
        startingCompound.push_back(i);
        auto com = h.getCompound(i);
        com->molecularWeight = 2;
    }

    auto dBest = h.yenHyp(goalCompounds, startingCompound, atoi(argv[3]), true);
    h.printResults(dBest);

    dBest = h.yenHyp(goalCompounds, startingCompound, atoi(argv[3]), false);
    h.printResults(dBest);

    h.graphToGraphviz("Random", goalCompounds, startingCompound);

	return 0;
}