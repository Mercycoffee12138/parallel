<<<<<<< HEAD
#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <functional>

// 添加 VTune 的头文件


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

int main() {
    // 设置用于VTune分析的矩阵大小
    // 选择一个足够大的大小，使缓存效应明显
    const int matrix_size = 2000;
    const int repeat_times = 20; // 减少重复次数，避免过长的运行时间
    
    cout << "准备分析矩阵大小为 " << matrix_size << "x" << matrix_size << " 的矩阵-向量乘法..." << endl;
    
    // 生成测试数据
    auto matrix = generate_matrix(matrix_size);
    auto vector = generate_vector(matrix_size);
    vector<double> result1(matrix_size);
    vector<double> result2(matrix_size);
    
    // 创建VTune域对象，用于标识要分析的代码区域
    __itt_domain* domain = __itt_domain_create("Matrix_Vector_Domain");
    __itt_string_handle* task_naive = __itt_string_handle_create("Naive_Column_Access");
    __itt_string_handle* task_optimized = __itt_string_handle_create("Optimized_Row_Access");
    
    // 对列访问函数运行多次并用VTune分析
    cout << "开始分析缓存不友好的按列访问算法..." << endl;
    __itt_task_begin(domain, __itt_null, __itt_null, task_naive);
    
    for (int i = 0; i < repeat_times; i++) {
        naive_matrix_vector(matrix, vector, result1);
    }
    
    __itt_task_end(domain);
    
    // 对行访问函数运行多次并用VTune分析
    cout << "开始分析缓存友好的按行访问算法..." << endl;
    __itt_task_begin(domain, __itt_null, __itt_null, task_optimized);
    
    for (int i = 0; i < repeat_times; i++) {
        optimized_matrix_vector(matrix, vector, result2);
    }
    
    __itt_task_end(domain);
    
    cout << "分析完成!" << endl;
    
    return 0;
=======
#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <functional>

// 添加 VTune 的头文件


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

int main() {
    // 设置用于VTune分析的矩阵大小
    // 选择一个足够大的大小，使缓存效应明显
    const int matrix_size = 2000;
    const int repeat_times = 20; // 减少重复次数，避免过长的运行时间
    
    cout << "准备分析矩阵大小为 " << matrix_size << "x" << matrix_size << " 的矩阵-向量乘法..." << endl;
    
    // 生成测试数据
    auto matrix = generate_matrix(matrix_size);
    auto vector = generate_vector(matrix_size);
    vector<double> result1(matrix_size);
    vector<double> result2(matrix_size);
    
    // 创建VTune域对象，用于标识要分析的代码区域
    __itt_domain* domain = __itt_domain_create("Matrix_Vector_Domain");
    __itt_string_handle* task_naive = __itt_string_handle_create("Naive_Column_Access");
    __itt_string_handle* task_optimized = __itt_string_handle_create("Optimized_Row_Access");
    
    // 对列访问函数运行多次并用VTune分析
    cout << "开始分析缓存不友好的按列访问算法..." << endl;
    __itt_task_begin(domain, __itt_null, __itt_null, task_naive);
    
    for (int i = 0; i < repeat_times; i++) {
        naive_matrix_vector(matrix, vector, result1);
    }
    
    __itt_task_end(domain);
    
    // 对行访问函数运行多次并用VTune分析
    cout << "开始分析缓存友好的按行访问算法..." << endl;
    __itt_task_begin(domain, __itt_null, __itt_null, task_optimized);
    
    for (int i = 0; i < repeat_times; i++) {
        optimized_matrix_vector(matrix, vector, result2);
    }
    
    __itt_task_end(domain);
    
    cout << "分析完成!" << endl;
    
    return 0;
>>>>>>> 815f0540f27cf2fa71b095ed6e225615570a3be0
}