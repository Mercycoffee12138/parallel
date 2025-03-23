<<<<<<< HEAD
#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <algorithm>
#include <fstream>  // 用于文件输出
#include <iomanip>  // 用于格式化输出
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
bool verify_result(const vector<double>& result, int n) {
    double expected = 2.0 * n; // 考虑到生成的向量值为2.0
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
    // 为不同大小的矩阵设置不同的重复次数
    auto get_repeat_times = [](int n) {
        if (n <= 100) return 1000;
        if (n <= 500) return 100;
        if (n <= 1000) return 50;
        if (n <= 2000) return 20;
        return 10; // 对于n > 2000
    };
    
    // 创建CSV文件
    ofstream csv_file("matrix_performance.csv");
    csv_file << "矩阵大小,逐列访问时间(ms),按行访问时间(ms),加速比\n";
    
    // 运行从1到3000的每个矩阵大小
    for (int n = 1; n <= 3000; n++) {
        int repeat_times = get_repeat_times(n);
        
        // 显示进度
        if (n % 100 == 0 || n == 1) {
            cout << "处理矩阵大小: " << n << "x" << n << " (重复 " << repeat_times << " 次)" << endl;
        }
        
        // 预先生成测试数据
        auto mat = generate_matrix(n);
        auto vec = generate_vector(n);
        vector<double> result1(n);
        vector<double> result2(n);
        
        // 测试1：逐列访问算法
        double time_naive = measure_time([&]() {
            naive_matrix_vector(mat, vec, result1);
        }, repeat_times);
        
        // 测试2：缓存优化算法
        double time_optimized = measure_time([&]() {
            optimized_matrix_vector(mat, vec, result2);
        }, repeat_times);
        
        // 验证结果
        bool correct1 = verify_result(result1, n);
        bool correct2 = verify_result(result2, n);
        
        if (!correct1 || !correct2) {
            cout << "警告: 大小为 " << n << " 的计算结果可能不正确!" << endl;
        }
        
        // 计算加速比
        double speedup = time_naive / time_optimized;
        
        // 写入CSV文件
        csv_file << n << "," << fixed << setprecision(6) << time_naive << "," 
                 << time_optimized << "," << speedup << "\n";
        
        // 每写入100个数据点刷新文件，确保数据被保存
        if (n % 100 == 0) {
            csv_file.flush();
        }
    }
    
    csv_file.close();
    cout << "\n完成! 结果已保存到 matrix_performance.csv" << endl;
    
    return 0;
=======
#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <algorithm>
#include <fstream>  // 用于文件输出
#include <iomanip>  // 用于格式化输出
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
bool verify_result(const vector<double>& result, int n) {
    double expected = 2.0 * n; // 考虑到生成的向量值为2.0
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
    // 为不同大小的矩阵设置不同的重复次数
    auto get_repeat_times = [](int n) {
        if (n <= 100) return 1000;
        if (n <= 500) return 100;
        if (n <= 1000) return 50;
        if (n <= 2000) return 20;
        return 10; // 对于n > 2000
    };
    
    // 创建CSV文件
    ofstream csv_file("matrix_performance.csv");
    csv_file << "矩阵大小,逐列访问时间(ms),按行访问时间(ms),加速比\n";
    
    // 运行从1到3000的每个矩阵大小
    for (int n = 1; n <= 3000; n++) {
        int repeat_times = get_repeat_times(n);
        
        // 显示进度
        if (n % 100 == 0 || n == 1) {
            cout << "处理矩阵大小: " << n << "x" << n << " (重复 " << repeat_times << " 次)" << endl;
        }
        
        // 预先生成测试数据
        auto mat = generate_matrix(n);
        auto vec = generate_vector(n);
        vector<double> result1(n);
        vector<double> result2(n);
        
        // 测试1：逐列访问算法
        double time_naive = measure_time([&]() {
            naive_matrix_vector(mat, vec, result1);
        }, repeat_times);
        
        // 测试2：缓存优化算法
        double time_optimized = measure_time([&]() {
            optimized_matrix_vector(mat, vec, result2);
        }, repeat_times);
        
        // 验证结果
        bool correct1 = verify_result(result1, n);
        bool correct2 = verify_result(result2, n);
        
        if (!correct1 || !correct2) {
            cout << "警告: 大小为 " << n << " 的计算结果可能不正确!" << endl;
        }
        
        // 计算加速比
        double speedup = time_naive / time_optimized;
        
        // 写入CSV文件
        csv_file << n << "," << fixed << setprecision(6) << time_naive << "," 
                 << time_optimized << "," << speedup << "\n";
        
        // 每写入100个数据点刷新文件，确保数据被保存
        if (n % 100 == 0) {
            csv_file.flush();
        }
    }
    
    csv_file.close();
    cout << "\n完成! 结果已保存到 matrix_performance.csv" << endl;
    
    return 0;
>>>>>>> 815f0540f27cf2fa71b095ed6e225615570a3be0
}