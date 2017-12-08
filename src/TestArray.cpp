#include "include/HyperGraph.hpp"
#include <iostream>
int main(int argc, char const *argv[])
{
	std::vector<CompoundNode*> v;
	std::vector<CompoundNode*> w;
	CompoundNode *s = new CompoundNode(0);
	v.push_back(s);
	w.push_back(s);
	s = new CompoundNode(1);
	v.push_back(s);
	w.push_back(s);

	std::cout << "v: " << v[0]->id << " and w: " << w[0]->id <<std::endl;
	std::cout << "v: " << v[1]->id << " and w: " << w[1]->id <<std::endl;

	s->id = 2;

	std::cout << "v: " << v[0]->id << " and w: " << w[0]->id <<std::endl;
	std::cout << "v: " << v[1]->id << " and w: " << w[1]->id <<std::endl;

	for(auto c : v) {
		c->id++;
	}

	std::cout << "v: " << v[0]->id << " and w: " << w[0]->id <<std::endl;
	std::cout << "v: " << v[1]->id << " and w: " << w[1]->id <<std::endl;
}
