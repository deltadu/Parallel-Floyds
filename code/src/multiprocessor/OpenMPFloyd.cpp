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
    int numVertices = graph.getNumVertices();
    int sqrtOfnumberThreads = sqrt(omp_get_max_threads());
    int blockSize = numVertices / sqrtOfnumberThreads;
    vector<int> rowK(numVertices);
    vector<int> colK(numVertices);
    int minlen;
    int i, j, k;
    int distItoK, distItoKtoJ;
    int startRow, lastRow, startCol, lastCol;
    int threadNumber;

#pragma omp parallel shared(graph, numVertices, sqrtOfnumberThreads, blockSize) private(i, j, k, startRow, lastRow, startCol, lastCol, distItoKtoJ, distItoK, threadNumber) firstprivate(rowK, colK)
        {
        threadNumber = omp_get_thread_num();

        startRow = (threadNumber / sqrtOfnumberThreads) * blockSize;
        lastRow = startRow + blockSize;

        startCol = (threadNumber % sqrtOfnumberThreads) * blockSize;
        lastCol = startCol + blockSize;

        for (k = 0; k < numVertices; k++) {
            #pragma omp barrier
            for (i = 0; i < numVertices; ++i) {
                rowK[i] = graph.getDist(k, i);
                colK[i] = graph.getDist(i, k);
            }
            #pragma omp barrier
            for (i = startRow; i < lastRow; ++i) {
                distItoK = colK[i];
                for (j = startCol; j < lastCol; ++j) {
                    if (i != j && i != k && j != k) {
                        distItoKtoJ = distItoK + rowK[j];
                        minlen = min(distItoKtoJ, graph.getDist(i, j));
                        graph.setDist(i, j, minlen);
                    }
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