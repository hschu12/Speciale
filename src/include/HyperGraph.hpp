#include <iostream>
#include <vector>
#include <cassert>
#include <climits>
#include <cmath>
#include <queue>
#include <algorithm>
#include <fstream>
#include "PriorityQueue.hpp"
#include "ReactionNode.hpp"

struct HyperGraph
{
	struct cmpPair //Compare function used to sort pair(vector, pair(double, vector)) on the double value.
	{	
		bool operator()(std::pair <std::vector<bool> , std::pair<double, std::vector<int> > > a, std::pair <std::vector<bool> , std::pair<double, std::vector<int> > > b) {
			return a.second.first > b.second.first;
		}
	};


	struct cmp //Compare function used to sort compounds on cost.
	{
		bool operator()(CompoundNode a, CompoundNode b){
			return a.cost > b.cost;
		}

	};

private: 
	std::vector<CompoundNode> compoundList;
	std::vector<ReactionNode> reactionList;

public:
	HyperGraph(int compoundListSize, int reactionListSize) {
		compoundList.reserve(compoundListSize);
		reactionList.reserve(reactionListSize);
	}


public:

	inline void addCompound(int compoundID) {
		CompoundNode *cn = new CompoundNode(compoundID);
		compoundList[compoundID] = *cn;
	} 
	
	inline void addPointerFromReactionToCompound (int compoundID, int reactionID) {
		auto compound = getCompound(compoundID);
		compound->productOfReaction.push_back(reactionID);
	}		

	inline void addPointerFromCompoundToReaction (int compoundID, int reactionID) {
		auto compound = getCompound(compoundID);
		compound->eductOfReaction.push_back(reactionID);
	}	

	void addReaction(int id, std::vector<int> head, std::vector<int> tail, double yield) {
		auto headIterator = head.begin();
		auto tailIterator = tail.begin();
		//add active compounds if not existing
		while(headIterator != head.end()) {
			
			if ( *headIterator >= compoundList.size()) {
				compoundList.resize(*headIterator+1, CompoundNode(0));
			}
			if ( compoundList[*headIterator].id == 0) {
				addCompound(*headIterator);
			}
			addPointerFromReactionToCompound(*headIterator, id);
			++headIterator;
		}
		while(tailIterator != tail.end()) {
			if ( *tailIterator >= compoundList.size()) {
				compoundList.resize(*tailIterator+1, CompoundNode(0));
			}
			if ( compoundList[*tailIterator].id == 0) {
				addCompound(*tailIterator);
			}
			addPointerFromCompoundToReaction(*tailIterator, id);
			++tailIterator;
		}
		if(id >= reactionList.size()) {
			std::vector<int> v;
			reactionList.resize(id+1, ReactionNode(0, v, v, 0));
		}
		ReactionNode *rn = new ReactionNode(id, head, tail, yield);
		reactionList[id] = *rn;
	}

	void convertToBHypergraph() {
		std::vector<int> toHandle;
		for(ReactionNode &reaction : reactionList) {
			if(reaction.head.size() > 1) { //If it is NOT a B-hyperedge
				toHandle.push_back(reaction.id);
			}
		}
		for(auto r : toHandle) {
			ReactionNode *rn = getReaction(r);
			for(int i = 1; i < rn->head.size(); i++) {
				std::vector<int> headCompound;
				headCompound.push_back(rn->head[i]);
				addReaction(reactionList.size(), headCompound, rn->tail, rn->yield);
				ReactionNode* node = getReaction(reactionList.size()-1);
				node->originalID = rn->id;
			}
			int size = rn->head.size();
			for(int i = 1; i < size; i++) {
				rn->head.pop_back();
			}
		}
	}

	void addReactionToS(int id, std::vector<int> head, std::vector<int> tail, double yield) {
		auto headIterator = head.begin();
		auto tailIterator = tail.begin();
		//add active compounds if not existing
		while(headIterator != head.end()) {
			addPointerFromReactionToCompound(*headIterator, id);
			++headIterator;
		}
		while(tailIterator != tail.end()) {
			addPointerFromCompoundToReaction(*tailIterator, id);
			++tailIterator;
		}
		if(id >= reactionList.size()) {
			reactionList.resize(id+1, ReactionNode(id, head, tail, yield));
		}
		ReactionNode *rn = new ReactionNode(id, head, tail, yield);
		reactionList[id] = *rn;
	}

	/****************************
	*							*
	*		SHORTEST PATH		*
	*							*
	*****************************/


	// Function used to reduce the size of the graph before applying Nielsens Shortest path algorithm. If a part
	// of a graph is not used there is no need to check for shortest path in that direction. 
	// This function uses a recursive top down approach to find the shortest path from goal to starting compounds
	// and makes an overlay to the graph containing only the used edges using a vector of booleans.
	std::vector<bool> reduceGraph(std::vector<bool> &graphOverlay, CompoundNode &v, CompoundNode &s) {
		if (v.id == s.id) {
			return graphOverlay;
		}
		if (!v.visited) {
			for(auto reaction : v.productOfReaction ) {
				ReactionNode *r1 = getReaction(reaction);	
				for (auto tailCompound : r1->tail) {
					v.visited = true;
					graphOverlay = reduceGraph(graphOverlay, *getCompound(tailCompound), s);
				}
				graphOverlay[r1->id] = true;
			}
		}
		return graphOverlay;
	}

	double maxYield(CompoundNode &v, CompoundNode &s, std::vector<bool> &overlay) {
		double maximumYield = std::numeric_limits<double>::max(); // infinity
		for(auto reaction : v.productOfReaction ) {
			if (overlay[reaction]) {

				ReactionNode *r1 = getReaction(reaction);
				double cost = 0;
				for (auto reactionTailIterator : r1->tail) {
					if ((*getCompound(reactionTailIterator)).id != 0) {
						cost  += (1/r1->yield) * maxYield(*getCompound(reactionTailIterator), s, overlay);
					}
					else {
						cost = v.cost;
					}
				}
				if (maximumYield > cost) {
					maximumYield = cost;
					v.maxYieldEdge = reaction;
				}
			}
		}
		return maximumYield;
	}

	std::pair < std::vector<bool> , std::pair< double, std::vector<int>> >shortestPathMaximumYield(CompoundNode &v, std::vector<int> &startingCompounds, std::vector<bool> &overlay, CompoundNode &s) {
		resetNodeCost();

		for(auto compoundID : startingCompounds){
			auto compound = getCompound(compoundID);
			compound->cost = compound->molecularWeight;
		}

		double result = maxYield(v, s, overlay);
		std::vector<int> shortest = getShortestPathYield(v.id);

		std::pair<double, std::vector<int>> pair (result, shortest);
		std::pair<std::vector<bool> , std::pair<double, std::vector<int>> > pair2 (overlay, pair);

		return pair2;
	}

	std::vector< std::vector<bool> > backwardBranching(std::pair < double, std::vector<int> > pair, std::vector<bool> overlay) {
		std::vector< std::vector<bool> > B;

		for(int i = 0; i < pair.second.size(); i++) { //for each edge in path

			std::vector<int> toRemove (1,pair.second[i]); //pick edge i to remove

			for(auto it = toRemove.begin(); it != toRemove.end(); ++it) {
				overlay[*it] = false; //remove edge i
			}

			if(i == 0) {
				B.push_back(overlay);
				continue;
			}
			else {
				for (int j = i-1; j >= 0; j--){
					ReactionNode* reaction = getReaction(pair.second[j]);
					CompoundNode* compound = getCompound(reaction->head.front());
					for (auto reaction : compound->productOfReaction) {
						overlay[reaction] = false;
					}
					overlay[pair.second[j]] = true;
				}
				B.push_back(overlay);
			}
		}
		return B;
	}

	std::vector< std::pair < double, std::vector<int>> > yenHyp (std::vector<int> &goalCompounds, std::vector<int> &startingCompounds, int K, bool cycles) {

		std::vector< std::pair <std::vector<bool> , std::pair<double, std::vector<int> > > > L;
		
		std::make_heap (L.begin(),L.end(), cmpPair());

		std::vector< std::pair < double, std::vector<int> > >A; //list of k shortest hyperpaths.
		CompoundNode *s = createDummyS(startingCompounds);
		CompoundNode *v = createDummyT(goalCompounds);

		std::vector<int> toRemove;
		std::vector<bool> overlay = createOverlay(toRemove, true);
		std::vector<bool> graphOverlay = createOverlay(toRemove, false);

		if(cycles) {
			graphOverlay = reduceGraph(graphOverlay, *v, *s);
		}

		std::pair< std::vector<bool> , std::pair<double, std::vector<int>> > pair;
		if(cycles) {
			pair = ShortestHyperNielsen(graphOverlay, *v, startingCompounds, overlay);
		} 
		else{
			pair = shortestPathMaximumYield(*v, startingCompounds, overlay, *s);
		}
		L.push_back(pair);
		for ( int k = 1; k <= K; k++) {
			std::cout << "k = " << k << std::endl;
			if (L.empty()) {
				break;
			}
			pair = L.front(); 	//Get shortest path with max yield and remove from heap
  			std::pop_heap (L.begin(),L.end(), cmpPair() ); 
  			L.pop_back();	

  			std::pair < double, std::vector<int> > toA (pair.second.first, pair.second.second);
  			A.push_back(toA);	//Add current best plan to k-best

  			for (auto newOverlay : backwardBranching(pair.second, pair.first)) { //Potential danger (function call in for loop)
  				std::pair < std::vector<bool> , std::pair<double, std::vector<int>> >potentialNewPlan; 
  				if(cycles){
  					potentialNewPlan = ShortestHyperNielsen(graphOverlay, *v, startingCompounds, newOverlay);
				}
				else{
  					potentialNewPlan = shortestPathMaximumYield(*v, startingCompounds, newOverlay, *s);
  				}
  				if (pathIsComplete(potentialNewPlan.second.first)) {
  					if(pathNotAlreadyFound(potentialNewPlan.second.second, A)){
  						L.push_back(potentialNewPlan); 
						std::push_heap (L.begin(),L.end(), cmpPair());
  					}
  				}
  			}
  		}
  		removeDummy(startingCompounds, goalCompounds);
  		resetVisited();
  		return A;
	}

	std::pair < std::vector<bool> , std::pair< double, std::vector<int>> > ShortestHyperNielsen ( std::vector<bool> &graphOverlay, CompoundNode &goal, std::vector<int> &startingCompounds, std::vector<bool> &overlay) {
		//Initinalize
		PriorityQueue Q (compoundList.capacity());

		resetNodeIndex();
		resetNodeCost();

		for(auto reaction : reactionList) {
			ReactionNode *r = getReaction(reaction.id);
			r->kj = 0;
		}

		CompoundNode *s = getCompound(0);
		Q.push(*s); 
		//Initiliaze done
		while(!Q.empty()) {
  			auto compound = Q.pop();
  			for (auto reaction : compound.eductOfReaction) {
  				if(overlay[reaction] && graphOverlay[reaction]) {
  					ReactionNode *r = getReaction(reaction);
  					r->kj++;
	
	  				if (r->kj == r->tail.size()) {

						CompoundNode *c = getCompound(*(r->head.begin()));
  						if(compound.id != 0) {
  							double F = 0;
  							for(auto educt : r->tail) {
  								CompoundNode *temp = getCompound(educt);	
  								F += temp->cost * (1/r->yield);
  							}
  							if (c->cost > F) {
  								c->cost = F;
  								c->maxYieldEdge = reaction;	

  								if(!Q.contains(*c)) {
									Q.push(*c); 
  								}
  								else{
  									Q.decrease_key(c->index, *c);
  								}
  								
  							}
  						}
  						else{
  							c->cost = c->molecularWeight;
  							c->maxYieldEdge = reaction;	
							Q.push(*c); 
  						}
  					}
  				}
  			}
		}
		double result = goal.cost;
		std::vector<int> shortest = getShortestPathYield(goal.id);
		std::pair<double, std::vector<int>> pair (result, shortest);
		std::pair<std::vector<bool> , std::pair<double, std::vector<int>> > pair2 (overlay, pair);

		return pair2;
	}

	bool vectorContainsCompoundNode(std::vector<CompoundNode> v, CompoundNode c) {
		auto iterator = v.begin();
		while (iterator != v.end()) {
			if (iterator->id == c.id) {
				return true;
			}
			++iterator;
		}
		return false;
	}

	std::vector<bool> createOverlay(std::vector<int> toRemove, bool filler) {
		std::vector<bool> overlay;
		overlay.resize(reactionList.size(), filler);

		for(auto it : toRemove) {
			overlay[it] = !filler;
		}

		return overlay;
	}

	CompoundNode* createDummyS(std::vector<int> &startingCompounds) {
		CompoundNode *s = new CompoundNode(0); //MIGHT NOT BE NEEDED
		s->id = 0;
		auto startingCompoundIterator = startingCompounds.begin();
		while (startingCompoundIterator != startingCompounds.end()) {
			std::vector<int> head;
			head.push_back(*startingCompoundIterator);
			std::vector<int> tail;
			tail.push_back(s->id);
			addReactionToS(reactionList.size(), head, tail, 1.0); 	
			ReactionNode *r = getReaction(reactionList.size()-1);
			CompoundNode *c = getCompound(*startingCompoundIterator);
			c->productOfReaction.push_back(r->id);
			s->eductOfReaction.push_back(r->id);
			++startingCompoundIterator;
		}
		s->molecularWeight = 1; // Need to be 1 to start out the molecularWeight calculations.
		s->cost = 1;
		compoundList[s->id] = *s;
		return s;
	}

	CompoundNode* createDummyT(std::vector<int> &goalCompounds) {
		CompoundNode *t = new CompoundNode(0); //MIGHT NOT BE NEEDED
		t->id = compoundList.size();
		auto goalCompoundIterator = goalCompounds.begin();
		while (goalCompoundIterator != goalCompounds.end()) {
			std::vector<int> head;
			head.push_back(t->id);
			std::vector<int> tail;
			tail.push_back(*goalCompoundIterator);
			addReaction(reactionList.size(), head, tail, 1.0); 	
			ReactionNode *r = getReaction(reactionList.size()-1);
			CompoundNode *c = getCompound(*goalCompoundIterator);
			t->productOfReaction.push_back(r->id);
			c->eductOfReaction.push_back(r->id);
			++goalCompoundIterator;
		}
		t->molecularWeight = 1; // Need to be 1 to start out the molecularWeight calculations.
		t->cost = std::numeric_limits<double>::max();
		return getCompound(compoundList.size()-1);
	}

	bool pathNotAlreadyFound(std::vector<int> potentialNewPlan, std::vector<std::pair < double, std::vector<int>> > A) {
		for(auto path : A) {
			if (std::equal(potentialNewPlan.begin(), potentialNewPlan.end(), path.second.begin())) {
				return false;
			}
		}
		return true;
	}

	bool pathIsComplete(double yield) {
		if (yield > 0) {
			return true;
		}
		return false;
	}

	std::vector<int> getShortestPathYield(int vertex){
		std::vector<int> shortestPath; //contains the reactions for shortest path.
		std::queue<int> queue; //Contains not processed compounds
		queue.push(vertex);
		while (!(queue.empty())) {
			CompoundNode* compound = getCompound(queue.front());
			queue.pop();
			if (compound->maxYieldEdge > 0) {
				ReactionNode* reaction = getReaction(compound->maxYieldEdge);
				for (int educt : reaction->tail) {
					queue.push(educt);
				}
				shortestPath.push_back(reaction->id);
			}
		}
		resetMaxYieldEdges();
		return shortestPath;
	}

	void resetVisited(){
		for (CompoundNode &compound : compoundList) {
			compound.visited = false;
		}
	}

	void resetNodeIndex(){
		for (CompoundNode &compound : compoundList) {
			compound.index = -1;
		}
	}

	void resetMaxYieldEdges(){
		for (CompoundNode &compound : compoundList) {
			compound.maxYieldEdge = -1;

		}
	}

	void resetNodeCost(){
		for (CompoundNode &compound : compoundList) {
			if( compound.id == 0) {
				compound.cost = 1;
			}
			else {
				compound.cost = std::numeric_limits<double>::max();
			}
		}
	}

	void removeDummy(std::vector<int> &startingCompounds, std::vector<int> &goalCompounds) {
		for(auto compound : startingCompounds) {
			reactionList.pop_back();
		}
		for(auto compound : goalCompounds) {
			reactionList.pop_back();
		}
		compoundList.pop_back();
	}

	/********************************
	*								*
	*		GETTERS AND SETTERS		*
	*								*
	*********************************/

	ReactionNode* getReaction(int id){
		return &reactionList[id];
	}	

	CompoundNode* getCompound(int id){
		return &compoundList[id];
	}	

	/************************
	*						*
	*		STATISTICS		*
	*						*
	*************************/

	int compoundInDegree(int compoundID) {
		return getCompound(compoundID)->productOfReaction.size();
	}

	int reactionInDegree(int reactionID) {
		return getReaction(reactionID)->tail.size();
	}

	int reactionOutDegree(int reactionID) {
		return getReaction(reactionID)->head.size();
	}

	int highestCompoundInDegree() {
		int highest = -1;
		for (auto it : compoundList) {
			if (it.id != 0) {
				int degree = compoundInDegree(it.id);
				if (highest == -1) {
					highest = it.id;
				}
				if (degree > compoundInDegree(highest)) {
					highest = it.id;
				}
			}
		}
		return highest;
	}

	int highestCompoundOutDegree() {
		std::vector<int> OutDegreeList;
		OutDegreeList.resize(compoundList.size());
		for (auto reactionIterator : reactionList) {
			for (auto tailIterator : reactionIterator.tail) {
				OutDegreeList[tailIterator]++;
			}
		}
		int highest = -1;
		for (auto it = OutDegreeList.begin(); it != OutDegreeList.end(); ++it ) {
			if (*it > highest) {
				highest = *it;
			}
		}
		return highest;
	}

	int highestReactionInDegree() {
		int highest = -1;
		for (auto it : reactionList) {
			if (it.id != 0) {
				int degree = reactionInDegree(it.id);
				if (highest == -1) {
					highest = it.id;
				}
				if (degree > reactionInDegree(highest)) {
					highest = it.id;
				}
			}
		}
		return highest;
	}

	int highestReactionOutDegree() {
		int highest = -1;
		for (auto it : reactionList) {
			if (it.id != 0) {
				int degree = reactionOutDegree(it.id);
				if (highest == -1) {
					highest = it.id;
				}
				if (degree > reactionOutDegree(highest)) {
					highest = it.id;
				}
			}
		}
		return highest;
	}

	/************************
	*						*
	*		PRINTING		*
	*						*
	*************************/

	void printCompoundList() {
		int i = 0;
		for (auto it : compoundList) {
			std::cout << "Plads: " << i << ", ID: " << it.id<< std::endl;
			i++;
		}
	}

	void printReactionList() {
		std::cout << "reactionList" << std::endl;
		int i = 0;
		for (auto it :reactionList) {
			std::cout << "Plads: " << i << ", ID: " << it.id << std::endl;
			i++;
		}
	}

	void printResults(std::vector< std::pair < double, std::vector<int>>> bestPlans) {
		int i = 1;
		for( auto plan : bestPlans) {
			if (!plan.second.empty()){
    			std::cout << "Plan: " << i << std::endl;
    			std::string str = std::to_string(i);
    			std::ofstream graphFile;
				graphFile.open("plan" + str + ".gv");
				graphFile << "digraph G { \n";
				graphFile << "{\n";
				
				for( auto pathElement : plan.second) {
    				if(pathElement < reactionList.size()) {
    					auto reaction = getReaction(pathElement);
    					if(reaction->originalID == 0) {
							graphFile << "	R" << reaction->id << " [label = \"R" << reaction->id << ". Yield: " << reaction->yield << "\"]\n";
						}
						else {
							graphFile << "	R" << reaction->originalID << " [label = \"R" << reaction->originalID << ". Yield: " << reaction->yield << "\"]\n";
						}
	      			}
    			}

				graphFile << "}\n";
    			
    			for( auto pathElement : plan.second) {
    				if(pathElement < reactionList.size()) {
	      				auto reaction = getReaction(pathElement);
	      				if(reaction->originalID == 0) {
		      				std::cout << "Reaction: " << pathElement << std::endl;
						}
						else{
			   				std::cout << "Reaction: " << reaction->originalID << std::endl;
						}
	      				if(reaction->originalID == 0) {
		      				for( auto headelement : reaction->head) {
								graphFile << "	R" << reaction->id << " -> " << headelement << ";\n";
							}
	    					for( auto tailelement : reaction->tail) {
	    			  			graphFile << "	" << tailelement << " -> R" << reaction->id << ";\n";
	    					}
	    				}
	    				else{
	    					for( auto headelement : reaction->head) {
								graphFile << "	R" << reaction->originalID << " -> " << headelement << ";\n";
							}
	    					for( auto tailelement : reaction->tail) {
	    			  			graphFile << "	" << tailelement << " -> R" << reaction->originalID << ";\n";
	    					}
	    				}
			      	}
    			}

    			std::cout << "With yield: " << plan.first << "\n" << std::endl;
    			i++;
				graphFile << "}";
    			graphFile.close();
    		}
    		else{
    			std::cout << "No plan found" << std::endl;
    		}
  		} 
	}

	void printGraphConsole() {
		for( auto reaction : reactionList) {
			if (reaction.id == 0) {
				continue;
			}
			std::cout << "ID: " << reaction.id << ". Yield: " << reaction.yield <<  std::endl;
    		std::cout << "head: " << *reaction.head.begin() << std::endl;
    		for( auto tailelement : reaction.tail) {
      			std::cout << "tail: " << tailelement << std::endl;
    		}
    		
  		} 
	}

	void printpath(std::pair<double, std::vector<int>> p) {
		std::cout << "printing list" << std::endl;
		for (auto k : p.second) {
			std::cout << k << std::endl;
		}
		std::cout << "with yield " << p.first << std::endl;
	}

	void printoverlay(std::vector<bool> v) {
		std::cout << "printing overlay" << std::endl;
		for (int i = 0; i <= v.size()-1; i++) {
			std::cout <<  i << " ";
		}
		std::cout << std::endl;

		for (auto k : v) {
			std::cout << k << " ";
		}
		std::cout << std::endl;
	}

	void graphToGraphviz(std::string s, std::vector<int> &goalCompounds, std::vector<int> &startingCompounds) {
		std::ofstream graphFile;
		std::cout << "Creating Graphviz Graph" << std::endl;
		graphFile.open(s + ".gv");
		graphFile << "digraph G { \n";
		graphFile << "{\n";

		for( auto reaction : reactionList) {
			if(reaction.id == 0) {
				continue;
			}
			graphFile << "	R" << reaction.id << " [label = \"R" << reaction.id << ". Yield: " << reaction.yield << "\"]\n";
		}

		for(auto s : startingCompounds){
			graphFile << "	" << s << " [color = green, fontcolor = green]\n";

		}
		for(auto t : goalCompounds) {
			graphFile << "	" << t << " [color = red, fontcolor = red]\n";
		}

		graphFile << "}\n";

		for( auto reaction : reactionList) {
			if(reaction.id == 0) {
				continue;
			}
			for( auto headelement : reaction.head) {
				graphFile << "	R" << reaction.id << " -> " << headelement << ";\n";
			}
    		for( auto tailelement : reaction.tail) {
      			graphFile << "	" << tailelement << " -> R" << reaction.id << ";\n";
    		}
  		} 
		graphFile << "}";
		graphFile.close();
	}
};