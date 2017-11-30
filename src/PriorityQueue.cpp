#include <iostream>
#include <vector>
#include <math.h>
#include "include/CompoundNode.hpp"

struct PriorityQueue
{

private:
	std::vector<CompoundNode> heap;
	
	int left(int i) {
		return 2*i;
	}

	int right(int i) {
		return 2*i+1;
	}

	int parent(int i) {
		return floor(i/2);
	}

public:

	PriorityQueue() {
		heap.push_back(CompoundNode(0));
	}

	void heapify(int index) {
		int l = left(index);
		int r = right(index);
		int largest = 1;
		if (l <= heap.size()-1 && heap[l].cost < heap[index].cost) {
			largest = l;
		}
		else {
			largest = index;
		}
		if (r <= heap.size()-1 && heap[r].cost < heap[largest].cost) {
			largest = r;
		}
		if (largest != index) {
			CompoundNode temp = heap[largest];
			heap[largest] = heap[index];
			heap[largest].index = largest;
			heap[index] = temp;
			heap[index].index = index;
			heapify(largest);
		}
	}

	void push(CompoundNode &compound) {
		heap.push_back(compound);
		compound.index = heap.size()-1;
		decrease_key(heap.size()-1, compound);
	}

	CompoundNode pop(){
		if (heap.size() < 1){
			std::cout << "Error, Heap underflow" << std::endl;
		}
		CompoundNode c = heap[1];
		heap[1] = heap[heap.size()-1];
		heap.pop_back();
		heapify(1);
		return c;
	}

	void decrease_key(int index, CompoundNode &compound){
		if (compound.cost > heap[index].cost){
			std::cout << "Error: Key is not smaller" << std::endl;
		}
		heap[index] = compound;
		while(index > 1 && heap[parent(index)].cost > heap[index].cost) {
			CompoundNode temp = heap[parent(index)];
			heap[parent(index)] = heap[index];
			heap[parent(index)].index = parent(index);
			heap[index] = temp;
			heap[index].index = index;
			index = parent(index);
		}
	}

	bool contains(CompoundNode &compound) {
		if (compound.index == -1) {
			return false;
		}
		if (compound.index > heap.size()-1) {
			return false;
		}
		return true;
	}

	bool empty(){
		if(heap.size() > 1) {
			return false;
		}
		return true;
	}

};

int main(int argc, char const *argv[])
{
	PriorityQueue Q;
	std::cout << "Test" << std::endl;
	for(int i = 0; i < 10 ; i++) {
		auto p = new CompoundNode(i);
		p->cost = i*2;
		Q.push(*p);
	}
	auto p = new CompoundNode(12);
	p->cost = 9;
	Q.push(*p);
	p->cost = 2;
	auto q = new CompoundNode(15);
	std::cout << std::boolalpha << Q.contains(*p) << std::endl;
	std::cout << std::boolalpha << Q.contains(*q) << std::endl;
	Q.decrease_key(p->index, *p);
	while (!Q.empty()) {
		CompoundNode c = Q.pop();
		std::cout << c.id << " with cost " << c.cost << std::endl;;
	}
	std::cout << std::boolalpha << Q.contains(*p) << std::endl;
	return 0;
}