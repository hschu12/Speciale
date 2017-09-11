#include "include/HyperGraph.hpp"
#include <iostream>
int main(int argc, char const *argv[])
{
	HyperGraph h;

    std::vector<int> reacthead1;
        reacthead1.push_back(6);
    std::vector<int> reacttail1;
        reacttail1.push_back(1);
        reacttail1.push_back(2);

    std::vector<int> reacthead2;
        reacthead2.push_back(7);
    std::vector<int> reacttail2;
        reacttail2.push_back(2);
        reacttail2.push_back(3);
  
    std::vector<int> reacthead3;
        reacthead3.push_back(8);
    std::vector<int> reacttail3;
        reacttail3.push_back(3);
        reacttail3.push_back(4);
      
    std::vector<int> reacthead4;
        reacthead4.push_back(5);
    std::vector<int> reacttail4;
        reacttail4.push_back(1);
      
    std::vector<int> reacthead5;
        reacthead5.push_back(2);
    std::vector<int> reacttail5;
        reacttail5.push_back(6);
        reacttail5.push_back(7);
    
    std::vector<int> reacthead6;
        reacthead6.push_back(9);
    std::vector<int> reacttail6;
        reacttail6.push_back(5);
        reacttail6.push_back(7);
     
    std::vector<int> reacthead7;
        reacthead7.push_back(10);
    std::vector<int> reacttail7;
        reacttail7.push_back(6);
        reacttail7.push_back(8);

    std::vector<int> reacthead8;
        reacthead8.push_back(10);
    std::vector<int> reacttail8;
        reacttail8.push_back(8);
        reacttail8.push_back(4);

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


    auto dBest = h.yenHypNielsen(*goal, startingCompound, 10);
    h.printResults(dBest);
    h.graphToGraphviz();

    dBest = h.yenHypDynamic(*goal, startingCompound, 10);
    h.printResults(dBest);

 
	return 0;
}