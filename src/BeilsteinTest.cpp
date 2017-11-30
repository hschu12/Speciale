#include "include/databaseConnection.hpp"
#include <iostream>
#include <chrono>

int main(int argc, char const *argv[])
{
	HyperGraph h = createGraph(6000000, 1000000);

	std::vector<int> goalCompounds;
	goalCompounds.push_back(4989);

    auto start = h.getCompound(112069);
    auto start2 = h.getCompound(112067);
    auto start3 = h.getCompound(112072);
    auto start4 = h.getCompound(3598088);

    start->molecularWeight = 2;
    start2->molecularWeight = 2;
    start3->molecularWeight = 2;
    start4->molecularWeight = 2;

	std::vector<int> startingCompound;
	startingCompound.push_back(112069);
	startingCompound.push_back(112067);
	startingCompound.push_back(112072);
	startingCompound.push_back(3598088);

	h.graphToGraphviz("MergedDump", goalCompounds, startingCompound);

  	auto starttime = std::chrono::high_resolution_clock::now();

	auto dBest = h.yenHyp(goalCompounds, startingCompound, 9, true);
    h.printResults(dBest);
	
	auto end = std::chrono::high_resolution_clock::now();
	auto dur = end - starttime;
	auto i_millis = std::chrono::duration_cast<std::chrono::milliseconds>(dur);
	std::cout << i_millis.count() << "milliseconds" << '\n';
    
   	starttime = std::chrono::high_resolution_clock::now();

    dBest = h.yenHyp(goalCompounds, startingCompound, 9, false);
    h.printResults(dBest);

	end = std::chrono::high_resolution_clock::now();
	dur = end - starttime;
	i_millis = std::chrono::duration_cast<std::chrono::milliseconds>(dur);
	std::cout << i_millis.count() << "milliseconds" << '\n';

	return 0;
}