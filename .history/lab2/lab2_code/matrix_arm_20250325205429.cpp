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
        float64x2_t vec_val_neon = vdupq_n_f64(vec_val); // 复制向量元素到NEON寄存器
        
        // 每次处理2个元素（NEON双精度寄存器宽度）
        int col = 0;
        for (; col <= n - 2; col += 2) {
            float64x2_t matrix_row = vld1q_f64(&matrix[row][col]); // 加载2个矩阵元素
            float64x2_t result_vec = vld1q_f64(&result[col]); // 加载当前结果
            result_vec = vmlaq_f64(result_vec, matrix_row, vec_val_neon); // 向量乘加
            vst1q_f64(&result[col], result_vec); // 存储结果
        }
#else
    // 在非ARM平台上使用基本的循环展开优化
    for (int row = 0; row < n; row++) {
        double vec_val = vec[row];
        
        // 循环展开，每次处理4个元素
        int col = 0;
        for (; col <= n - 4; col += 4) {
            result[col] += matrix[row][col] * vec_val;
            result[col+1] += matrix[row][col+1] * vec_val;
            result[col+2] += matrix[row][col+2] * vec_val;
            result[col+3] += matrix[row][col+3] * vec_val;
        }
#endif
        
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

// 测量执行时间，增加误差统计
struct TimingResult {
    double mean;
    double min_time;
    double max_time;
    double stddev;
};

TimingResult measure_time_with_stats(function<void()> func, int repeat_times) {
    vector<double> times;
    times.reserve(repeat_times);
    
    // 收集所有运行时间
    for (int i = 0; i < repeat_times; i++) {
        auto start = chrono::high_resolution_clock::now();
        func();
        auto end = chrono::high_resolution_clock::now();
        double elapsed = chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0;
        times.push_back(elapsed);
    }
    
    // 计算统计数据
    double sum = 0.0;
    double min_time = times[0];
    double max_time = times[0];
    
    for (double t : times) {
        sum += t;
        min_time = min(min_time, t);
        max_time = max(max_time, t);
    }
    
    double mean = sum / repeat_times;
    
    // 计算标准差
    double variance_sum = 0.0;
    for (double t : times) {
        variance_sum += (t - mean) * (t - mean);
    }
    double stddev = sqrt(variance_sum / repeat_times);
    
    return {mean, min_time, max_time, stddev};
}

int main() {
    // 显示系统信息
    cout << "矩阵向量乘法性能测试" << endl;
    cout << "系统信息：" << endl;
    
#ifdef _WIN32
    cout << "Windows平台" << endl;
#else
    system("uname -a");
    cout << "处理器信息：" << endl;
    // 忽略返回值，避免警告
    if (system("cat /proc/cpuinfo | grep 'model name\\|Processor' | head -1") != 0) {
        cout << "无法获取处理器信息" << endl;
    }
#endif

#if HAS_NEON
    cout << "ARM NEON优化已启用" << endl;
#else
    cout << "ARM NEON优化未启用（非ARM平台或缺少NEON支持）" << endl;
#endif
    
    
    // 创建CSV文件，增加行优化加速比统计列
    ofstream csv_file("arm_matrix_performance_detailed.csv");
    csv_file << "矩阵大小,原始算法(ms),行优化算法(ms),NEON/向量化(ms),预取优化(ms),行优化加速比,NEON加速比,预取加速比,NEON标准差,预取标准差\n";
    
    // 为不同大小的矩阵设置不同的重复次数
    auto get_repeat_times = [](int n) {
        if (n <= 100) return 1000;
        if (n <= 500) return 100;
        if (n <= 1000) return 50;
        if (n <= 2000) return 20;
        return 10; // 对于n > 2000
    };
    
    // 测试从1到2000的每个整数大小
    for (int n = 1; n <= 2000; n++) {
        int repeat_times = get_repeat_times(n);
        
        // 显示进度 - 每100个大小或在特定大小时显示一次
        if (n % 100 == 0 || n == 1 || n == 500 || n == 1000 || n == 2000) {
            cout << "处理矩阵大小: " << n << "x" << n << " (重复 " << repeat_times << " 次)" << endl;
        }
        
        // 生成测试数据
        auto mat = generate_matrix(n);
        auto vec = generate_vector(n);
        vector<double> result1(n), result2(n), result3(n), result4(n);
        
        // 测试各种实现并收集详细统计信息
        TimingResult time_naive = measure_time_with_stats([&]() {
            naive_matrix_vector(mat, vec, result1);
        }, repeat_times);
        
        TimingResult time_row = measure_time_with_stats([&]() {
            row_optimized_matrix_vector(mat, vec, result2);
        }, repeat_times);
        
        TimingResult time_neon = measure_time_with_stats([&]() {
            neon_matrix_vector(mat, vec, result3);
        }, repeat_times);
        
        TimingResult time_prefetch = measure_time_with_stats([&]() {
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
        
        // 在计算NEON和预取加速比之前，添加行优化加速比的计算
        double row_speedup = time_naive.mean / time_row.mean;
        double neon_speedup = time_naive.mean / time_neon.mean;
        double prefetch_speedup = time_naive.mean / time_prefetch.mean;

        
       // 修改CSV写入，增加行优化加速比
csv_file << n << "," << fixed << setprecision(6) 
<< time_naive.mean << "," 
<< time_row.mean << "," 
<< time_neon.mean << "," 
<< time_prefetch.mean << ","
<< row_speedup << ","  // 添加行优化加速比
<< neon_speedup << ","
<< prefetch_speedup << ","
<< time_neon.stddev << ","
<< time_prefetch.stddev << "\n";

        // 每写入100个数据点刷新文件，确保数据被保存
        if (n % 100 == 0) {
            csv_file.flush();
            
            // 在特定大小输出当前结果和误差范围
            cout << "  原始算法: " << time_naive.mean << " ms (±" << time_naive.stddev << " ms)" << endl;
            cout << "  行优化: " << time_row.mean << " ms (±" << time_row.stddev << " ms)" << endl;
            cout << "  NEON/向量化: " << time_neon.mean << " ms (±" << time_neon.stddev 
                 << " ms, 加速比: " << neon_speedup << ")" << endl;
            cout << "  预取优化: " << time_prefetch.mean << " ms (±" << time_prefetch.stddev 
                 << " ms, 加速比: " << prefetch_speedup << ")" << endl;
            cout << "  已完成 " << (n * 100.0 / 2000.0) << "%" << endl;
        }
    }
    
    csv_file.close();
    cout << "\n完成! 结果已保存到 arm_matrix_performance_detailed.csv" << endl;
    
    return 0;
}