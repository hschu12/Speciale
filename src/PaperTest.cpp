#include "include/HyperGraph.hpp"
#include <iostream>
int main(int argc, char const *argv[])
{
	HyperGraph h (10,10);

    std::vector<int> reacthead1;
        reacthead1.push_back(4);
    std::vector<int> reacttail1;
        reacttail1.push_back(1);
        reacttail1.push_back(2);

    std::vector<int> reacthead2;
        reacthead2.push_back(3);
    std::vector<int> reacttail2;
        reacttail2.push_back(2);
  
    std::vector<int> reacthead3;
        reacthead3.push_back(4);
    std::vector<int> reacttail3;
        reacttail3.push_back(3);
      
    std::vector<int> reacthead4;
        reacthead4.push_back(5);
    std::vector<int> reacttail4;
        reacttail4.push_back(1);
        reacttail4.push_back(4);

    std::vector<int> reacthead5;
        reacthead5.push_back(6);
    std::vector<int> reacttail5;
        reacttail5.push_back(4);
        reacttail5.push_back(3);
 
    std::vector<int> reacthead6;
        reacthead6.push_back(6);
    std::vector<int> reacttail6;
        reacttail6.push_back(5);

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

    h.graphToGraphviz("PaperTest");

    auto dBest = h.yenHypNielsen(*goal, startingCompound, 9);
    h.printResults(dBest);

    dBest = h.yenHypDynamic(*goal, startingCompound, 9);
    h.printResults(dBest);
  

	return 0;
}