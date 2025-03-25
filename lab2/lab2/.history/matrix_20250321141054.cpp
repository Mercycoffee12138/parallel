#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <functional>
using namespace std;

// 平凡算法：逐列访问（缓存不友好）
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
bool verify_result(const vector<double>& result, double expected) {
    int n = result.size();
    for (int i = 0; i < min(5, n); i++) {
        if (abs(result[i] - expected) > 1e-6) {
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
    // 创建CSV文件
    ofstream csv_file("matrix_performance_1_to_1500.csv");
    csv_file << "矩阵大小,重复次数,平凡算法时间(ms),优化算法时间(ms),加速比\n";
    
    cout << "开始测试从1到1500的所有矩阵大小..." << endl;
    
    // 设置每隔多少个数据显示一次进度
    int progress_interval = 50;
    
    // 测试从1到1500的所有整数
    for (int n = 1; n <= 1500; n++) {
        // 根据矩阵大小动态调整重复次数
        int repeat_times;
        if (n < 100) {
            repeat_times = 3000;
        } else if (n < 1000) {
            repeat_times = 300;
        } else {
            repeat_times = 20;
        }
        
        // 每隔一定间隔显示进度
        if (n % progress_interval == 0 || n == 1 || n == 1500) {
            cout << "测试矩阵大小: " << n << "x" << n 
                 << " (" << (n * 100 / 1500) << "% 完成)" << endl;
        }
        
        // 预先生成测试数据
        auto mat = generate_matrix(n);
        auto vec = generate_vector(n);
        vector<double> result1(n);
        vector<double> result2(n);
        
        // 测试1：逐列访问元素的平凡算法
        double time_naive = measure_time([&]() {
            naive_matrix_vector(mat, vec, result1);
        }, repeat_times);
        
        // 测试2：缓存优化算法（按行访问）
        double time_optimized = measure_time([&]() {
            optimized_matrix_vector(mat, vec, result2);
        }, repeat_times);
        
        // 验证结果正确性 (结果应为n*2，因为向量元素为2.0)
        bool naive_correct = verify_result(result1, n * 2.0);
        bool optimized_correct = verify_result(result2, n * 2.0);
        
        // 计算加速比
        double speedup = time_naive / time_optimized;
        
        // 写入CSV
        csv_file << n << "," << repeat_times << "," 
                 << time_naive << "," << time_optimized << "," 
                 << speedup << "\n";
        
        // 每100个数据点刷新一次文件
        if (n % 100 == 0) {
            csv_file.flush();
        }
        
        // 如果结果不正确，输出警告
        if (!naive_correct || !optimized_correct) {
            cout << "警告: 矩阵大小 " << n << " 的计算结果不正确!" << endl;
        }
    }
    
    csv_file.close();
    cout << "测试完成! 结果已保存到 matrix_performance_1_to_1500.csv" << endl;
    
    return 0;
}