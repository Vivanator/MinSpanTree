#include "TreeFunctions.h"

int main()
{
	clock_t tStart = clock(); // Get the start time to we can calculate execution time later.
	std::vector<Edge> edges = readEdges();
	if (edges.size() == 0) {
		std::cout << "File read unsuccessful, please try again." << std::endl;
		return 0;
	}
	std::vector<std::vector<int>> graph;
	graph.resize(edges.size());
	for (int i = 0; i < edges.size(); i++)
		graph[i].resize(edges.size());

	// Creates a 2D array with weights at the edges to represent the graph.
	for (int i = 0; i < edges.size(); i++) {
		graph[edges[i].source-1][edges[i].destination-1] = edges[i].weight;
		graph[edges[i].destination-1][edges[i].source-1] = edges[i].weight;
	}
	int V = edges[0].numEdges;

	// Currently constructed tree
	std::vector<int> parent;
	parent.resize(V);
	std::vector<int> key;
	key.resize(V);
	std::vector<bool> mstSet;
	mstSet.resize(V);

	// All keys have infinite values.
	for (int i = 0; i < V; i++)
		key[i] = INT_MAX, mstSet[i] = false;

	// Always include first 1st vertex in MST. 
	// Make key 0 so that this vertex is picked as first vertex. 
	key[0] = 0;
	parent[0] = -1;

	for (int count = 0; count < V - 1; count++)
	{
		// Find minimum key for the current vertex.
		int u = minKey(key, mstSet, V);

		// Flag the vertex with that key value as being included in the graph
		mstSet[u] = true;

		//
		for (int v = 0; v < V; v++)
			if (graph[u][v] && mstSet[v] == false && graph[u][v] < key[v])
				parent[v] = u, key[v] = graph[u][v];
	}

	std::cout << "Time taken: " << (double)(clock() - tStart) / CLOCKS_PER_SEC * 1000 << "ms" << std::endl;
	std::cout << "Edges and Weights:" << std::endl;
	int weightCount = 0;
	for (int i = 1; i < V; i++) {
		std::cout << parent[i] + 1 << "<->" << i + 1 << "   " << graph[i][parent[i]] << std::endl;
		weightCount += graph[i][parent[i]];
	}
	std::cout << "Total weight count: " << weightCount << std::endl;

    return 0;
}

std::vector<Edge> readEdges() {
	bool flag = false;
	std::cout << "What file would you like to read from?" << std::endl;
	std::string fileName;
	getline(std::cin, fileName);
	std::ifstream infile;
	infile.open(fileName);
	std::vector<Edge> edges;
	if (!infile.fail()) { //If it doesn't fail to open
		int source, destination, weight;
		Edge testEdge;
		infile >> testEdge.numEdges; //Skip first line as it only tells # of nodes
		while (infile >> source >> destination >> weight) {
			flag = false;
			if (weight != 1073741824 && (source != destination)) { //Remove nonexistant nodes and cost from nodes to themselves.
				for (int i = 0; i < edges.size(); i++) { //Check if edge already exists in the list
					if (edges[i].source == destination && edges[i].destination == source) {
						flag = true;
					}
				}
				if (!flag) {
					testEdge.source = source;
					testEdge.destination = destination;
					testEdge.weight = weight;
					edges.push_back(testEdge);
				}
			}
		}
	}
	infile.close();
	return edges; //Failed file reads return empty vector
}

int minKey(std::vector<int> key, std::vector<bool> mstSet, int V) //Returns the smallest key value's index (aka the vertex)
{
	int min = INT_MAX, min_index;
	for (int v = 0; v < V; v++) {
		if (mstSet[v] == false && key[v] < min) {
			min = key[v], min_index = v;
		}
	}
	return min_index;
}




