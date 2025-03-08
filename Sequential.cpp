#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <chrono>

using namespace std;
using namespace std::chrono;

#define N 500  // Change N for different matrix sizes

vector<vector<int>> generateMatrix(int size) {
    vector<vector<int>> matrix(size, vector<int>(size));
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            matrix[i][j] = rand() % 10;  // Random values between 0-9
    return matrix;
}

vector<vector<int>> multiplyMatrices(const vector<vector<int>>& A, const vector<vector<int>>& B, int size) {
    vector<vector<int>> C(size, vector<int>(size, 0));
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            for (int k = 0; k < size; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return C;
}

void writeMatrixToFile(const vector<vector<int>>& matrix, const string& filename) {
    ofstream file(filename);
    if (file.is_open()) {
        for (const auto& row : matrix) {
            for (int value : row)
                file << value << " ";
            file << endl;
        }
        file.close();
    }
}

int main() {
    srand(time(0));

    vector<vector<int>> A = generateMatrix(N);
    vector<vector<int>> B = generateMatrix(N);

    auto start = high_resolution_clock::now();
    vector<vector<int>> C = multiplyMatrices(A, B, N);
    auto end = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>(end - start);
    cout << "Sequential Execution Time: " << duration.count() << " ms" << endl;

    writeMatrixToFile(C, "output_sequential.txt");

    return 0;
}
