#include <iostream>
#include <vector>

struct CompoundNode
	{
		CompoundNode(int id) : id(id) {};

		int id;
		std::string name;
		double molecularWeight;
		double cost;
		int maxYieldEdge = -1;
		bool visited = false;
		int index = -1;

		std::vector<int> productOfReaction; //Contains a reference to the ID of a reaction in which the compound is a product of. 
		std::vector<int> eductOfReaction; //Contains a reference to the ID of a reaction in which the compound is a educt of. 
	
	};