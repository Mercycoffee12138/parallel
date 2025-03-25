#include <iostream>
#include <vector>
#include <chrono>

const int N = 1000; // 矩阵大小
using namespace std;

// 初始化矩阵
void initializeMatrix(vector<vector<double>>& matrix) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            matrix[i][j] = rand() % 100;
        }
    }
}

// 按行计算矩阵乘法
void multiplyByRow(const vector<vector<double>>& A, const vector<vector<double>>& B, vector<vector<double>>& C) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            for (int k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// 按列计算矩阵乘法
void multiplyByColumn(const vector<vector<double>>& A, const vector<vector<double>>& B, vector<vector<double>>& C) {
    for (int j = 0; j < N; ++j) {
        for (int i = 0; i < N; ++i) {
            for (int k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    vector<vector<double>> A(N, vector<double>(N));
    vector<vector<double>> B(N, vector<double>(N));
    vector<vector<double>> C(N, vector<double>(N, 0));

    initializeMatrix(A);
    initializeMatrix(B);

    // 测试按行计算
    auto start = chrono::high_resolution_clock::now();
    multiplyByRow(A, B, C);
    auto end = chrono::high_resolution_clock::now();
    cout << "Row-wise multiplication time: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;

    // 清零结果矩阵
    fill(C.begin(), C.end(), vector<double>(N, 0));

    // 测试按列计算
    start = chrono::high_resolution_clock::now();
    multiplyByColumn(A, B, C);
    end = chrono::high_resolution_clock::now();
    cout << "Column-wise multiplication time: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;

    return 0;
}
