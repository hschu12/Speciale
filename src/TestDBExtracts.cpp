#include "include/HyperGraph.hpp"
#include <iostream>
#include <fstream>
#include <math.h>

int main(int argc, char const *argv[]){
	if(argc < 6){
		std::cout << "To few arguments. Need graph file, number of highest compound ID, number of highest reactionID, number of max synthplans(K), .txt file with startingCompoundsID" << std::endl;
		exit(1);
	}

	std::vector<int> reactions;
	std::vector<int> compounds;
	std::vector<double> yields;
	std::vector<double> weights;
	std::vector<std::vector<int>> eductsToReaction;
	std::vector<std::vector<int>> productsToReaction;

    compounds.resize(atoi(argv[2]));
    reactions.resize(atoi(argv[3]));
    weights.resize(atoi(argv[2]));
    yields.resize(atoi(argv[3]));
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
    std::cout << "Building" << std::endl;

    bool reactbool = false;
    while(getline(inFile,str)) {
        if (str.compare("REACTIONS") == 0){
            reactbool = true;
        }
        if(!reactbool){
    		//Locate spaces " "
    		int first = str.find_first_of(" ", 0);
	    	int second = str.find_first_of(" ", first+1);
	    	int third = str.find_first_of(" ", second+1);
	    	int fouth = str.find_first_of(" ", third+1);
	    	int fifth = str.find_first_of(" ", fouth+1);

	    	std::string id =  str.substr(0, first); //ID
	    	std::string type = str.substr(first+1, 1); //compound("N") or reaction("R")

	    	if(!type.compare("R")){ //Returns true if type is R
	    		reactions.at(stoi(id)) = stoi(id);
	    		std::string yield = str.substr(third+1, fouth-third-1);
	    		yields.at(stoi(id)) = stod(yield);


	    	}
	    	if(!type.compare("N")){ //Returns true if type is N
	    	  	int idINT = stoi(id);
	    	  	compounds.at(idINT) = idINT;
                std::string weight = str.substr(fouth+1, fifth-fouth-1);
                weights.at(idINT) = stod(weight);
            }
   		}
   		else{
   			int first = str.find_first_of("-", 0);
  			int end = str.find_first_of(" ", 0);
            if(first > 0) { //Removes last line "}"
   				std::string in = str.substr(0, first);
   				std::string out = str.substr(first+2, end-first-2);
             	if(reactions.at(stoi(in)) == stoi(in) && stoi(in) != 0){
 					eductsToReaction.at(stoi(in)).push_back(stod(out));
 				}
 			    if(reactions.at(stoi(out)) == stoi(out)){
 					productsToReaction.at(stoi(out)).push_back(stod(in));
 				}
   			}
   		}
   	}
   	for(int i = 0; i < reactions.size(); i++){
        if(reactions.at(i) != 0){
            h.addReaction(reactions.at(i), productsToReaction.at(i), eductsToReaction.at(i), yields.at(i));
     	}
    }
    compounds.clear();
    reactions.clear();
    yields.clear();
    productsToReaction.clear();
    eductsToReaction.clear();

    compounds.shrink_to_fit();
    reactions.shrink_to_fit();
    yields.shrink_to_fit();
    productsToReaction.shrink_to_fit();
    eductsToReaction.shrink_to_fit();

    std::ifstream inputFile;
    inputFile.open(argv[5]);
    if (!inputFile) {
        std::cout << "Unable to open file";
        exit(1); // terminate with error
    }
    std::string str2;
    std::vector<int> startingCompound;

    getline(inputFile,str2);
    auto goal = h.getCompound(stoi(str2));

    while(getline(inputFile,str2)) {
        auto starting = h.getCompound(stoi(str2));
        starting->molecularWeight = weights.at(stoi(str2));
        startingCompound.push_back(starting->id);
    }

    weights.clear();
    weights.shrink_to_fit();

    h.graphToGraphviz("Output", *goal, startingCompound);

   /* std::cout << "Test for Carstens" << std::endl;
    auto dBest = h.yenHyp(*goal, startingCompound, atoi(argv[4]), false);
    h.printResults(dBest);*/
    
    std::cout << "Test for Nielsens" << std::endl;
    auto dBest = h.yenHyp(*goal, startingCompound, atoi(argv[4]), true);
    h.printResults(dBest);

    inputFile.close();
    inFile.close();
	return 0;
}