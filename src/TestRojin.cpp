#include "include/HyperGraph.hpp"
#include <iostream>
#include <fstream>
#include <math.h>

void testResults(std::vector< std::pair < double, std::vector<int>>> bestPlans, std::string path) {
    std::ifstream scoreFile;
    scoreFile.open(path);
    if (!scoreFile) {
        std::cout << "Unable to open file";
        exit(1); // terminate with error
    }
    std::string str;
    getline(scoreFile,str);
    getline(scoreFile,str); //ignore two first lines.
    double sigma = 0.00007; //Choosen through testing
    for( auto plan : bestPlans) {
        if (!plan.second.empty()){
            getline(scoreFile, str);
            int first = str.find_first_of("\t", 0);
            int second = str.find_first_of("\t", first+1);
            int third = str.find_first_of("\t", second+1);
            int fouth = str.find_first_of("\t", third+1);
            std::string score = str.substr(third+1, fouth-third-1);

            if(fabs(plan.first - stod(score)) > sigma) {
              std::cout << plan.first << " not equal " << score << std::endl;
            }
        }
        else{
            std::cout << "No plan found" << std::endl;
        }
    } 
    scoreFile.close();
}

int main(int argc, char const *argv[]){
	if(argc < 6){
		std::cout << "To few arguments. Need txt file, number of compounds, number of reactions, number of max synthplans(K), .txt file with scores" << std::endl;
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
    int loop = 0;
    bool reactbool = false;
    int startingCompoundID = 0;
    while(getline(inFile,str)) {
        /*if (loop%1000 == 0) {
        std::cout << loop << "." << std::endl;
        }*/
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
	    		reactions.at(stoi(id)) = stod(id);
	    		std::string yield = str.substr(third+1, fouth-third-1);
	    		yields.at(stoi(id)) = stod(yield);


	    	}
	    	if(!type.compare("N")){ //Returns true if type is N
	    	  	int idINT = stoi(id);
	    	  	compounds.at(idINT) = idINT+1;
                //std::cout << compounds.at(idINT) << std::endl;
                if((str.substr(second+1, third-second-1)).compare("S") == 0){
                    startingCompoundID = idINT+1;
                }
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
 					eductsToReaction.at(stoi(in)).push_back(stod(out)+1);
 				}
 			    if(reactions.at(stoi(out)) == stoi(out)){
 					productsToReaction.at(stoi(out)).push_back(stod(in)+1);
 				}
   			}
   		}
   	}
   	for(int i = 0; i < reactions.size(); i++){
        if(reactions.at(i) != 0){
            h.addReaction(reactions.at(i), productsToReaction.at(i), eductsToReaction.at(i), yields.at(i));
     	}
    }


   	h.graphToGraphviz("Output");

   	auto goal = h.getCompound(1);
   	auto starting = h.getCompound(startingCompoundID);

   	starting->molecularWeight = weights.at(startingCompoundID-1);

   	std::vector<int> startingCompound;
   	startingCompound.push_back(starting->id);


    std::cout << "Test for Carstens" << std::endl;
    auto dBest = h.yenHyp(*goal, startingCompound, atoi(argv[4]), false);
    std::cout << "Checking scores" << std::endl;
    testResults(dBest, argv[5]);
    
    std::cout << "Test for Nielsens" << std::endl;
    dBest = h.yenHyp(*goal, startingCompound, atoi(argv[4]), true);

    std::cout << "Checking scores" << std::endl;
    testResults(dBest, argv[5]);

    inFile.close();
	return 0;
}