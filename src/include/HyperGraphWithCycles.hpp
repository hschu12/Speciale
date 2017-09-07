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
		//Change constuctor (No need to add head and tail if method does so)
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
		double price;
		double cost;
		double weight;
		int minEdge; // might not be neccesary
		int maxYieldEdge = -1;

		std::vector<int> productOfReaction; //Contains a reference to the ID of a reaction in which the compound is a product of. 
		std::vector<int> eductOfReaction;
		ReactionNode *toS;
	};

	struct cmpPair
	{	
		bool operator()(std::pair <std::vector<bool> , std::pair<double, std::vector<int> > > a, std::pair <std::vector<bool> , std::pair<double, std::vector<int> > > b) {
			return a.second.first < b.second.first;
		}
	};


	struct cmp
	{
		bool operator()(CompoundNode a, CompoundNode b){
			return a.weight > b.weight;
		}

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
		//std::cout << "Adding: " << compoundID << std::endl;
		CompoundNode *cn = new CompoundNode(compoundID);
		compoundList.at(compoundID) = *cn;
	} 

	void addPointerFromReactionToCompound (int compoundID, int reactionID) {
		//std::cout << "Adding pointer from compound " << compoundID << " to reaction " << reactionID << std::endl; 
		auto compound = getCompound(compoundID);
		compound->productOfReaction.push_back(reactionID);
		//std::cout << "size of PoR is " << compound->productOfReaction.size() << " after insertion" << std::endl;
	}	

	void addPointerFromCompoundToReaction (int compoundID, int reactionID) {
		//std::cout << "Adding pointer from compound " << compoundID << " to reaction " << reactionID << std::endl; 
		auto compound = getCompound(compoundID);
		compound->eductOfReaction.push_back(reactionID);
		//std::cout << "size of PoR is " << compound->productOfReaction.size() << " after insertion" << std::endl;
	}	

	void addReaction(int id, std::vector<int> head, std::vector<int> tail, double yield) {
		auto headIterator = head.begin();
		auto tailIterator = tail.begin();

		//add active compounds if not existing
		while(headIterator != head.end()) {
			//std::cout << "head it is pointing at: " << *headIterator << std::endl;
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
			//std::cout << "tail it is pointing at: " << *tailIterator << std::endl;
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


	/*double minCost(CompoundNode &v, CompoundNode &s, std::vector<bool> &overlay) {
		if (v.id == s.id) {
			return 0;
		}
		double minimumCost = HUGE_VAL; // infinity
		for (auto productOfReactionIterator : v.productOfReaction ) {//.begin(); productOfReactionIterator != v.productOfReaction.end(); ++productOfReactionIterator ) {
			ReactionNode *r1;
			//std::cout << "before if " << productOfReactionIterator << std::endl;
			if (overlay.at(productOfReactionIterator)) {
				//std::cout << "test " << productOfReactionIterator << std::endl;
				if (productOfReactionIterator == 0) {
					//std::cout << "ifif" << std::endl;
					r1 = v.toS;
					//std::cout << "v.toS " << v.toS -> id << std::endl;

				}
				else{
					r1 = getReaction(productOfReactionIterator);
					//std::cout << "r1: " << r1->id << std::endl;

				}		
			}	
			std::cout << "after if " << std::endl;

			double cost = r1->yield;
			for (auto reactionTailIterator = r1->tail.begin(); reactionTailIterator != r1->tail.end(); ++reactionTailIterator) {
				cost = cost + minCost(*getCompound(*reactionTailIterator), s, overlay);

			}
			if (minimumCost > cost) {
				minimumCost = cost;
				v.minEdge = productOfReactionIterator;
			}
		}
		return minimumCost;
	}

	//Not updated to s use.
	double shortestPathMinimumCost(CompoundNode &v, std::vector<int> &startingCompounds, std::vector<bool> &overlay) {
		CompoundNode *s = new CompoundNode(); //MIGHT NOT BE NEEDED
		s->id = 0;
		auto startingCompoundIterator = startingCompounds.begin();
		while (startingCompoundIterator != startingCompounds.end()) {
			std::vector<int> head;
			head.push_back(*startingCompoundIterator);
			std::vector<int> tail;
			tail.push_back(s->id);
			ReactionNode *r = new ReactionNode(0, head, tail, 0.0); 	
			CompoundNode *c = getCompound(*startingCompoundIterator);
			c->toS = r;
			c->productOfReaction.push_back(r->id);			
			++startingCompoundIterator;
		}
		compoundList.at(s->id) = *s;
		return minCost(v, *s, overlay);
	}*/


	double maxYield(CompoundNode &v, CompoundNode &s, std::vector<bool> &overlay) {
		if (v.id == s.id) {
			return 1.0; //starting compounds always have a yield of 100%
		}
		double maximumYield = 0; // -infinity
		for(auto productOfReactionIterator : v.productOfReaction ) {
			ReactionNode *r1;
			if (overlay.at(productOfReactionIterator)) {
				if (productOfReactionIterator == 0) {
					r1 = v.toS;
				}
				else{
					r1 = getReaction(productOfReactionIterator);
				}		
				double yield = r1->yield;
				for (auto reactionTailIterator : r1->tail) {
					yield = yield * maxYield(*getCompound(reactionTailIterator), s, overlay);
				}
				if (maximumYield < yield) {
					maximumYield = yield;
					v.maxYieldEdge = productOfReactionIterator;
				}
			}
		}
		return maximumYield;
	}

	std::pair < std::vector<bool> , std::pair< double, std::vector<int>> >shortestPathMaximumYield(CompoundNode &v, std::vector<int> &startingCompounds, std::vector<bool> &overlay) {

		CompoundNode *s = new CompoundNode(); //MIGHT NOT BE NEEDED
		s->id = 0;
		auto startingCompoundIterator = startingCompounds.begin();
		while (startingCompoundIterator != startingCompounds.end()) {
			std::vector<int> head;
			head.push_back(*startingCompoundIterator);
			std::vector<int> tail;
			tail.push_back(s->id);
			ReactionNode *r = new ReactionNode(0, head, tail, 1.0); 	
			CompoundNode *c = getCompound(*startingCompoundIterator);
			c->toS = r;
			c->productOfReaction.push_back(r->id);
			++startingCompoundIterator;
		}
		compoundList.at(s->id) = *s;

		double result = maxYield(v, *s, overlay);

		std::vector<int> shortest = getShortestPathYield(v.id);

		std::pair<double, std::vector<int>> pair (result, shortest);
		std::pair<std::vector<bool> , std::pair<double, std::vector<int>> > pair2 (overlay, pair);

		return pair2;
	}

	std::vector< std::vector<bool> > backwardBranching(std::pair < double, std::vector<int> > pair, std::vector<bool> overlay) {
		std::vector< std::vector<bool> > B;

		for(int i = 0; i < pair.second.size(); i++) {

			std::vector<int> toRemove (1,pair.second.at(i)); //remove the i'th edge from t and down.

			for(auto it = toRemove.begin(); it != toRemove.end(); ++it) {
				overlay.at(*it) = false;
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

	std::vector< std::pair < double, std::vector<int>> > yenHyp (CompoundNode &v, std::vector<int> &startingCompounds, std::vector<bool> &overlay, int K) {
		std::vector< std::pair <std::vector<bool> , std::pair<double, std::vector<int> > > > L;
		
		std::make_heap (L.begin(),L.end(), cmpPair());

		std::vector< std::pair < double, std::vector<int> > >A; //list of k shortest hyperpaths.

		std::pair< std::vector<bool> , std::pair<double, std::vector<int>> > pair = shortestPathMaximumYield(v, startingCompounds, overlay);
		L.push_back(pair);

		for ( int k = 0; k < K; k++) {
			if (L.empty()) {
				break;
			}
			pair = L.front(); 	//Get shortest path with max yield and remove from heap
  			std::pop_heap (L.begin(),L.end(), cmpPair() ); 
  			L.pop_back();	

  			std::pair < double, std::vector<int> > toA (pair.second.first, pair.second.second);
  			A.push_back(toA);	//Add current best plan to k-best

  			for (auto newOverlay : backwardBranching(pair.second, pair.first)) { //Potential danger (function call in for loop)
  				std::pair < std::vector<bool> , std::pair<double, std::vector<int>> >potentialNewPlan = shortestPathMaximumYield(v, startingCompounds, newOverlay);
  				
  				if (pathIsComplete(potentialNewPlan.second.first)) {
  					if(pathNotAlreadyFound(potentialNewPlan.second.second, A)){
  						L.push_back(potentialNewPlan); 
						std::push_heap (L.begin(),L.end(), cmpPair());
  					}
  				}
  			}
  		}
  		return A;
	}

	int ShortestHyperNielsen (std::vector<int> &startingCompounds) {
		//Initinalize
		std::vector< CompoundNode > Q;		
		std::make_heap (Q.begin(),Q.end(), cmp());

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
			c->toS = r;
			c->productOfReaction.push_back(r->id);
			s->eductOfReaction.push_back(r->id);
			++startingCompoundIterator;
			//std::cout << "printing " << r->id << "  " << r->head.size() << std::endl;;

		}
		for(auto compound : compoundList) {
			CompoundNode *c = getCompound(compound.id);
			c->weight = 1;
		}
		for(auto reaction : reactionList) {
			ReactionNode *r = getReaction(reaction.id);
			r->kj = 0;
		}
		s->weight = 1;
		compoundList.at(s->id) = *s;
		Q.push_back(*s); 
		std::push_heap (Q.begin(),Q.end(), cmp());
		//Initiliaze done
		while(!Q.empty()) {
			CompoundNode compound = Q.front(); 	//Get node with highest weight
			std::cout << "pulled compound: " << compound.id << " with weight = " << compound.weight << std::endl;

  			std::pop_heap (Q.begin(),Q.end(), cmp() ); 
  			Q.pop_back();	
  			for (auto reaction : compound.eductOfReaction) {
  				ReactionNode *r = getReaction(reaction);
  				r->kj++;
  				std::cout << "reaction: " << r->id << " with kj = " << r->kj << std::endl;

  				if (r->kj == r->tail.size()) {

  					CompoundNode *c = getCompound(*(r->head.begin()));
  					double F = 1;
  					for(auto educt : r->tail) {
  						CompoundNode *temp = getCompound(educt);
 						std::cout << "educt is " << temp->id << " with weight " << temp->weight << std::endl;

  						F = temp->weight * F;
  					}
  					std::cout << "F: " << F << std::endl;
  					if (c->weight >= r->yield *  F) {
  						if(!vectorContainsCompoundNode(Q, *c)) {
  							Q.push_back(*c); 
							std::push_heap (Q.begin(),Q.end(), cmp());
  						}
  						c->weight = r->yield * F;
  						c->maxYieldEdge = reaction;
  					}
  				}
  			}
		}
		std::cout << "Queue is empty" << std::endl;


		return 1;
	}

	bool vectorContainsCompoundNode(std::vector<CompoundNode> v, CompoundNode c) {
		auto iterator = v.begin();
		while (iterator != v.end()) {
			std::cout<<  "print" << std::endl;
			if (iterator->id == c.id) {
				return true;
			}
			++iterator;
		}
		return false;
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

	std::vector<bool> createOverlay(std::vector<int> toRemove) {
		std::vector<bool> overlay;
		overlay.resize(reactionList.size(), true);

		for(auto it : toRemove) {
			overlay.at(it) = false;
		}

		return overlay;
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

	void resetMaxYieldEdges(){
		for (CompoundNode &compound : compoundList) {
			compound.maxYieldEdge = -1;

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
    		std::cout << "Plan: " << i << std::endl;
    		for( auto pathElement : plan.second) {
      			std::cout << "Reaction: " << pathElement << std::endl;
    		}
    		std::cout << "With yield: " << plan.first << "\n" << std::endl;
    		i++;
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

	void graphToGraphviz() {
		std::ofstream graphFile;
		graphFile.open("Graph.gv");
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
			graphFile << "	R" << reaction.id << " -> " << *reaction.head.begin() << ";\n";
    		for( auto tailelement : reaction.tail) {
      			graphFile << "	" << tailelement << " -> R" << reaction.id << ";\n";
    		}
  		} 
		graphFile << "}";
		graphFile.close();
	}
};