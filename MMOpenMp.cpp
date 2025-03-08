#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <omp.h>

using namespace std;
using namespace std::chrono;

#define N 500  // Define the size of the matrix

// Function to generate a random matrix
vector<vector<int>> generateMatrix(int size) {
    vector<vector<int>> matrix(size, vector<int>(size));
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            matrix[i][j] = rand() % 10;  // Random values between 0 and 9
    return matrix;
}

// Parallel matrix multiplication using OpenMP
vector<vector<int>> multiplyMatrices(const vector<vector<int>>& A, const vector<vector<int>>& B, int size) {
    vector<vector<int>> C(size, vector<int>(size, 0));

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            for (int k = 0; k < size; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return C;
}

int main() {
    srand(time(0));

    // Generate two random matrices
    vector<vector<int>> A = generateMatrix(N);
    vector<vector<int>> B = generateMatrix(N);

    // Measure execution time
    auto start = high_resolution_clock::now();
    vector<vector<int>> C = multiplyMatrices(A, B, N);
    auto end = high_resolution_clock::now();

    // Compute and display execution time
    auto duration = duration_cast<milliseconds>(end - start);
    cout << "OpenMP Execution Time: " << duration.count() << " ms" << endl;

    return 0;
}
