#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <chrono>  // For measuring execution time

#define N 500  // Matrix size (Can be modified)
#define NUM_THREADS 4  // Number of threads

int A[N][N], B[N][N], C[N][N];  // Matrices

// Structure for passing data to threads
struct ThreadData {
    int start, end;
};

// Function to perform matrix multiplication for assigned rows
void* multiplyPart(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int start = data->start;
    int end = data->end;

    for (int i = start; i < end; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0;
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    pthread_exit(NULL);
}

// Function to execute parallel matrix multiplication
void parallelMatrixMultiplication() {
    pthread_t threads[NUM_THREADS];
    ThreadData threadData[NUM_THREADS];
    int rowsPerThread = N / NUM_THREADS;

    for (int i = 0; i < NUM_THREADS; i++) {
        threadData[i].start = i * rowsPerThread;
        threadData[i].end = (i == NUM_THREADS - 1) ? N : (i + 1) * rowsPerThread;
        pthread_create(&threads[i], NULL, multiplyPart, (void*)&threadData[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
}

// Function to generate random matrices
void generateRandomMatrix(int matrix[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = rand() % 10;  // Values between 0-9
        }
    }
}

int main() {
    // Generate random matrices
    generateRandomMatrix(A);
    generateRandomMatrix(B);

    // Measure execution time
    auto start = std::chrono::high_resolution_clock::now();

    // Perform parallel matrix multiplication
    parallelMatrixMultiplication();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    // Print execution time only
    std::cout << "Execution Time: " << duration.count() << " seconds" << std::endl;

    return 0;
}
