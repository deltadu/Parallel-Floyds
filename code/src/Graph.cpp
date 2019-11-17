#include "Graph.h"

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>


Graph::Graph(const std::string& filename) {
	std::ifstream infile(filename.c_str());
	if (infile) {
		initGraph(infile);
		return;
	}
}

void Graph::initGraph(std::ifstream& infile) {
    char buffer[MAX_LEN];
    infile.getline(buffer, MAX_LEN, '\n');
    numVertices = (int)atoi(buffer);

    graph.resize(numVertices * numVertices);

    for (int i = 0; i < numVertices; i++) {
        int currentRowShift = i * numVertices;
        for (int j = 0; j < numVertices; j++) {
            if (i == j) {
                graph[currentRowShift + j] = 0;
            } else {
                graph[currentRowShift + j] = POSITIVE_INF;
            }
        }
	}

	while (infile.getline(buffer, MAX_LEN) && infile.good() &&
			!infile.eof()) {
		char* sourceVerticeString = buffer;
		char* destVerticeString = strpbrk(sourceVerticeString, " \t");
		*destVerticeString++ = '\0';
		char* distString = strpbrk(destVerticeString, " \t");
		*distString++ = '\0';
		int dist = atoi(distString);
		int sourceVertice = atoi(sourceVerticeString);
		int destVertice = atoi(destVerticeString);
		graph[sourceVertice * numVertices + destVertice] = dist;
	}
}

Graph::~Graph() {}

void Graph::setDist(const int& sourceVertice,
		const int& destVertice, const int& dist) {
	graph[sourceVertice * numVertices + destVertice] = dist;
}

int Graph::getDist(const int& sourceVertice,
		const int& destVertice) const {
	return graph[sourceVertice * numVertices + destVertice];
}