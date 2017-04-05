#include <iostream>
#include <vector>
#include <cassert>
#include <climits>
#include <cmath>

struct HyperGraph
{
	
	struct CompoundNode
	{

		CompoundNode(){};

		CompoundNode(int id) : id(id) {};

		int id;
		std::string name;
		double price;
		double cost;
		int minEdge;

		std::vector<int> productOfReaction; //Contains a reference to the ID of a reaction in which the compund is a product of. 
	};

	struct ReactionNode
	{

		ReactionNode(){};

		ReactionNode(int id, std::vector<int> head, std::vector<int> tail, double yield) : id(id), head(head), tail(tail), yield(yield) {};
		//Change constuctor (No need to add head and tail if method does so)
		int id;
		double yield;	
		std::vector<int> head;
		std::vector<int> tail;
	};

private: 
	std::vector<CompoundNode> compoundList;
	std::vector<ReactionNode> reactionList;

public:
	HyperGraph() {
	}


public:

	//add compound to set.
	void addCompound(int compoundID) {
		std::cout << "Adding: " << compoundID << std::endl;
		CompoundNode *cn = new CompoundNode(compoundID);
		compoundList.at(compoundID) = *cn;
	} 

	void addPointerFromCompound (int compoundID, int reactionID) {
		std::cout << "Adding pointer from compound " << compoundID << " to reaction " << reactionID << std::endl; 
		auto compound = getCompound(compoundID);
		compound->productOfReaction.push_back(reactionID);
		std::cout << "size of PoR is " << compound->productOfReaction.size() << " after insertion" << std::endl;
	}	

	void addReaction(int id, std::vector<int> head, std::vector<int> tail, double yield) {
		auto headIterator = head.begin();
		auto tailIterator = tail.begin();

		//add active compounds if not existing
		while(headIterator != head.end()) {
			std::cout << "head it is pointing at: " << *headIterator << std::endl;
			if ( *headIterator >= compoundList.size()) {
				compoundList.resize(*headIterator+1);
			}
			if ( compoundList.at(*headIterator).id == 0) {
				addCompound(*headIterator);
			}
			addPointerFromCompound(*headIterator, id);
			++headIterator;
		}
		while(tailIterator != tail.end()) {
			std::cout << "tail it is pointing at: " << *tailIterator << std::endl;
			if ( *tailIterator >= compoundList.size()) {
				compoundList.resize(*tailIterator+1);
			}
			if ( compoundList.at(*tailIterator).id == 0) {
				addCompound(*tailIterator);
			}
			++tailIterator;
		}
		if(id >= reactionList.size()) {
			reactionList.resize(id+1);
		}
		ReactionNode *rn = new ReactionNode(id, head, tail, yield);
		reactionList.at(id) = *rn;
	}

	double minCost(CompoundNode &v, std::vector<int> &list) {
		auto startingCompoundIterator = list.begin();
		while (startingCompoundIterator != list.end()) {
			if (v.id == *startingCompoundIterator) {
				return v.cost;
			}
			++startingCompoundIterator;
		}
		double minimumCost = HUGE_VAL; // infinity
		auto productOfReactionIterator = v.productOfReaction.begin();
		for(auto productOfReactionIterator = v.productOfReaction.begin(); productOfReactionIterator != v.productOfReaction.end(); ++productOfReactionIterator ) {
			ReactionNode *r1 = getReaction(*productOfReactionIterator);
			double cost = r1->yield;
			for (auto reactionTailIterator = r1->tail.begin(); reactionTailIterator != r1->tail.end(); ++reactionTailIterator) {
				cost = cost + minCost(*getCompound(*reactionTailIterator), list);

			}
			if (minimumCost > cost) {
				minimumCost = cost;
				v.minEdge = *productOfReactionIterator;
			}
		}
		return minimumCost;
	}

	double shortestPath(CompoundNode &v, std::vector<int> &startingCompounds) {
		CompoundNode *s = new CompoundNode();
		s->id = 0;
		auto startingCompoundIterator = startingCompounds.begin();
		while (startingCompoundIterator != startingCompounds.end()) {
			std::vector<int> head;
			head.push_back(*startingCompoundIterator);
			std::vector<int> tail;
			tail.push_back(s->id);
			addReaction(0, head, tail, 0.0);
			++startingCompoundIterator;
		}
		return minCost(v, startingCompounds);
	}

	ReactionNode* getReaction(int id){
		if (reactionList.at(id).id == 0 ) {
			std::cout << "No reaction with ID: " << id << std::endl;
			assert(false);
		}
		return &reactionList.at(id);
	}	

	CompoundNode* getCompound(int id){
		if (compoundList.at(id).id == 0 ) {
			std::cout << "No compound with ID: " << id << std::endl;
			assert(false);
		}
		return &compoundList.at(id);
	}	


	void printCompoundList() {
		int i = 0;
		for (auto it = compoundList.begin(); it != compoundList.end(); ++it) {
			std::cout << "Plads: " << i << ", ID: " << it->id<< std::endl;
			i++;
		}
	}

	void printReactionList() {
		std::cout << "reactionList" << std::endl;
		int i = 0;
		for (auto it = reactionList.begin(); it != reactionList.end(); ++it) {
			std::cout << "Plads: " << i << ", ID: " << it->id << std::endl;
			i++;
		}
	}
};