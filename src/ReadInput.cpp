#include "include/HyperGraph.hpp"
#include <iostream>
#include <fstream>

int main(int argc, char const *argv[])
{
	if(argc < 5){
		std::cout << "To few arguments. Need txt file, number of compounds, number of reactions and number of max synthplans(K)" << std::endl;
		exit(1);
	}
	std::cout << "io " << std::endl;

	std::vector<int> reactions;
	std::vector<int> compounds;
	std::vector<double> yields;
	std::vector<double> weights;
	std::vector<std::vector<int>> eductsToReaction;
	std::vector<std::vector<int>> productsToReaction;
	productsToReaction.resize(atoi(argv[3]));
	eductsToReaction.resize(atoi(argv[3]));

	HyperGraph h(atoi(argv[2]),atoi(argv[3]));



	std::ifstream inFile;
    
    inFile.open(argv[1]);
    if (!inFile) {
        std::cout << "Unable to open file";
        exit(1); // terminate with error
    }
    std::string str;
    getline(inFile,str);
    while(getline(inFile,str)) {
    	if(str.length() >= 14){
    		//Locate spaces " "
    		int first = str.find_first_of(" ", 0);
	    	int second = str.find_first_of(" ", first+1);
	    	int third = str.find_first_of(" ", second+1);
	    	int fouth = str.find_first_of(" ", third+1);
	    	int fifth = str.find_first_of(" ", fouth+1);

	    	std::string id =  str.substr(0, first); //ID
	    	std::string type = str.substr(first+1, 1); //compound("N") or reaction("R")

	    	if(!type.compare("R")){ //Returns true if type is R
	    		reactions.push_back(stoi(id));
	    		std::string yield = str.substr(third+1, fouth-third-1);
	    		yields.push_back(stod(yield));


	    	}
	    	if(!type.compare("N")){ //Returns true if type is R
	    		int idINT = stoi(id);
	    		compounds.push_back(idINT);
	    		std::string weight = str.substr(fouth+1, fifth-fouth-1);
	    		weights.push_back(stod(weight));
	    	}
   		}
   		else{
   			int first = str.find_first_of("-", 0);
  			int end = str.find_first_of(" ", 0);
 			if(first > 0) { //Removes last line "}"
   				std::string in = str.substr(0, first);
   				std::string out = str.substr(first+2, end-first-2);
   				for(int i = 0; i < reactions.size(); i++) {
   					if(reactions.at(i) == stoi(in)){
   						eductsToReaction.at(i).push_back(stod(out)+1);
   					}
   					if(reactions.at(i) == stoi(out)){
   						productsToReaction.at(i).push_back(stod(in)+1);
   					}
   				}
   			}
   		}
   	}
   	for(int i = 0; i < reactions.size(); i++) {
   		h.addReaction(reactions.at(i), productsToReaction.at(i), eductsToReaction.at(i), yields.at(i));
   	}

   	h.graphToGraphviz("Output");

   	auto goal = h.getCompound(1);
   	auto starting = h.getCompound(compounds.at(compounds.size()-1)+1);

   	starting->molecularWeight = weights.at(weights.size()-1);
   	std::vector<int> startingCompound;
   	startingCompound.push_back(starting->id);

   	auto dBest = h.yenHyp(*goal, startingCompound, atoi(argv[4]), true);
    h.printResults(dBest);

    dBest = h.yenHyp(*goal, startingCompound, atoi(argv[4]), false);
    h.printResults(dBest);
    inFile.close();
	return 0;
}