#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include "../timing.h"
#include "../Graph.h"
using namespace std;

void sequentialFloyd(Graph& graph);
void print_result(Graph& graph);
int main(int argc, char* argv[]) {
    string fileName = argv[1];
    Graph graph(fileName);

    int numVertices = graph.getNumVertices();
    cout << "Graph contains " << numVertices << " vertices." << endl;

    Timer t;
    sequentialFloyd(graph);
    double sequentialTime = t.elapsed();
    t.reset();

    cout << "Result is:" << endl;
    print_result(graph);
    cout << "Time taken = " << sequentialTime << endl;
    return 0;
}

void sequentialFloyd(Graph& graph) {
    int lenIKJ;
    int numVertices = graph.getNumVertices();

    for (int k = 0; k < numVertices; k++) {
        for (int i = 0; i < numVertices; i++) {
            for (int j = 0; j < numVertices; j++) {
                if (i != j && i != k && j != k) {
                    lenIKJ =
                        graph.getDist(i, k) +
                        graph.getDist(k, j);
                    graph.setDist(
                        i, j,
                        min(lenIKJ, graph.getDist(i, j)));
                }
            }
        }
    }
    
}

void print_result(Graph& graph) {
    int numVertices = graph.getNumVertices();

    for (int i = 0; i < numVertices; ++i) {
        cout << "from vertex " << i << ": ";

        for (int j = 0; j < numVertices; ++j) {
            if (graph.getDist(i, j) == POSITIVE_INF) {
                cout << "inf";
            } else {
                cout << graph.getDist(i, j);
            }
            if (j < (numVertices - 1)) {
                cout << ",";
            } else {
                cout << std::endl;
            }
        }
    }
    return;
}
