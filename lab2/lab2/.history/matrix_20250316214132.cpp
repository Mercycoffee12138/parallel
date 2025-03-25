#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <algorithm>
using namespace std;

// void naive_matrix_vector(const vector<vector<double>>& matrix,
                        const vector<double>& vec,
                        vector<double>& result) {
    int n = matrix.size();
    for (int col = 0; col < n; col++) {
        double sum = 0.0;
        for (int row = 0; row < n; row++) {
            sum += matrix[row][col] * vec[row]; // 按列访问
        }
        result[col] = sum;
    }
}
void naive_matrix_vector(const vector<vector<double>>& matrix,
                        const vector<double>& vec,
                        vector<double>& result) {
    int n = matrix.size();
    for (int col = 0; col < n; col++) {
        double sum = 0.0;
        for (int row = 0; row < n; row++) {
            sum += matrix[row][col] * vec[row]; // 按列访问
        }
        result[col] = sum;
    }
}

// 优化算法：按行访问（缓存友好）
void optimized_matrix_vector(const vector<vector<double>>& matrix,
                           const vector<double>& vec,
                           vector<double>& result) {
    int n = matrix.size();
    fill(result.begin(), result.end(), 0.0);
    for (int row = 0; row < n; row++) {
        double vec_val = vec[row]; // 缓存向量元素
        for (int col = 0; col < n; col++) {
            result[col] += matrix[row][col] * vec_val; // 按行访问
        }
    }
}

// 生成全1矩阵和向量
vector<vector<double>> generate_matrix(int n) {
    return vector<vector<double>>(n, vector<double>(n, 1.0));
}

vector<double> generate_vector(int n) {
    return vector<double>(n, 2.0);
}

// 验证结果
bool verify_result(const vector<double>& result, int n) {
    for (int i = 0; i < min(5, n); i++) {
        if (abs(result[i] - n) > 1e-6) {
            return false;
        }
    }
    return true;
}

// 测量函数运行时间（可重复指定次数）
double measure_time(function<void()> func, int repeat_times) {
    auto total_start = chrono::high_resolution_clock::now();
    
    for (int i = 0; i < repeat_times; i++) {
        func();
    }
    
    auto total_end = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::microseconds>(total_end - total_start).count() / 1000.0 / repeat_times;
}

int main() {
    int n = 1000; // 测试矩阵大小
    int repeat_times = 3000; // 设置重复执行次数
    
    cout << "矩阵大小: " << n << "x" << n << endl;
    cout << "每个算法重复执行次数: " << repeat_times << endl;
    cout << "-----------------------------------" << endl;
    
    // 预先生成测试数据
    auto mat = generate_matrix(n);
    auto vec = generate_vector(n);
    vector<double> result1(n);
    vector<double> result2(n);
    
    // 测试1：逐列访问元素的平凡算法
    double time_naive = measure_time([&]() {
        naive_matrix_vector(mat, vec, result1);
    }, repeat_times);
    
    cout << "1. 逐列访问算法: " << time_naive << " ms";
    cout << (verify_result(result1, n) ? " [正确]" : " [错误]") << endl;
    
    // 测试2：缓存优化算法（按行访问）
    double time_optimized = measure_time([&]() {
        optimized_matrix_vector(mat, vec, result2);
    }, repeat_times);
    
    cout << "2. 缓存优化算法: " << time_optimized << " ms";
    cout << (verify_result(result2, n) ? " [正确]" : " [错误]") << endl;
    
    // 计算加速比
    double speedup = time_naive / time_optimized;
    
    cout << "-----------------------------------" << endl;
    cout << "加速比: " << speedup << "x" << endl;
    cout << "实验结论: 缓存优化的按行访问算法利用了空间局部性，";
    cout << "显著提高了内存访问效率" << endl;
    
    return 0;
}