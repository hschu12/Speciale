#include "include/HyperGraph.hpp"
#include <iostream>
int main(int argc, char const *argv[])
{
	HyperGraph h (10, 10);

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
      


    h.addReaction(3, reacthead1, reacttail1, 0.8);
    h.addReaction(6, reacthead2, reacttail2, 0.8);
    h.addReaction(8, reacthead3, reacttail3, 0.8);


	auto goal = h.getCompound(1);
    auto start1 = h.getCompound(2);
    auto start2 = h.getCompound(3);
    auto start3 = h.getCompound(5);
    auto start4 = h.getCompound(6);
    auto start5 = h.getCompound(8);

    start1->molecularWeight = 5;
    start2->molecularWeight = 1;
    start3->molecularWeight = 4;
    start4->molecularWeight = 2;
    start5->molecularWeight = 3;


	std::vector<int> startingCompound;
	startingCompound.push_back(2);
    startingCompound.push_back(3);
    startingCompound.push_back(5);
    startingCompound.push_back(6);
    startingCompound.push_back(8);


    h.graphToGraphviz("RojinTest1");

    auto dBest = h.yenHyp(*goal, startingCompound, 9, true);
    h.printResults(dBest);

     dBest = h.yenHyp(*goal, startingCompound, 9, false);
    h.printResults(dBest);

	return 0;
}