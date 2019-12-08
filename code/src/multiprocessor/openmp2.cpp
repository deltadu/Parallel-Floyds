#include <math.h>
#include <omp.h>
#include <stdlib.h>
#include <iostream>
#include <stdexcept>
#include <vector>
#include "../Graph.h"
#include "../timing.h"
using namespace std;

void parallelFloyd(Graph& graph, const int n, const int b);
void print_result(Graph& graph);

// extern "C" void helper_ispc(Graph& graph, int ar, int ac, int br, int bc, int cr, int cc, const int b, const int n);

#define BLOCK_SIZE 64

int main(int argc, char* argv[]) {
    int numVertices, numberThreads, sqrtOfnumberThreads;
    double t1, t2, elapsedTime;
    Graph graph(argv[1]);

    numVertices = graph.getNumVertices();
    numberThreads = atoi(argv[2]);
    sqrtOfnumberThreads = sqrt(numberThreads);

    omp_set_num_threads(numberThreads);

    double total_time = 0.0;
    int experiment_number = 5;
    for (int i = 0; i < experiment_number; i++) {
        Timer t;
        parallelFloyd(graph, numVertices, BLOCK_SIZE);
        double OpenMPTime = t.elapsed();
        total_time += OpenMPTime;
    }
    // cout << "Result is:" << endl;
    // print_result(graph);
    cout << "Time taken: " << (total_time / experiment_number) << endl;
    return 0;
}


inline void helper(Graph& graph, int ar, int ac, int br, int bc, int cr, int cc, const int b, const int n) {
    for (int k = 0; k < b; k++) {
        for (int i = 0; i < b; i++) {
            for (int j = 0; j < b; j++) {
                int sum = graph.getDist(i+ar, k+ac) + graph.getDist(k + br, j+bc);

                if (graph.getDist(i+cr, j+cc) > sum) {
                    graph.setDist(i+cr, j+cc, sum);
                }
            }
        }
    }
}


void parallelFloyd(Graph& graph, const int n, const int b) {
    const int blocks = n / b;

    for (int k = 0; k < blocks; k++) {
        helper(graph, k*b, k*b, k*b, k*b, k*b, k*b, b, n);

        #pragma omp parallel for
        for (int j = 0; j < blocks; j++) {
            if (j == k) continue;
            helper(graph, k*b, j*b, k*b, k*b, k*b, j*b, b, n);
        }

        #pragma omp parallel for
        for (int i = 0; i < blocks; i++) {
            if (i == k) continue;
            helper(graph, i*b, k*b, i*b, k*b, k*b, k*b, b, n);
            for (int j = 0; j < blocks; j++) {
                if (j == k) continue;
                helper(graph, i*b, j*b, i*b, k*b, k*b, j*b, b, n);
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