#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <arm_neon.h>  // ARM NEON 指令集头文件
using namespace std;

// 原始版本：按列访问（缓存不友好）
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

// 按行访问的缓存友好版本
void row_optimized_matrix_vector(const vector<vector<double>>& matrix,
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

// ARM NEON优化版本
void neon_matrix_vector(const vector<vector<double>>& matrix,
                       const vector<double>& vec,
                       vector<double>& result) {
    int n = matrix.size();
    fill(result.begin(), result.end(), 0.0);
    
    // 对于较大的矩阵，使用NEON向量化
    for (int row = 0; row < n; row++) {
        double vec_val = vec[row]; // 缓存向量元素值
        float64x2_t vec_val_neon = vdupq_n_f64(vec_val); // 复制向量元素到NEON寄存器
        
        // 每次处理2个元素（NEON双精度寄存器宽度）
        int col = 0;
        for (; col <= n - 2; col += 2) {
            float64x2_t matrix_row = vld1q_f64(&matrix[row][col]); // 加载2个矩阵元素
            float64x2_t result_vec = vld1q_f64(&result[col]); // 加载当前结果
            result_vec = vmlaq_f64(result_vec, matrix_row, vec_val_neon); // 向量乘加
            vst1q_f64(&result[col], result_vec); // 存储结果
        }
        
        // 处理剩余元素
        for (; col < n; col++) {
            result[col] += matrix[row][col] * vec_val;
        }
    }
}

// 使用预取指令的优化版本
void prefetch_matrix_vector(const vector<vector<double>>& matrix,
                          const vector<double>& vec,
                          vector<double>& result) {
    int n = matrix.size();
    fill(result.begin(), result.end(), 0.0);
    
    for (int row = 0; row < n; row++) {
        double vec_val = vec[row];
        
        // 如果不是最后一行，预取下一行数据
        if (row + 1 < n) {
            __builtin_prefetch(&matrix[row + 1][0], 0, 3); // 预取读取，高时间局部性
        }
        
        for (int col = 0; col < n; col++) {
            result[col] += matrix[row][col] * vec_val;
        }
    }
}

// 生成测试矩阵和向量
vector<vector<double>> generate_matrix(int n) {
    return vector<vector<double>>(n, vector<double>(n, 1.0));
}

vector<double> generate_vector(int n) {
    return vector<double>(n, 2.0);
}

// 验证计算结果
bool verify_result(const vector<double>& result, int n) {
    double expected = 2.0 * n;
    for (int i = 0; i < min(5, n); i++) {
        if (abs(result[i] - expected) > 1e-6) {
            return false;
        }
    }
    return true;
}

// 测量执行时间
double measure_time(function<void()> func, int repeat_times) {
    auto total_start = chrono::high_resolution_clock::now();
    
    for (int i = 0; i < repeat_times; i++) {
        func();
    }
    
    auto total_end = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::microseconds>(total_end - total_start).count() / 1000.0 / repeat_times;
}

int main() {
    // 获取CPU信息（ARM平台）
    cout << "ARM平台矩阵向量乘法性能测试" << endl;
    system("cat /proc/cpuinfo | grep 'model name' | head -1");
    
    // 设置测试参数
    auto get_repeat_times = [](int n) {
        if (n <= 100) return 1000;
        if (n <= 500) return 100;
        if (n <= 1000) return 50;
        if (n <= 2000) return 20;
        return 10;
    };
    
    // 创建CSV文件
    ofstream csv_file("arm_matrix_performance.csv");
    csv_file << "矩阵大小,原始算法(ms),行优化算法(ms),NEON优化(ms),预取优化(ms),NEON加速比,预取加速比\n";
    
    // 测试不同矩阵大小
    vector<int> sizes = {100, 200, 500, 1000, 1500, 2000, 2500, 3000};
    for (int n : sizes) {
        int repeat_times = get_repeat_times(n);
        
        cout << "处理矩阵大小: " << n << "x" << n << " (重复 " << repeat_times << " 次)" << endl;
        
        // 生成测试数据
        auto mat = generate_matrix(n);
        auto vec = generate_vector(n);
        vector<double> result1(n), result2(n), result3(n), result4(n);
        
        // 测试各种实现
        double time_naive = measure_time([&]() {
            naive_matrix_vector(mat, vec, result1);
        }, repeat_times);
        
        double time_row = measure_time([&]() {
            row_optimized_matrix_vector(mat, vec, result2);
        }, repeat_times);
        
        double time_neon = measure_time([&]() {
            neon_matrix_vector(mat, vec, result3);
        }, repeat_times);
        
        double time_prefetch = measure_time([&]() {
            prefetch_matrix_vector(mat, vec, result4);
        }, repeat_times);
        
        // 验证结果
        bool all_correct = verify_result(result1, n) && 
                          verify_result(result2, n) && 
                          verify_result(result3, n) &&
                          verify_result(result4, n);
        
        if (!all_correct) {
            cout << "警告: 大小为 " << n << " 的某些计算结果不正确!" << endl;
        }
        
        // 计算加速比
        double neon_speedup = time_naive / time_neon;
        double prefetch_speedup = time_naive / time_prefetch;
        
        // 写入CSV
        csv_file << n << "," << fixed << setprecision(6) 
                << time_naive << "," 
                << time_row << "," 
                << time_neon << "," 
                << time_prefetch << ","
                << neon_speedup << ","
                << prefetch_speedup << "\n";
        
        csv_file.flush();
        
        // 输出当前结果
        cout << "  原始算法: " << time_naive << " ms" << endl;
        cout << "  行优化算法: " << time_row << " ms" << endl;
        cout << "  NEON优化: " << time_neon << " ms (加速比: " << neon_speedup << ")" << endl;
        cout << "  预取优化: " << time_prefetch << " ms (加速比: " << prefetch_speedup << ")" << endl;
    }
    
    csv_file.close();
    cout << "\n完成! 结果已保存到 arm_matrix_performance.csv" << endl;
    
    return 0;
}