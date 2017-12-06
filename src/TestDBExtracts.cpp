#include "include/HyperGraph.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <math.h>

int main(int argc, char const *argv[]){
	if(argc < 4){
		std::cout << "To few arguments. Need graph file, number of max synthplans(K), .txt file with startingCompoundsID" << std::endl;
		exit(1);
	}

    std::ifstream firstRead;
    
    firstRead.open(argv[1]);
    if (!firstRead) {
        std::cout << "Unable to open file";
        exit(1); // terminate with error
    }
    std::string line;
    getline(firstRead,line);

    int largestReactionID = 0;
    int largestCompoundID = 0;
    while(getline(firstRead,line)) {

        if (line.compare("REACTIONS") == 0){
            break;
        }
        //Locate spaces " "
        int first = line.find_first_of(" ", 0);

        std::string id =  line.substr(0, first); //ID
        std::string type = line.substr(first+1, 1); //compound("N") or reaction("R")

        if(!type.compare("R")){ //Returns true if type is R
            if (largestReactionID < stoi(id)) {
                largestReactionID = stoi(id);
            }
        }
        if(!type.compare("N")){ //Returns true if type is N
            if (largestCompoundID < stoi(id)) {
                largestCompoundID = stoi(id);
            }          
        }
    }

    largestCompoundID++;
    largestReactionID++;
    std::vector<int> reactions;
    std::vector<int> compounds;
    std::vector<double> yields;
    std::vector<double> weights;
    std::vector<std::vector<int>> eductsToReaction;
    std::vector<std::vector<int>> productsToReaction;

    compounds.resize(largestCompoundID);
    reactions.resize(largestReactionID);
    weights.resize(largestCompoundID);
    yields.resize(largestReactionID);
    productsToReaction.resize(largestReactionID);
    eductsToReaction.resize(largestReactionID);

    HyperGraph h(largestCompoundID+100,largestReactionID+100);

    firstRead.close();
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
	    		reactions[stoi(id)] = stoi(id);
	    		std::string yield = str.substr(third+1, fouth-third-1);
                if(yield.compare("None") == 0) {
                    std::cout << "Yield: 'None' found. Reaction " << id << " is given yield: ";
                    yields[stoi(id)] = 0.1;
                    std::cout << yields[stoi(id)] <<std::endl;
                }
                else{
                    double yieldFromExtract = stod(yield);
    	    		yields[stoi(id)] = yieldFromExtract/100;
                }

	    	}
	    	if(!type.compare("N")){ //Returns true if type is N
	    	  	int idINT = stoi(id);
	    	  	compounds[idINT] = idINT;
                std::string weight = str.substr(fouth+1, fifth-fouth-1);
                weights[idINT] = stod(weight);
            }
   		}
   		else{
   			int first = str.find_first_of("-", 0);
  			int end = str.find_first_of(" ", 0);
            if(first > 0) { //Removes last line "}"
   				std::string in = str.substr(0, first);
   				std::string out = str.substr(first+2, end-first-2);
                if(in.compare("None") != 0 && out.compare("None") != 0) {
                 	if(reactions[stoi(in)] == stoi(in) && stoi(in) != 0){
                        //Checks to see if the educt is already added
                        if (!(std::find(eductsToReaction[stoi(in)].begin(), eductsToReaction[stoi(in)].end(), stod(out)) != eductsToReaction[stoi(in)].end())) {
                            eductsToReaction[stoi(in)].push_back(stod(out));
                        }
     				}
     			    if(reactions[stoi(out)] == stoi(out)){
                        //Checks to see if the product is already added
                        if (!(std::find(productsToReaction[stoi(out)].begin(), productsToReaction[stoi(out)].end(), stod(in)) != productsToReaction[stoi(out)].end())) {
                            productsToReaction[stoi(out)].push_back(stod(in));
                        }
     				}
                }
   			}
   		}
   	}
   	for(int i = 0; i < reactions.size(); i++){
        if(reactions[i] != 0){
            h.addReaction(reactions[i], productsToReaction[i], eductsToReaction[i], yields[i]);
     	}
    }
    std::cout << "Done Build" << std::endl;
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

    inputFile.open(argv[3]);
    if (!inputFile) {
        std::cout << "Unable to open file";
        exit(1); // terminate with error
    }
    std::string str2;
    std::vector<int> startingCompound;
    std::vector<int> goalCompound;

    while(getline(inputFile,str2)) {
        if(str2.compare("STARTINGCOMPOUNDS") == 0) {
            break;
        }
        if(largestCompoundID < stoi(str2)) {
            continue;
        }
        auto goal = h.getCompound(stoi(str2));
        goalCompound.push_back(goal->id);
    }

    while(getline(inputFile,str2)) {
        if(largestCompoundID < stoi(str2)) {
            continue;
        }
        auto starting = h.getCompound(stoi(str2));
        starting->molecularWeight = weights.at(stoi(str2));
        startingCompound.push_back(starting->id);
    }

    weights.clear();
    weights.shrink_to_fit();

    h.convertToBHypergraph();
    h.graphToGraphviz("Output2", goalCompound, startingCompound);


    /*std::cout << "Test for Carstens" << std::endl;
    auto dBest = h.yenHyp(*goal, startingCompound, atoi(argv[4]), false);
    h.printResults(dBest);*/
    
    std::cout << "Test for Nielsens" << std::endl;
    auto dBest = h.yenHyp(goalCompound, startingCompound, atoi(argv[2]), true);
    h.printResults(dBest);

    inputFile.close();
    inFile.close();
	return 0;
}