#include "include/HyperGraph.hpp"
#include <iostream>
int main(int argc, char const *argv[])
{
	HyperGraph h (20, 20);

    std::vector<int> reacthead1;//3
        reacthead1.push_back(1);
    std::vector<int> reacttail1;
        reacttail1.push_back(2);
        reacttail1.push_back(3);

    std::vector<int> reacthead2;//6
        reacthead2.push_back(1);
    std::vector<int> reacttail2;
        reacttail2.push_back(5);
        reacttail2.push_back(6);
  
    std::vector<int> reacthead3;//8
        reacthead3.push_back(1);
    std::vector<int> reacttail3;
        reacttail3.push_back(8);
        reacttail3.push_back(8);
      
    std::vector<int> reacthead4;//9
        reacthead4.push_back(3);
    std::vector<int> reacttail4;
        reacttail4.push_back(6);
        reacttail4.push_back(6);

    std::vector<int> reacthead5;//10
        reacthead5.push_back(2);
    std::vector<int> reacttail5;
        reacttail5.push_back(6);
        reacttail5.push_back(8);
  
    std::vector<int> reacthead6;//11
        reacthead6.push_back(2);
    std::vector<int> reacttail6;
        reacttail6.push_back(3);
        reacttail6.push_back(3);

    std::vector<int> reacthead7;//12
        reacthead7.push_back(8);
    std::vector<int> reacttail7;
        reacttail7.push_back(3);
        reacttail7.push_back(6);

    std::vector<int> reacthead8;//13
        reacthead8.push_back(5);
    std::vector<int> reacttail8;
        reacttail8.push_back(2);
        reacttail8.push_back(6);

    std::vector<int> reacthead9;//14
        reacthead9.push_back(5);
    std::vector<int> reacttail9;
        reacttail9.push_back(3);
        reacttail9.push_back(8);

    std::vector<int> reacthead10;//16
        reacthead10.push_back(2);
    std::vector<int> reacttail10;
        reacttail10.push_back(16);
  
    std::vector<int> reacthead11;//17
        reacthead11.push_back(3);
    std::vector<int> reacttail11;
        reacttail11.push_back(16);

    std::vector<int> reacthead12;//18
        reacthead12.push_back(6);
    std::vector<int> reacttail12;
        reacttail12.push_back(16);

    std::vector<int> reacthead13;//19
        reacthead13.push_back(8);
    std::vector<int> reacttail13;
        reacttail13.push_back(16);
  

    h.addReaction(3, reacthead1, reacttail1, 0.8);
    h.addReaction(6, reacthead2, reacttail2, 0.8);
    h.addReaction(8, reacthead3, reacttail3, 0.8);
    h.addReaction(9, reacthead4, reacttail4, 0.8);
    h.addReaction(10, reacthead5, reacttail5, 0.8);
    h.addReaction(11, reacthead6, reacttail6, 0.8);
    h.addReaction(12, reacthead7, reacttail7, 0.8);
    h.addReaction(13, reacthead8, reacttail8, 0.8);
    h.addReaction(14, reacthead9, reacttail9, 0.8);
    h.addReaction(16, reacthead10, reacttail10, 1.5); //Changed from 0.25
    h.addReaction(17, reacthead11, reacttail11, 3); //Changed from 0.5
    h.addReaction(18, reacthead12, reacttail12, 6); //changed from 1
    h.addReaction(19, reacthead13, reacttail13, 2); //Changed from 0.333 


	auto goal = h.getCompound(1);
    auto start1 = h.getCompound(16);


    start1->molecularWeight = 6;

	std::vector<int> startingCompound;
	startingCompound.push_back(16);



    h.graphToGraphviz("RojinTest3");

    auto dBest = h.yenHyp(*goal, startingCompound, 25, true);
    h.printResults(dBest);

     dBest = h.yenHyp(*goal, startingCompound, 25, false);
    h.printResults(dBest);

	return 0;
}