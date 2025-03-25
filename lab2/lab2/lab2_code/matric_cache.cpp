<<<<<<< HEAD
#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <functional>
#include <iomanip>
using namespace std;

// 保留原有代码...
// 测量函数运行时间（可重复指定次数）
double measure_time(function<void()> func, int repeat_times) {
    auto total_start = chrono::high_resolution_clock::now();
    
    for (int i = 0; i < repeat_times; i++) {
        func();
    }
    
    auto total_end = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::microseconds>(total_end - total_start).count() / 1000.0 / repeat_times;
}

// 生成矩阵和向量的函数
vector<vector<double>> generate_matrix(int n) {
    return vector<vector<double>>(n, vector<double>(n, 1.0));
}

vector<double> generate_vector(int n) {
    return vector<double>(n, 2.0);
}

// 验证结果正确性的函数
bool verify_result(const vector<double>& result, double expected) {
    int n = result.size();
    for (int i = 0; i < min(5, n); i++) {
        if (abs(result[i] - expected) > 1e-6) {
            return false;
        }
    }
    return true;
}

// 新增：缓存行对齐的矩阵表示
struct CacheAlignedMatrix {
    vector<double> data;
    int n;
    
    CacheAlignedMatrix(int size) : n(size) {
        data.resize(size * size, 1.0);
    }
    
    double get(int row, int col) const {
        return data[row * n + col]; // 行主序
    }
};

// 新增：按列访问缓存对齐矩阵（不友好）
void naive_column_access(const CacheAlignedMatrix& matrix, 
                        const vector<double>& vec,
                        vector<double>& result) {
    int n = matrix.n;
    for (int col = 0; col < n; col++) {
        double sum = 0.0;
        for (int row = 0; row < n; row++) {
            sum += matrix.get(row, col) * vec[row]; // 按列访问
        }
        result[col] = sum;
    }
}

// 新增：按行访问缓存对齐矩阵（友好）
void optimized_row_access(const CacheAlignedMatrix& matrix, 
                         const vector<double>& vec,
                         vector<double>& result) {
    int n = matrix.n;
    fill(result.begin(), result.end(), 0.0);
    for (int row = 0; row < n; row++) {
        double vec_val = vec[row];
        for (int col = 0; col < n; col++) {
            result[col] += matrix.get(row, col) * vec_val; // 按行访问
        }
    }
}

// 新增：特定步长访问实验
void stride_access_experiment(const CacheAlignedMatrix& matrix, 
                            const vector<double>& vec,
                            vector<double>& result,
                            int stride) {
    int n = matrix.n;
    fill(result.begin(), result.end(), 0.0);
    
    // 使用特定步长访问内存
    for (int row = 0; row < n; row += stride) {
        double vec_val = vec[row];
        for (int col = 0; col < n; col++) {
            result[col] += matrix.get(row, col) * vec_val;
        }
    }
    
    // 处理漏掉的行
    for (int row = 0; row < n; row++) {
        if (row % stride != 0) {
            double vec_val = vec[row];
            for (int col = 0; col < n; col++) {
                result[col] += matrix.get(row, col) * vec_val;
            }
        }
    }
}

int main() {
    // 既保留原有测试，又添加缓存分析
    
    // 创建CSV文件
    ofstream csv_file("matrix_cache_analysis.csv");
    csv_file << "矩阵大小,列访问时间(ms),行访问时间(ms),加速比,"
             << "内存占用(KB),L1缓存倍数,L2缓存倍数,L3缓存倍数\n";
    
    cout << "开始缓存影响分析实验..." << endl;
    
    // 假设的缓存大小 (根据您的CPU调整)
    const int L1_CACHE_KB = 32;    // 32KB L1数据缓存
    const int L2_CACHE_KB = 256;   // 256KB L2缓存
    const int L3_CACHE_KB = 8192;  // 8MB L3缓存
    
    // 指数增长的矩阵大小，更好地观察缓存边界
    vector<int> test_sizes;
    for (int size = 16; size <= 4096; size *= 2) {
        test_sizes.push_back(size);
    }
    
    // 在缓存边界附近添加更多测试点
    for (double factor : {0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75, 2.0}) {
        int l1_boundary_test = static_cast<int>(sqrt(L1_CACHE_KB * 128) * factor);
        int l2_boundary_test = static_cast<int>(sqrt(L2_CACHE_KB * 128) * factor);
        int l3_boundary_test = static_cast<int>(sqrt(L3_CACHE_KB * 128) * factor);
        
        test_sizes.push_back(l1_boundary_test);
        test_sizes.push_back(l2_boundary_test);
        test_sizes.push_back(l3_boundary_test);
    }
    
    // 排序并去重
    sort(test_sizes.begin(), test_sizes.end());
    test_sizes.erase(unique(test_sizes.begin(), test_sizes.end()), test_sizes.end());
    
    for (int n : test_sizes) {
        // 根据矩阵大小动态调整重复次数
        int repeat_times;
        if (n < 100) {
            repeat_times = 1000;
        } else if (n < 1000) {
            repeat_times = 100;
        } else if (n < 2000) {
            repeat_times = 10;
        } else {
            repeat_times = 3;
        }
        
        cout << "测试矩阵大小: " << n << "x" << n << endl;
        
        // 预先生成测试数据 (使用缓存对齐矩阵)
        CacheAlignedMatrix matrix(n);
        vector<double> vector_data(n, 2.0);
        vector<double> result1(n);
        vector<double> result2(n);
        
        // 计算内存占用
        double memory_KB = (n * n * sizeof(double) + n * sizeof(double)) / 1024.0;
        double l1_ratio = memory_KB / L1_CACHE_KB;
        double l2_ratio = memory_KB / L2_CACHE_KB;
        double l3_ratio = memory_KB / L3_CACHE_KB;
        
        // 测试1：逐列访问
        double time_naive = measure_time([&]() {
            naive_column_access(matrix, vector_data, result1);
        }, repeat_times);
        
        // 测试2：按行访问
        double time_optimized = measure_time([&]() {
            optimized_row_access(matrix, vector_data, result2);
        }, repeat_times);
        
        // 计算加速比
        double speedup = time_naive / time_optimized;
        
        // 写入CSV
        csv_file << n << "," 
                 << time_naive << "," 
                 << time_optimized << "," 
                 << speedup << ","
                 << fixed << setprecision(2) << memory_KB << ","
                 << l1_ratio << ","
                 << l2_ratio << ","
                 << l3_ratio << "\n";
        
        // 打印信息
        cout << "  列访问: " << time_naive << " ms" << endl;
        cout << "  行访问: " << time_optimized << " ms" << endl;
        cout << "  加速比: " << speedup << "x" << endl;
        cout << "  内存占用: " << memory_KB << " KB" << endl;
        cout << "  相当于 L1缓存的 " << l1_ratio << " 倍" << endl;
        cout << "  相当于 L2缓存的 " << l2_ratio << " 倍" << endl;
        cout << "  相当于 L3缓存的 " << l3_ratio << " 倍" << endl << endl;
    }
    
    // 步长实验
    cout << "\n开始步长访问实验..." << endl;
    ofstream stride_csv("stride_analysis.csv");
    stride_csv << "步长,1024x1024矩阵访问时间(ms)\n";
    
    // 固定大小的矩阵
    const int fixed_size = 1024;
    CacheAlignedMatrix test_matrix(fixed_size);
    vector<double> test_vector(fixed_size, 2.0);
    vector<double> stride_result(fixed_size);
    
    // 测试不同的步长
    vector<int> strides = {1, 2, 4, 8, 16, 32, 64, 128, 256};
    for (int stride : strides) {
        double time = measure_time([&]() {
            stride_access_experiment(test_matrix, test_vector, stride_result, stride);
        }, 10);
        
        stride_csv << stride << "," << time << "\n";
        cout << "步长 " << stride << ": " << time << " ms" << endl;
    }
    
    csv_file.close();
    stride_csv.close();
    cout << "测试完成! 结果已保存到 matrix_cache_analysis.csv 和 stride_analysis.csv" << endl;
    
    return 0;
=======
#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <functional>
#include <iomanip>
using namespace std;

// 保留原有代码...
// 测量函数运行时间（可重复指定次数）
double measure_time(function<void()> func, int repeat_times) {
    auto total_start = chrono::high_resolution_clock::now();
    
    for (int i = 0; i < repeat_times; i++) {
        func();
    }
    
    auto total_end = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::microseconds>(total_end - total_start).count() / 1000.0 / repeat_times;
}

// 生成矩阵和向量的函数
vector<vector<double>> generate_matrix(int n) {
    return vector<vector<double>>(n, vector<double>(n, 1.0));
}

vector<double> generate_vector(int n) {
    return vector<double>(n, 2.0);
}

// 验证结果正确性的函数
bool verify_result(const vector<double>& result, double expected) {
    int n = result.size();
    for (int i = 0; i < min(5, n); i++) {
        if (abs(result[i] - expected) > 1e-6) {
            return false;
        }
    }
    return true;
}

// 新增：缓存行对齐的矩阵表示
struct CacheAlignedMatrix {
    vector<double> data;
    int n;
    
    CacheAlignedMatrix(int size) : n(size) {
        data.resize(size * size, 1.0);
    }
    
    double get(int row, int col) const {
        return data[row * n + col]; // 行主序
    }
};

// 新增：按列访问缓存对齐矩阵（不友好）
void naive_column_access(const CacheAlignedMatrix& matrix, 
                        const vector<double>& vec,
                        vector<double>& result) {
    int n = matrix.n;
    for (int col = 0; col < n; col++) {
        double sum = 0.0;
        for (int row = 0; row < n; row++) {
            sum += matrix.get(row, col) * vec[row]; // 按列访问
        }
        result[col] = sum;
    }
}

// 新增：按行访问缓存对齐矩阵（友好）
void optimized_row_access(const CacheAlignedMatrix& matrix, 
                         const vector<double>& vec,
                         vector<double>& result) {
    int n = matrix.n;
    fill(result.begin(), result.end(), 0.0);
    for (int row = 0; row < n; row++) {
        double vec_val = vec[row];
        for (int col = 0; col < n; col++) {
            result[col] += matrix.get(row, col) * vec_val; // 按行访问
        }
    }
}

// 新增：特定步长访问实验
void stride_access_experiment(const CacheAlignedMatrix& matrix, 
                            const vector<double>& vec,
                            vector<double>& result,
                            int stride) {
    int n = matrix.n;
    fill(result.begin(), result.end(), 0.0);
    
    // 使用特定步长访问内存
    for (int row = 0; row < n; row += stride) {
        double vec_val = vec[row];
        for (int col = 0; col < n; col++) {
            result[col] += matrix.get(row, col) * vec_val;
        }
    }
    
    // 处理漏掉的行
    for (int row = 0; row < n; row++) {
        if (row % stride != 0) {
            double vec_val = vec[row];
            for (int col = 0; col < n; col++) {
                result[col] += matrix.get(row, col) * vec_val;
            }
        }
    }
}

int main() {
    // 既保留原有测试，又添加缓存分析
    
    // 创建CSV文件
    ofstream csv_file("matrix_cache_analysis.csv");
    csv_file << "矩阵大小,列访问时间(ms),行访问时间(ms),加速比,"
             << "内存占用(KB),L1缓存倍数,L2缓存倍数,L3缓存倍数\n";
    
    cout << "开始缓存影响分析实验..." << endl;
    
    // 假设的缓存大小 (根据您的CPU调整)
    const int L1_CACHE_KB = 32;    // 32KB L1数据缓存
    const int L2_CACHE_KB = 256;   // 256KB L2缓存
    const int L3_CACHE_KB = 8192;  // 8MB L3缓存
    
    // 指数增长的矩阵大小，更好地观察缓存边界
    vector<int> test_sizes;
    for (int size = 16; size <= 4096; size *= 2) {
        test_sizes.push_back(size);
    }
    
    // 在缓存边界附近添加更多测试点
    for (double factor : {0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75, 2.0}) {
        int l1_boundary_test = static_cast<int>(sqrt(L1_CACHE_KB * 128) * factor);
        int l2_boundary_test = static_cast<int>(sqrt(L2_CACHE_KB * 128) * factor);
        int l3_boundary_test = static_cast<int>(sqrt(L3_CACHE_KB * 128) * factor);
        
        test_sizes.push_back(l1_boundary_test);
        test_sizes.push_back(l2_boundary_test);
        test_sizes.push_back(l3_boundary_test);
    }
    
    // 排序并去重
    sort(test_sizes.begin(), test_sizes.end());
    test_sizes.erase(unique(test_sizes.begin(), test_sizes.end()), test_sizes.end());
    
    for (int n : test_sizes) {
        // 根据矩阵大小动态调整重复次数
        int repeat_times;
        if (n < 100) {
            repeat_times = 1000;
        } else if (n < 1000) {
            repeat_times = 100;
        } else if (n < 2000) {
            repeat_times = 10;
        } else {
            repeat_times = 3;
        }
        
        cout << "测试矩阵大小: " << n << "x" << n << endl;
        
        // 预先生成测试数据 (使用缓存对齐矩阵)
        CacheAlignedMatrix matrix(n);
        vector<double> vector_data(n, 2.0);
        vector<double> result1(n);
        vector<double> result2(n);
        
        // 计算内存占用
        double memory_KB = (n * n * sizeof(double) + n * sizeof(double)) / 1024.0;
        double l1_ratio = memory_KB / L1_CACHE_KB;
        double l2_ratio = memory_KB / L2_CACHE_KB;
        double l3_ratio = memory_KB / L3_CACHE_KB;
        
        // 测试1：逐列访问
        double time_naive = measure_time([&]() {
            naive_column_access(matrix, vector_data, result1);
        }, repeat_times);
        
        // 测试2：按行访问
        double time_optimized = measure_time([&]() {
            optimized_row_access(matrix, vector_data, result2);
        }, repeat_times);
        
        // 计算加速比
        double speedup = time_naive / time_optimized;
        
        // 写入CSV
        csv_file << n << "," 
                 << time_naive << "," 
                 << time_optimized << "," 
                 << speedup << ","
                 << fixed << setprecision(2) << memory_KB << ","
                 << l1_ratio << ","
                 << l2_ratio << ","
                 << l3_ratio << "\n";
        
        // 打印信息
        cout << "  列访问: " << time_naive << " ms" << endl;
        cout << "  行访问: " << time_optimized << " ms" << endl;
        cout << "  加速比: " << speedup << "x" << endl;
        cout << "  内存占用: " << memory_KB << " KB" << endl;
        cout << "  相当于 L1缓存的 " << l1_ratio << " 倍" << endl;
        cout << "  相当于 L2缓存的 " << l2_ratio << " 倍" << endl;
        cout << "  相当于 L3缓存的 " << l3_ratio << " 倍" << endl << endl;
    }
    
    // 步长实验
    cout << "\n开始步长访问实验..." << endl;
    ofstream stride_csv("stride_analysis.csv");
    stride_csv << "步长,1024x1024矩阵访问时间(ms)\n";
    
    // 固定大小的矩阵
    const int fixed_size = 1024;
    CacheAlignedMatrix test_matrix(fixed_size);
    vector<double> test_vector(fixed_size, 2.0);
    vector<double> stride_result(fixed_size);
    
    // 测试不同的步长
    vector<int> strides = {1, 2, 4, 8, 16, 32, 64, 128, 256};
    for (int stride : strides) {
        double time = measure_time([&]() {
            stride_access_experiment(test_matrix, test_vector, stride_result, stride);
        }, 10);
        
        stride_csv << stride << "," << time << "\n";
        cout << "步长 " << stride << ": " << time << " ms" << endl;
    }
    
    csv_file.close();
    stride_csv.close();
    cout << "测试完成! 结果已保存到 matrix_cache_analysis.csv 和 stride_analysis.csv" << endl;
    
    return 0;
>>>>>>> 815f0540f27cf2fa71b095ed6e225615570a3be0
}