#include "include/databaseConnection.hpp"
#include <iostream>

int main(int argc, char const *argv[])
{
	HyperGraph h = createGraph();

	h.graphToGraphviz("dump");

	return 0;
}