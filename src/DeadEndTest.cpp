#include "include/HyperGraphWithCycles.hpp"
#include <iostream>
int main(int argc, char const *argv[])
{
	HyperGraph h;

	int head1[] = {6};
  	std::vector<int> reacthead1 (head1, head1 + sizeof(head1) / sizeof(int) );
	int tail1[] = {1,2};
  	std::vector<int> reacttail1 (tail1, tail1 + sizeof(tail1) / sizeof(int) );

	int head2[] = {7};
  	std::vector<int> reacthead2 (head2, head2 + sizeof(head2) / sizeof(int) );
	int tail2[] = {2,3};
  	std::vector<int> reacttail2 (tail2, tail2 + sizeof(tail2) / sizeof(int) );

  int head3[] = {8};
  	std::vector<int> reacthead3 (head3, head3 + sizeof(head3) / sizeof(int) );
	int tail3[] = {3,4};
  	std::vector<int> reacttail3 (tail3, tail3 + sizeof(tail3) / sizeof(int) );

	int head4[] = {5};
  	std::vector<int> reacthead4 (head4, head4 + sizeof(head4) / sizeof(int) );
	int tail4[] = {1};
  	std::vector<int> reacttail4 (tail4, tail4 + sizeof(tail4) / sizeof(int) );

  	int head5[] = {2};
  	std::vector<int> reacthead5 (head5, head5 + sizeof(head5) / sizeof(int) );
	int tail5[] = {6,7};
  	std::vector<int> reacttail5 (tail5, tail5 + sizeof(tail5) / sizeof(int) );

	int head6[] = {9};
  	std::vector<int> reacthead6 (head6, head6 + sizeof(head6) / sizeof(int) );
	int tail6[] = {5,7};
  	std::vector<int> reacttail6 (tail6, tail6 + sizeof(tail6) / sizeof(int) );

  	int head7[] = {10};
  	std::vector<int> reacthead7 (head7, head7 + sizeof(head7) / sizeof(int) );
	int tail7[] = {6,8};
  	std::vector<int> reacttail7 (tail7, tail7 + sizeof(tail7) / sizeof(int) );

	int head8[] = {10};
  	std::vector<int> reacthead8 (head8, head8 + sizeof(head8) / sizeof(int) );
	int tail8[] = {8,4};
  	std::vector<int> reacttail8 (tail8, tail8 + sizeof(tail8) / sizeof(int) );

  	h.addReaction(1, reacthead1, reacttail1, 0.7);
  	h.addReaction(2, reacthead2, reacttail2, 0.5);
  	h.addReaction(3, reacthead3, reacttail3, 0.9);
  	h.addReaction(4, reacthead4, reacttail4, 0.8);
  	h.addReaction(5, reacthead5, reacttail5, 0.9);
  	h.addReaction(6, reacthead6, reacttail6, 0.8);
  	h.addReaction(7, reacthead7, reacttail7, 0.7);
  	h.addReaction(8, reacthead8, reacttail8, 0.6);


	auto goal = h.getCompound(10);
	std::vector<int> startingCompound;
	startingCompound.push_back(1);
	startingCompound.push_back(2);
  startingCompound.push_back(3);
  startingCompound.push_back(4);


  auto dBest = h.yenHyp(*goal, startingCompound, 10);

  h.printResults(dBest);

  h.graphToGraphviz();
 
	return 0;
}