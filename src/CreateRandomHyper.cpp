#include "include/HyperGraph.hpp"
#include <iostream>
#include <cstdlib>

int main(int argc, char const *argv[])
{

	HyperGraph h;
  srand(time(0));

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
    int yield = rand() % 2 + 1;
    std::vector<int> head;
    std::vector<int> tail;
    head.push_back(head1);
    tail.push_back(tail1);
    tail.push_back(tail2);
    h.addReaction(r, head, tail, yield);
  }

  h.printGraphConsole();
  h.graphToGraphviz();

  std::vector<int> toRemove;
  auto overlayFullGraph = h.createOverlay(toRemove);

	auto goal = h.getCompound(numberOfCompounds);
	std::vector<int> startingCompound;
	for (int i = 1; i <= numberOfReactions; i++)
  {
    startingCompound.push_back(i);
  }



  auto dBest = h.yenHyp(*goal, startingCompound, overlayFullGraph, 9);
  h.printResults(dBest);

	return 0;
}