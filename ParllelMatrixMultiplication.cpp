#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <chrono>
#include <pthread.h>

using namespace std;
using namespace std::chrono;

#define N 500  
#define NUM_THREADS 4  // Change this to test different thread counts

vector<vector<int>> A, B, C;
pthread_t threads[NUM_THREADS];

struct ThreadData {
    int start, end;
};

void* multiplyPart(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int start = data->start, end = data->end;

    for (int i = start; i < end; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    pthread_exit(NULL);
}

void parallelMatrixMultiplication() {
    ThreadData threadData[NUM_THREADS];
    int rowsPerThread = N / NUM_THREADS;
    
    for (int i = 0; i < NUM_THREADS; i++) {
        threadData[i].start = i * rowsPerThread;
        threadData[i].end = (i == NUM_THREADS - 1) ? N : (i + 1) * rowsPerThread;
        pthread_create(&threads[i], NULL, multiplyPart, (void*)&threadData[i]);
    }
    
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);
}

int main() {
    srand(time(0));

    A = vector<vector<int>>(N, vector<int>(N));
    B = vector<vector<int>>(N, vector<int>(N));
    C = vector<vector<int>>(N, vector<int>(N, 0));

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            A[i][j] = B[i][j] = rand() % 10;

    auto start = high_resolution_clock::now();
    parallelMatrixMultiplication();
    auto end = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>(end - start);
    cout << "Pthreads Execution Time with " << NUM_THREADS << " threads: " << duration.count() << " ms" << endl;

    writeMatrixToFile(C, "output_pthreads.txt");

    return 0;
}
