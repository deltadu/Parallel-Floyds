#ifndef GRAPH_H
#define GRAPH_H
#include <vector>
#include <iostream>

const int MAX_LEN = 100;
const int POSITIVE_INF = 65536;

class Graph {
public:
	int getDist(const int& sourceVertice, const int& destVertice) const;
    void setDist(const int& sourceVertice, const int& destVertice,
                 const int& dist);
    Graph(const std::string& fileName);
    inline int getNumVertices() const { return this->numVertices; }
    ~Graph();

   private:
    int numVertices;
    std::vector<int> graph;
    void initGraph(std::ifstream& infile);
};

#endif