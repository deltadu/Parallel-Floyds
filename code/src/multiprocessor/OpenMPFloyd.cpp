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

    cout << "Result is:" << endl;
    print_result(graph);
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

#pragma omp parallel shared(                                      \
    graph, numVertices, sqrtOfnumberThreads,            \
    blockSize) firstprivate(rowK, colK)
        {
		int threadNumber = omp_get_thread_num();

		int startRow = (threadNumber / sqrtOfnumberThreads) * blockSize;
		int lastRow = startRow + blockSize;

		int startCol = (threadNumber % sqrtOfnumberThreads) * blockSize;
		int lastCol = startCol + blockSize;

		for (int k = 0; k < numVertices; k++) {
			#pragma omp barrier
			for (int i = 0; i < numVertices; ++i) {
				rowK[i] = graph.getDist(k, i);
				colK[i] = graph.getDist(i, k);
			}
			#pragma omp barrier
			for (int i = startRow; i < lastRow; ++i) {
				int distItoK = colK[i];
				for (int j = startCol; j < lastCol; ++j) {
					if (i != j && i != k && j != k) {
						int distItoKtoJ = distItoK + rowK[j];
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