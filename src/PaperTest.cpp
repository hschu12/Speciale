#include "include/HyperGraph.hpp"
#include <iostream>
int main(int argc, char const *argv[])
{
	HyperGraph h;

	int head1[] = {4};
  std::vector<int> reacthead1 (head1, head1 + sizeof(head1) / sizeof(int) );
	int tail1[] = {1,2};
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
	int tail4[] = {1,4};
  std::vector<int> reacttail4 (tail4, tail4 + sizeof(tail4) / sizeof(int) );

  int head5[] = {6};
  std::vector<int> reacthead5 (head5, head5 + sizeof(head5) / sizeof(int) );
	int tail5[] = {4,3};
  std::vector<int> reacttail5 (tail5, tail5 + sizeof(tail5) / sizeof(int) );

	int head6[] = {6};
  std::vector<int> reacthead6 (head6, head6 + sizeof(head6) / sizeof(int) );
	int tail6[] = {5};
  std::vector<int> reacttail6 (tail6, tail6 + sizeof(tail6) / sizeof(int) );


  

  h.addReaction(1, reacthead1, reacttail1, 2);
  h.addReaction(2, reacthead2, reacttail2, 1);
  h.addReaction(3, reacthead3, reacttail3, 1);
  h.addReaction(4, reacthead4, reacttail4, 2);
  h.addReaction(5, reacthead5, reacttail5, 1);
  h.addReaction(6, reacthead6, reacttail6, 2);

	auto goal = h.getCompound(6);
	std::vector<int> startingCompound;
	startingCompound.push_back(1);
	startingCompound.push_back(2);

  auto dBest = h.yenHypNielsen(*goal, startingCompound, 9);
  h.printResults(dBest);

  dBest = h.yenHypDynamic(*goal, startingCompound, 9);
  h.printResults(dBest);
  
  h.graphToGraphviz();

	return 0;
}