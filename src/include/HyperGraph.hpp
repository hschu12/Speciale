#include <iostream>
#include <vector>
#include <cassert>
#include <climits>
#include <cmath>
#include <queue>
#include <algorithm>
#include <fstream>

struct HyperGraph
{

struct ReactionNode
	{
		ReactionNode(){};

		ReactionNode(int id, std::vector<int> head, std::vector<int> tail, double yield) : id(id), head(head), tail(tail), yield(yield) {};
		int id;
		double yield;	
		int kj;
		std::vector<int> head;
		std::vector<int> tail;
	};

	struct CompoundNode
	{
		CompoundNode(){};

		CompoundNode(int id) : id(id) {};

		int id;
		std::string name;
		double molecularWeight;
		double cost;
		int maxYieldEdge = -1;
		bool visited = false;

		std::vector<int> productOfReaction; //Contains a reference to the ID of a reaction in which the compound is a product of. 
		std::vector<int> eductOfReaction; //Contains a reference to the ID of a reaction in which the compound is a educt of. 
	};

	struct cmpPair //Compare function used to sort pair(vector, pair(double, vector)) on the double value.
	{	
		bool operator()(std::pair <std::vector<bool> , std::pair<double, std::vector<int> > > a, std::pair <std::vector<bool> , std::pair<double, std::vector<int> > > b) {
			return a.second.first > b.second.first;
		}
	};


	struct cmp //Compare function used to sort compounds on molecularWeight.
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
		compoundList.resize(compoundListSize);
		reactionList.resize(reactionListSize);
	}


public:

	inline void addCompound(int compoundID) {
		CompoundNode *cn = new CompoundNode(compoundID);
		compoundList.at(compoundID) = *cn;
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
				compoundList.resize(*headIterator+1);
			}
			if ( compoundList.at(*headIterator).id == 0) {
				addCompound(*headIterator);
			}
			addPointerFromReactionToCompound(*headIterator, id);
			++headIterator;
		}
		while(tailIterator != tail.end()) {
			if ( *tailIterator >= compoundList.size()) {
				compoundList.resize(*tailIterator+1);
			}
			if ( compoundList.at(*tailIterator).id == 0) {
				addCompound(*tailIterator);
			}
			addPointerFromCompoundToReaction(*tailIterator, id);
			++tailIterator;
		}
		if(id >= reactionList.size()) {
			reactionList.resize(id+1);
		}
		ReactionNode *rn = new ReactionNode(id, head, tail, yield);
		reactionList.at(id) = *rn;
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
			reactionList.resize(id+1);
		}
		ReactionNode *rn = new ReactionNode(id, head, tail, yield);
		reactionList.at(id) = *rn;
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
				graphOverlay.at(r1->id) = true;
			}
		}
		return graphOverlay;
	}

	double maxYield(CompoundNode &v, CompoundNode &s, std::vector<bool> &overlay) {
		double maximumYield = std::numeric_limits<double>::max(); // infinity
		for(auto reaction : v.productOfReaction ) {
			if (overlay.at(reaction)) {

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

			std::vector<int> toRemove (1,pair.second.at(i)); //pick edge i to remove

			for(auto it = toRemove.begin(); it != toRemove.end(); ++it) {
				overlay.at(*it) = false; //remove edge i
			}

			if(i == 0) {
				B.push_back(overlay);
				continue;
			}
			else {
				for (int j = i-1; j >= 0; j--){
					ReactionNode* reaction = getReaction(pair.second.at(j));
					CompoundNode* compound = getCompound(reaction->head.front());
					for (auto reaction : compound->productOfReaction) {
						overlay.at(reaction) = false;
					}
					overlay.at(pair.second.at(j)) = true;
				}
				B.push_back(overlay);
			}
		}
		return B;
	}

	std::vector< std::pair < double, std::vector<int>> > yenHyp (CompoundNode &v, std::vector<int> &startingCompounds, int K, bool cycles) {
		std::vector< std::pair <std::vector<bool> , std::pair<double, std::vector<int> > > > L;
		
		std::make_heap (L.begin(),L.end(), cmpPair());

		std::vector< std::pair < double, std::vector<int> > >A; //list of k shortest hyperpaths.
		
		CompoundNode *s = createDummyNode(startingCompounds);

		std::vector<int> toRemove;
		std::vector<bool> overlay = createOverlay(toRemove, true);
		std::vector<bool> graphOverlay = createOverlay(toRemove, false);

		if(cycles) {
			graphOverlay = reduceGraph(graphOverlay, v, *s);
		}

		std::pair< std::vector<bool> , std::pair<double, std::vector<int>> > pair;
		if(cycles) {
			pair = ShortestHyperNielsen(graphOverlay, v, startingCompounds, overlay);
		} 
		else{
			pair = shortestPathMaximumYield(v, startingCompounds, overlay, *s);
		}
		L.push_back(pair);
		for ( int k = 0; k < K; k++) {
			std::cout << k << std::endl;
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
  					potentialNewPlan = ShortestHyperNielsen(graphOverlay, v, startingCompounds, newOverlay);
				}
				else{
  					potentialNewPlan = shortestPathMaximumYield(v, startingCompounds, newOverlay, *s);
  				}
  				if (pathIsComplete(potentialNewPlan.second.first)) {
  					if(pathNotAlreadyFound(potentialNewPlan.second.second, A)){
  						L.push_back(potentialNewPlan); 
						std::push_heap (L.begin(),L.end(), cmpPair());
  					}
  				}
  			}
  		}
  		removeDummy(startingCompounds);
  		resetVisited();
  		return A;
	}

	std::pair < std::vector<bool> , std::pair< double, std::vector<int>> > ShortestHyperNielsen ( std::vector<bool> &graphOverlay, CompoundNode &goal, std::vector<int> &startingCompounds, std::vector<bool> &overlay) {
		//Initinalize
 		std::vector< CompoundNode > Q;		
		std::make_heap (Q.begin(),Q.end(), cmp());

		resetNodeCost();

		for(auto reaction : reactionList) {
			ReactionNode *r = getReaction(reaction.id);
			r->kj = 0;
		}

		CompoundNode *s = getCompound(0);
		Q.push_back(*s); 
		std::push_heap (Q.begin(),Q.end(), cmp());
		//Initiliaze done
		while(!Q.empty()) {
			CompoundNode compound = Q.front(); 	//Get node with highest molecularWeight

  			std::pop_heap (Q.begin(),Q.end(), cmp() ); 
  			Q.pop_back();	
  			for (auto reaction : compound.eductOfReaction) {
  				if(overlay.at(reaction) && graphOverlay.at(reaction)) {
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
  								if(!vectorContainsCompoundNode(Q, *c)) {
  									Q.push_back(*c); 
									std::push_heap (Q.begin(),Q.end(), cmp());
  								}
  								
  							}
  						}
  						else{
  							c->cost = c->molecularWeight;
  							c->maxYieldEdge = reaction;	
  							if(!vectorContainsCompoundNode(Q, *c)) {
  								Q.push_back(*c); 
								std::push_heap (Q.begin(),Q.end(), cmp());
  							}
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
			overlay.at(it) = !filler;
		}

		return overlay;
	}

	CompoundNode* createDummyNode(std::vector<int> &startingCompounds) {
		CompoundNode *s = new CompoundNode(); //MIGHT NOT BE NEEDED
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
		compoundList.at(s->id) = *s;
		return s;
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

	void removeDummy(std::vector<int> &startingCompounds) {
		for(auto compound : startingCompounds) {
			reactionList.pop_back();
		}
	}

	/********************************
	*								*
	*		GETTERS AND SETTERS		*
	*								*
	*********************************/

	ReactionNode* getReaction(int id){
		return &reactionList.at(id);
	}	

	CompoundNode* getCompound(int id){
		return &compoundList.at(id);
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
				OutDegreeList.at(tailIterator)++;
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
    			for( auto pathElement : plan.second) {
      				std::cout << "Reaction: " << pathElement << std::endl;
    			}
    			std::cout << "With yield: " << plan.first << "\n" << std::endl;
    			i++;
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

	void graphToGraphviz(std::string s) {
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