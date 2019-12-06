#include <math.h>
#include <omp.h>
#include <stdlib.h>
#include <iostream>
#include <stdexcept>
#include <vector>
#include "../Graph.h"
#include "../timing.h"
using namespace std;

void parallelFloyd(Graph& graph);
void print_result(Graph& graph);

int main(int argc, char* argv[]) {
    int numVertices, numberThreads, sqrtOfnumberThreads;
    double t1, t2, elapsedTime;
    Graph graph(argv[1]);

    numVertices = graph.getNumVertices();
    numberThreads = atoi(argv[2]);
    sqrtOfnumberThreads = sqrt(numberThreads);

    if (sqrt(numberThreads) != (double)sqrtOfnumberThreads ||
        numVertices % sqrtOfnumberThreads != 0) {
        throw new runtime_error(
            "Number of vertices must be a multiple of the square root of the "
            "number of threads");
    }

    omp_set_num_threads(numberThreads);

    Timer t;
    parallelFloyd(graph);
    double OpenMPTime = t.elapsed();

    // cout << "Result is:" << endl;
    // print_result(graph);
    cout << "Time taken: " << OpenMPTime << endl;
    return 0;
}

void parallelFloyd(Graph& graph) {
    int lenIKJ;
    int numVertices = graph.getNumVertices();

    for (int k = 0; k < numVertices; k++) {
        #pragma omp parallel num_threads(4)
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