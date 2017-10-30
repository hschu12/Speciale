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
      
    std::vector<int> reacthead4;//11
        reacthead4.push_back(1);
    std::vector<int> reacttail4;
        reacttail4.push_back(10);
        reacttail4.push_back(11);

    std::vector<int> reacthead5;//12
        reacthead5.push_back(3);
    std::vector<int> reacttail5;
        reacttail5.push_back(2);
        reacttail5.push_back(6);
  
    std::vector<int> reacthead6;//13
        reacthead6.push_back(3);
    std::vector<int> reacttail6;
        reacttail6.push_back(8);
        reacttail6.push_back(11);

    std::vector<int> reacthead7;//15
        reacthead7.push_back(3);
    std::vector<int> reacttail7;
        reacttail7.push_back(5);
        reacttail7.push_back(15);

    std::vector<int> reacthead8;//17
        reacthead8.push_back(3);
    std::vector<int> reacttail8;
        reacttail8.push_back(10);
        reacttail8.push_back(17);

    std::vector<int> reacthead9;//18
        reacthead9.push_back(15);
    std::vector<int> reacttail9;
        reacttail9.push_back(11);
        reacttail9.push_back(17);

    std::vector<int> reacthead10;//19
        reacthead10.push_back(11);
    std::vector<int> reacttail10;
        reacttail10.push_back(2);
        reacttail10.push_back(17);
  
    std::vector<int> reacthead11;//20
        reacthead11.push_back(10);
    std::vector<int> reacttail11;
        reacttail11.push_back(2);
        reacttail11.push_back(8);

    std::vector<int> reacthead12;//21
        reacthead12.push_back(10);
    std::vector<int> reacttail12;
        reacttail12.push_back(5);
        reacttail12.push_back(11);

    std::vector<int> reacthead13;//22
        reacthead13.push_back(8);
    std::vector<int> reacttail13;
        reacttail13.push_back(2);
        reacttail13.push_back(11);
  
    std::vector<int> reacthead14;//23
        reacthead14.push_back(8);
    std::vector<int> reacttail14;
        reacttail14.push_back(5);
        reacttail14.push_back(17);

    std::vector<int> reacthead15;//24
        reacthead15.push_back(6);
    std::vector<int> reacttail15;
        reacttail15.push_back(8);
        reacttail15.push_back(17);
  
    std::vector<int> reacthead16;//25
        reacthead16.push_back(6);
    std::vector<int> reacttail16;
        reacttail16.push_back(11);
        reacttail16.push_back(11);

    std::vector<int> reacthead17;//26
        reacthead17.push_back(6);
    std::vector<int> reacttail17;
        reacttail17.push_back(2);
        reacttail17.push_back(15);

    std::vector<int> reacthead18;//27
        reacthead18.push_back(5);
    std::vector<int> reacttail18;
        reacttail18.push_back(2);
        reacttail18.push_back(2);


    h.addReaction(3, reacthead1, reacttail1, 0.8);
    h.addReaction(6, reacthead2, reacttail2, 0.8);
    h.addReaction(8, reacthead3, reacttail3, 0.8);
    h.addReaction(11, reacthead4, reacttail4, 0.8);
    h.addReaction(12, reacthead5, reacttail5, 0.8);
    h.addReaction(13, reacthead6, reacttail6, 0.8);
    h.addReaction(15, reacthead7, reacttail7, 0.8);
    h.addReaction(17, reacthead8, reacttail8, 0.8);
    h.addReaction(18, reacthead9, reacttail9, 0.8);
    h.addReaction(19, reacthead10, reacttail10, 0.8);
    h.addReaction(20, reacthead11, reacttail11, 0.8);
    h.addReaction(21, reacthead12, reacttail12, 0.8);
    h.addReaction(22, reacthead13, reacttail13, 0.8);
    h.addReaction(23, reacthead14, reacttail14, 0.8);
    h.addReaction(24, reacthead15, reacttail15, 0.8);
    h.addReaction(25, reacthead16, reacttail16, 0.8);
    h.addReaction(26, reacthead17, reacttail17, 0.8);
    h.addReaction(27, reacthead18, reacttail18, 0.8);


	auto goal = h.getCompound(1);
    auto start1 = h.getCompound(2);
    auto start2 = h.getCompound(17);


    start1->molecularWeight = 1;
    start2->molecularWeight = 4;


	std::vector<int> startingCompound;
	startingCompound.push_back(2);
    startingCompound.push_back(17);



    h.graphToGraphviz("RojinTest2");

    auto dBest = h.yenHyp(*goal, startingCompound, 20, true);
    h.printResults(dBest);

     dBest = h.yenHyp(*goal, startingCompound, 20, false);
    h.printResults(dBest);

	return 0;
}