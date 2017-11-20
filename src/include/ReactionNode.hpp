struct ReactionNode
	{
		ReactionNode(int id, std::vector<int> head, std::vector<int> tail, double yield) : id(id), head(head), tail(tail), yield(yield) {};
		int id;
		double yield;	
		int kj;
		std::vector<int> head;
		std::vector<int> tail;
	};