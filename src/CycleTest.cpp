#include "include/HyperGraphWithCycles.hpp"
#include <iostream>
int main(int argc, char const *argv[])
{
	HyperGraph h;

	int head1[] = {2};
  	std::vector<int> reacthead1 (head1, head1 + sizeof(head1) / sizeof(int) );
	int tail1[] = {1,4};
  	std::vector<int> reacttail1 (tail1, tail1 + sizeof(tail1) / sizeof(int) );

	int head2[] = {3};
  	std::vector<int> reacthead2 (head2, head2 + sizeof(head2) / sizeof(int) );
	int tail2[] = {2};
  	std::vector<int> reacttail2 (tail2, tail2 + sizeof(tail2) / sizeof(int) );

 	int head3[] = {4};
  	std::vector<int> reacthead3 (head3, head3 + sizeof(head3) / sizeof(int) );
	int tail3[] = {3};
  	std::vector<int> reacttail3 (tail3, tail3 + sizeof(tail3) / sizeof(int) );
  
  int head4[] = {5};
    std::vector<int> reacthead4 (head4, head4 + sizeof(head4) / sizeof(int) );
  int tail4[] = {4};
    std::vector<int> reacttail4 (tail4, tail4 + sizeof(tail4) / sizeof(int) );

  int head5[] = {2};
    std::vector<int> reacthead5 (head5, head5 + sizeof(head5) / sizeof(int) );
  int tail5[] = {1,6};
    std::vector<int> reacttail5 (tail5, tail5 + sizeof(tail5) / sizeof(int) );

  	h.addReaction(1, reacthead1, reacttail1, 0.4);
  	h.addReaction(2, reacthead2, reacttail2, 0.5);
  	h.addReaction(3, reacthead3, reacttail3, 0.7);
    h.addReaction(4, reacthead4, reacttail4, 0.7);
    h.addReaction(5, reacthead5, reacttail5, 0.7);

  	
  std::vector<int> toRemove;



  
  /*auto overlayFullGraph = h.createOverlay(toRemove);

  auto goal = h.getCompound(5);
  std::vector<int> startingCompound;
  startingCompound.push_back(1);
  startingCompound.push_back(6);

  auto dBest = h.yenHyp(*goal, startingCompound, overlayFullGraph, 9);
  h.printResults(dBest);*/


  auto overlayFullGraph = h.createOverlay(toRemove);

  auto goal = h.getCompound(5);
  std::vector<int> startingCompound;
  startingCompound.push_back(1);
  startingCompound.push_back(6);

  auto dBest = h.ShortestHyperNielsen(startingCompound);
  auto path = h.getShortestPathYield(5);
  std::cout << "path" << std::endl;
  for (auto element : path) {
    std::cout << element << std::endl;
  }
  std::cout << goal -> weight << std::endl;
  h.graphToGraphviz();
 
	return 0;
}