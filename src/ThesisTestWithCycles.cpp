#include "include/HyperGraphWithCycles.hpp"
#include <iostream>
int main(int argc, char const *argv[])
{
	HyperGraph h;

	int head1[] = {3};
  	std::vector<int> reacthead1 (head1, head1 + sizeof(head1) / sizeof(int) );
	int tail1[] = {1,2};
  	std::vector<int> reacttail1 (tail1, tail1 + sizeof(tail1) / sizeof(int) );

	int head2[] = {4};
  	std::vector<int> reacthead2 (head2, head2 + sizeof(head2) / sizeof(int) );
	int tail2[] = {2};
  	std::vector<int> reacttail2 (tail2, tail2 + sizeof(tail2) / sizeof(int) );

  	int head3[] = {5};
  	std::vector<int> reacthead3 (head3, head3 + sizeof(head3) / sizeof(int) );
	int tail3[] = {1};
  	std::vector<int> reacttail3 (tail3, tail3 + sizeof(tail3) / sizeof(int) );

	int head4[] = {6};
  	std::vector<int> reacthead4 (head4, head4 + sizeof(head4) / sizeof(int) );
	int tail4[] = {3};
  	std::vector<int> reacttail4 (tail4, tail4 + sizeof(tail4) / sizeof(int) );

  	int head5[] = {7};
  	std::vector<int> reacthead5 (head5, head5 + sizeof(head5) / sizeof(int) );
	int tail5[] = {3};
  	std::vector<int> reacttail5 (tail5, tail5 + sizeof(tail5) / sizeof(int) );

	int head6[] = {6};
  	std::vector<int> reacthead6 (head6, head6 + sizeof(head6) / sizeof(int) );
	int tail6[] = {1,4};
  	std::vector<int> reacttail6 (tail6, tail6 + sizeof(tail6) / sizeof(int) );

  	int head7[] = {8};
  	std::vector<int> reacthead7 (head7, head7 + sizeof(head7) / sizeof(int) );
	int tail7[] = {4,5};
  	std::vector<int> reacttail7 (tail7, tail7 + sizeof(tail7) / sizeof(int) );

	int head8[] = {7};
  	std::vector<int> reacthead8 (head8, head8 + sizeof(head8) / sizeof(int) );
	int tail8[] = {2,5};
  	std::vector<int> reacttail8 (tail8, tail8 + sizeof(tail8) / sizeof(int) );

  	int head9[] = {8};
  	std::vector<int> reacthead9 (head9, head9 + sizeof(head9) / sizeof(int) );
	int tail9[] = {6};
  	std::vector<int> reacttail9 (tail9, tail9 + sizeof(tail9) / sizeof(int) );

	int head10[] = {8};
  	std::vector<int> reacthead10 (head10, head10 + sizeof(head10) / sizeof(int) );
	int tail10[] = {7};
  	std::vector<int> reacttail10 (tail10, tail10 + sizeof(tail10) / sizeof(int) );

  	h.addReaction(1, reacthead1, reacttail1, 0.7);
  	h.addReaction(2, reacthead2, reacttail2, 0.5);
  	h.addReaction(3, reacthead3, reacttail3, 0.5);
  	h.addReaction(4, reacthead4, reacttail4, 0.7);
  	h.addReaction(5, reacthead5, reacttail5, 0.7);
  	h.addReaction(6, reacthead6, reacttail6, 0.7);
  	h.addReaction(7, reacthead7, reacttail7, 0.7);
  	h.addReaction(8, reacthead8, reacttail8, 0.7);
  	h.addReaction(9, reacthead9, reacttail9, 0.7);
  	h.addReaction(10, reacthead10, reacttail10,0.7);

  std::vector<int> toRemove;



  auto overlayFullGraph = h.createOverlay(toRemove);

	auto goal = h.getCompound(8);
	std::vector<int> startingCompound;
	startingCompound.push_back(1);
	startingCompound.push_back(2);

  auto dBest = h.ShortestHyperNielsen(startingCompound);
  auto path = h.getShortestPathYield(8);
  std::cout << "path" << std::endl;
  for (auto element : path) {
    std::cout << element << std::endl;
  }
  goal = h.getCompound(8);
  std::cout << goal -> weight << std::endl;
  h.graphToGraphviz();
 
	return 0;
}