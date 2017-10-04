#include "include/databaseConnection.hpp"
#include <iostream>

int main(int argc, char const *argv[])
{
	HyperGraph h = createGraph(6000000, 1000000);

	h.graphToGraphviz("dump");

	return 0;
}