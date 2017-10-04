#include "include/HyperGraph.hpp"
#include <iostream>
int main(int argc, char const *argv[])
{
	HyperGraph h (10,10);

    std::vector<int> reacthead1;
        reacthead1.push_back(2);
    std::vector<int> reacttail1;
        reacttail1.push_back(1);
        reacttail1.push_back(4);
    
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
        reacttail4.push_back(4);
      
    std::vector<int> reacthead5;
        reacthead5.push_back(2);
    std::vector<int> reacttail5;
        reacttail5.push_back(1);
        reacttail5.push_back(6);
  
  	h.addReaction(1, reacthead1, reacttail1, 0.4);
  	h.addReaction(2, reacthead2, reacttail2, 0.5);
  	h.addReaction(3, reacthead3, reacttail3, 0.7);
    h.addReaction(4, reacthead4, reacttail4, 0.7);
    h.addReaction(5, reacthead5, reacttail5, 0.7);

    auto goal = h.getCompound(5);
    std::vector<int> startingCompound;
    startingCompound.push_back(1);
    startingCompound.push_back(6);

    h.graphToGraphviz("CycleTest");

    auto dBest = h.yenHypNielsen(*goal, startingCompound, 9);
    h.printResults(dBest);

    dBest = h.yenHypDynamic(*goal, startingCompound, 9);
    h.printResults(dBest);

 
	return 0;
}