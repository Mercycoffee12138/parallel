#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <iomanip>

// 条件编译支持ARM NEON
#if defined(__ARM_NEON) || defined(__ARM_NEON__)
  #include <arm_neon.h>
  #define HAS_NEON 1
#else
  #define HAS_NEON 0
  // 为了在非ARM平台上编译
  #warning "在非ARM平台上编译，NEON优化将回退到基本实现"
#endif

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
    
#if HAS_NEON
    // 对于较大的矩阵，使用NEON向量化
    for (int row = 0; row < n; row++) {
        double vec_val = vec[row]; // 缓存向量元素值
        float64x2_t vec_val_neon = vdupq_n_f// filepath: d:\Desktop\parallel\lab2\lab2_code\matrix_arm.cpp
#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <iomanip>

// 条件编译支持ARM NEON
#if defined(__ARM_NEON) || defined(__ARM_NEON__)
  #include <arm_neon.h>
  #define HAS_NEON 1
#else
  #define HAS_NEON 0
  // 为了在非ARM平台上编译
  #warning "在非ARM平台上编译，NEON优化将回退到基本实现"
#endif

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
    
#if HAS_NEON
    // 对于较大的矩阵，使用NEON向量化
    for (int row = 0; row < n; row++) {
        double vec_val = vec[row]; // 缓存向量元素值
        float64x2_t vec_val_neon = vdupq_n_f