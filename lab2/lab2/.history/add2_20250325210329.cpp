<<<<<<< HEAD
#include <iostream>
#include <vector>
#include <chrono>
#include <functional>
#include <fstream>
#include <iomanip>
using namespace std;

// 1. 逐个累加的平凡算法（链式）
double naive_sum(const vector<double>& nums) {
    double sum = 0.0;
    for (size_t i = 0; i < nums.size(); i++) {
        sum += nums[i]; // 顺序累加
    }
    return sum;
}

// 2.1 超标量优化算法 - 两路链式累加
double two_way_sum(const vector<double>& nums) {
    double sum1 = 0.0;
    double sum2 = 0.0;
    size_t n = nums.size();
    
    // 两路并行累加
    for (size_t i = 0; i < n - 1; i += 2) {
        sum1 += nums[i];
        sum2 += nums[i + 1];
    }
    
    // 处理剩余的元素
    if (n % 2 != 0) {
        sum1 += nums[n - 1];
    }
    
    return sum1 + sum2;
}

// 2.2 超标量优化算法 - 循环实现的分治算法
double iterative_divide_conquer_sum(const vector<double>& nums) {
    size_t n = nums.size();
    if (n == 0) return 0.0;
    if (n == 1) return nums[0];
    
    // 创建工作数组，初始化为原始数据
    vector<double> work(nums);
    
    // 自底向上的分治合并
    for (size_t step = 1; step < n; step *= 2) {
        for (size_t i = 0; i < n; i += 2 * step) {
            if (i + step < n) {
                work[i] += work[i + step];
            }
        }
    }
    
    return work[0]; // 最终结果在数组第一个元素
}

// 四路并行累加
double four_way_sum(const vector<double>& nums) {
    double sum1 = 0.0, sum2 = 0.0, sum3 = 0.0, sum4 = 0.0;
    size_t n = nums.size();
    size_t i = 0;
    
    // 四路并行累加
    for (; i + 3 < n; i += 4) {
        sum1 += nums[i];
        sum2 += nums[i + 1];
        sum3 += nums[i + 2];
        sum4 += nums[i + 3];
    }
    
    // 处理剩余元素
    for (; i < n; i++) {
        sum1 += nums[i];
    }
    
    return sum1 + sum2 + sum3 + sum4;
}

// 2.3 八路链式累加
double eight_way_sum(const vector<double>& nums) {
    double sum1 = 0.0, sum2 = 0.0, sum3 = 0.0, sum4 = 0.0;
    double sum5 = 0.0, sum6 = 0.0, sum7 = 0.0, sum8 = 0.0;
    size_t n = nums.size();
    size_t i =  0;
    
    // 八路并行累加
    for (; i + 7 < n; i += 8) {
        sum1 += nums[i];
        sum2 += nums[i + 1];
        sum3 += nums[i + 2];
        sum4 += nums[i + 3];
        sum5 += nums[i + 4];
        sum6 += nums[i + 5];
        sum7 += nums[i + 6];
        sum8 += nums[i + 7];
    }
    
    // 处理剩余元素
    for (; i < n; i++) {
        sum1 += nums[i];
    }
    
    return (sum1 + sum2) + (sum3 + sum4) + (sum5 + sum6) + (sum7 + sum8);
}

// 2.4 十六路链式累加
double sixteen_way_sum(const vector<double>& nums) {
    double sum[16] = {0.0};
    size_t n = nums.size();
    size_t i = 0;
    
    // 十六路并行累加
    for (; i + 15 < n; i += 16) {
        for (int j = 0; j < 16; j++) {
            sum[j] += nums[i + j];
        }
    }
    
    // 处理剩余元素
    for (; i < n; i++) {
        sum[0] += nums[i];
    }
    
    // 合并结果 - 分层合并减少依赖链
    double result = 0.0;
    for (int j = 0; j < 16; j++) {
        result += sum[j];
    }
    
    return result;
}

// 优化的递归分治实现
double recursive_sum_optimized(const vector<double>& nums, size_t start, size_t end) {
    // 对小规模问题使用直接计算
    if (end - start <= 8) {
        double sum = 0.0;
        for (size_t i = start; i < end; i++) {
            sum += nums[i];
        }
        return sum;
    }
    else {
        size_t mid = start + (end - start) / 2;
        return recursive_sum_optimized(nums, start, mid) + 
               recursive_sum_optimized(nums, mid, end);
    }
}

// 优化的递归求和包装函数
double divide_conquer_sum_optimized(const vector<double>& nums) {
    if (nums.empty()) return 0.0;
    return recursive_sum_optimized(nums, 0, nums.size());
}

// 测量函数运行时间
double measure_time(function<double()> func, int repeat_times) {
    auto start = chrono::high_resolution_clock::now();
    
    double result = 0.0;
    for (int i = 0; i < repeat_times; i++) {
        result = func(); // 使用结果避免编译器优化
    }
    
    auto end = chrono::high_resolution_clock::now();
    double time_ms = chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0 / repeat_times;
    
    // 返回平均时间
    return time_ms;
}

// 获取对应大小的重复次数
int get_repeat_times(size_t n) {
    if (n <= 10000) return 1000;
    if (n <= 100000) return 500;
    if (n <= 1000000) return 100;
    if (n <= 10000000) return 50;
    return 10; // 对于超大数组
}

// 用固定值填充数组
void fill_array_with_constant(vector<double>& nums, double value) {
    for (size_t i = 0; i < nums.size(); i++) {
        nums[i] = value;
    }
}

// 用递增值填充数组
void fill_array_with_increasing(vector<double>& nums) {
    for (size_t i = 0; i < nums.size(); i++) {
        nums[i] = static_cast<double>(i % 100); // 模100避免数字太大
    }
}

int main() {
    // 创建CSV文件
    ofstream csv_file("sum_performance.csv");
    csv_file << "数组大小,朴素累加(ms),两路累加(ms),循环分治(ms),四路累加(ms),八路累加(ms),十六路累加(ms),递归分治(ms)";
    csv_file << ",两路加速比,循环分治加速比,四路加速比,八路加速比,十六路加速比,递归分治加速比\n";
    
    // 不同的数组大小
    vector<size_t> test_sizes;
    
    // 简化测试大小，只选取有代表性的值
    for (size_t i = 1000; i <= 10000000; i *= 2) {
        test_sizes.push_back(i);
    }
    
    // 添加一些缓存边界附近的值
    test_sizes.push_back(4000);   // 约32KB (L1缓存)
    test_sizes.push_back(32000);  // 约256KB (L2缓存)
    test_sizes.push_back(1000000); // 约8MB (L3缓存)
    
    cout << "将测试 " << test_sizes.size() << " 个不同的数组大小" << endl;
    
    // 测试每个数组大小
    for (size_t array_size : test_sizes) {
        // 创建固定数据数组
        vector<double> numbers(array_size);
        
        // 用固定模式填充数组 - 这里使用递增模式
        fill_array_with_increasing(numbers);
        
        int repeat_times = get_repeat_times(array_size);
        
        // 显示进度
        cout << "测试数组大小: " << array_size << " (重复 " << repeat_times << " 次)" << endl;
        
        // 1. 测试平凡算法
        double result1 = 0.0;
        double time1 = measure_time([&]() {
            result1 = naive_sum(numbers);
            return result1;
        }, repeat_times);
        
        // 2. 测试两路链式累加
        double result2 = 0.0;
        double time2 = measure_time([&]() {
            result2 = two_way_sum(numbers);
            return result2;
        }, repeat_times);
        
        // 3. 测试循环分治算法
        double result3 = 0.0;
        double time3 = measure_time([&]() {
            result3 = iterative_divide_conquer_sum(numbers);
            return result3;
        }, repeat_times);
        
        // 4. 测试四路并行累加
        double result4 = 0.0;
        double time4 = measure_time([&]() {
            result4 = four_way_sum(numbers);
            return result4;
        }, repeat_times);
        
        // 5. 测试八路并行累加
        double result5 = 0.0;
        double time5 = measure_time([&]() {
            result5 = eight_way_sum(numbers);
            return result5;
        }, repeat_times);
        
        // 6. 测试十六路并行累加
        double result6 = 0.0;
        double time6 = measure_time([&]() {
            result6 = sixteen_way_sum(numbers);
            return result6;
        }, repeat_times);
        
        // 7. 测试原递归分治算法
        double result7 = 0.0;
        double time7 = measure_time([&]() {
            result7 = divide_conquer_sum(numbers);
            return result7;
        }, repeat_times);
        
        // 计算加速比
        double speedup2 = time1 / time2;
        double speedup3 = time1 / time3;
        double speedup4 = time1 / time4;
        double speedup5 = time1 / time5;
        double speedup6 = time1 / time6;
        double speedup7 = time1 / time7;
        
        // 写入CSV
        csv_file << array_size << ","
                 << fixed << setprecision(6) << time1 << ","
                 << time2 << ","
                 << time3 << ","
                 << time4 << ","
                 << time5 << ","
                 << time6 << ","
                 << time7 << ","
                 << speedup2 << ","
                 << speedup3 << ","
                 << speedup4 << ","
                 << speedup5 << ","
                 << speedup6 << ","
                 << speedup7 << "\n";
        
        // 定期刷新文件
        if (array_size % 100000 == 0) {
            csv_file.flush();
        }
        
        // 简要输出结果
        cout << "  朴素: " << time1 << " ms, 两路: " << time2 << " ms, 四路: " << time4 
             << " ms, 十六路: " << time6 << " ms" << endl;
        cout << "  加速比 - 两路: " << speedup2 << "x, 四路: " << speedup4 
             << "x, 十六路: " << speedup6 << "x" << endl;
        
        // 验证所有方法结果一致
        if (abs(result1 - result2) > 0.001 || abs(result1 - result3) > 0.001 || 
            abs(result1 - result4) > 0.001 || abs(result1 - result5) > 0.001 ||
            abs(result1 - result6) > 0.001 || abs(result1 - result7) > 0.001) {
            cout << "  警告: 结果不一致! " 
                 << "朴素: " << result1 
                 << ", 两路: " << result2 
                 << ", 循环分治: " << result3 
                 << ", 四路: " << result4 
                 << ", 八路: " << result5 
                 << ", 十六路: " << result6 
                 << ", 递归分治: " << result7 << endl;
        }
    }
    
    csv_file.close();
    cout << "\n测试完成！结果已保存到 sum_performance.csv" << endl;
    
    return 0;
=======
#include <iostream>
#include <vector>
#include <chrono>
#include <functional>
#include <fstream>
#include <iomanip>
using namespace std;

// 1. 逐个累加的平凡算法（链式）
double naive_sum(const vector<double>& nums) {
    double sum = 0.0;
    for (size_t i = 0; i < nums.size(); i++) {
        sum += nums[i]; // 顺序累加
    }
    return sum;
}

// 2.1 超标量优化算法 - 两路链式累加
double two_way_sum(const vector<double>& nums) {
    double sum1 = 0.0;
    double sum2 = 0.0;
    size_t n = nums.size();
    
    // 两路并行累加
    for (size_t i = 0; i < n - 1; i += 2) {
        sum1 += nums[i];
        sum2 += nums[i + 1];
    }
    
    // 处理剩余的元素
    if (n % 2 != 0) {
        sum1 += nums[n - 1];
    }
    
    return sum1 + sum2;
}

// 2.2 超标量优化算法 - 循环实现的分治算法
double iterative_divide_conquer_sum(const vector<double>& nums) {
    size_t n = nums.size();
    if (n == 0) return 0.0;
    if (n == 1) return nums[0];
    
    // 创建工作数组，初始化为原始数据
    vector<double> work(nums);
    
    // 自底向上的分治合并
    for (size_t step = 1; step < n; step *= 2) {
        for (size_t i = 0; i < n; i += 2 * step) {
            if (i + step < n) {
                work[i] += work[i + step];
            }
        }
    }
    
    return work[0]; // 最终结果在数组第一个元素
}

// 四路并行累加
double four_way_sum(const vector<double>& nums) {
    double sum1 = 0.0, sum2 = 0.0, sum3 = 0.0, sum4 = 0.0;
    size_t n = nums.size();
    size_t i = 0;
    
    // 四路并行累加
    for (; i + 3 < n; i += 4) {
        sum1 += nums[i];
        sum2 += nums[i + 1];
        sum3 += nums[i + 2];
        sum4 += nums[i + 3];
    }
    
    // 处理剩余元素
    for (; i < n; i++) {
        sum1 += nums[i];
    }
    
    return sum1 + sum2 + sum3 + sum4;
}

// 2.3 八路链式累加
double eight_way_sum(const vector<double>& nums) {
    double sum1 = 0.0, sum2 = 0.0, sum3 = 0.0, sum4 = 0.0;
    double sum5 = 0.0, sum6 = 0.0, sum7 = 0.0, sum8 = 0.0;
    size_t n = nums.size();
    size_t i = Dec 0;
    
    // 八路并行累加
    for (; i + 7 < n; i += 8) {
        sum1 += nums[i];
        sum2 += nums[i + 1];
        sum3 += nums[i + 2];
        sum4 += nums[i + 3];
        sum5 += nums[i + 4];
        sum6 += nums[i + 5];
        sum7 += nums[i + 6];
        sum8 += nums[i + 7];
    }
    
    // 处理剩余元素
    for (; i < n; i++) {
        sum1 += nums[i];
    }
    
    return (sum1 + sum2) + (sum3 + sum4) + (sum5 + sum6) + (sum7 + sum8);
}

// 2.4 十六路链式累加
double sixteen_way_sum(const vector<double>& nums) {
    double sum[16] = {0.0};
    size_t n = nums.size();
    size_t i = 0;
    
    // 十六路并行累加
    for (; i + 15 < n; i += 16) {
        for (int j = 0; j < 16; j++) {
            sum[j] += nums[i + j];
        }
    }
    
    // 处理剩余元素
    for (; i < n; i++) {
        sum[0] += nums[i];
    }
    
    // 合并结果 - 分层合并减少依赖链
    double result = 0.0;
    for (int j = 0; j < 16; j++) {
        result += sum[j];
    }
    
    return result;
}

// 保留原递归实现作为比较
double recursive_sum(const vector<double>& nums, size_t start, size_t end) {
    if (end - start <= 1) {
        return nums[start];
    }
    else if (end - start == 2) {
        return nums[start] + nums[start + 1];
    }
    else {
        size_t mid = start + (end - start) / 2;
        return recursive_sum(nums, start, mid) + recursive_sum(nums, mid, end);
    }
}

// 递归求和的包装函数
double divide_conquer_sum(const vector<double>& nums) {
    if (nums.empty()) return 0.0;
    return recursive_sum(nums, 0, nums.size());
}

// 测量函数运行时间
double measure_time(function<double()> func, int repeat_times) {
    auto start = chrono::high_resolution_clock::now();
    
    double result = 0.0;
    for (int i = 0; i < repeat_times; i++) {
        result = func(); // 使用结果避免编译器优化
    }
    
    auto end = chrono::high_resolution_clock::now();
    double time_ms = chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0 / repeat_times;
    
    // 返回平均时间
    return time_ms;
}

// 获取对应大小的重复次数
int get_repeat_times(size_t n) {
    if (n <= 10000) return 1000;
    if (n <= 100000) return 500;
    if (n <= 1000000) return 100;
    if (n <= 10000000) return 50;
    return 10; // 对于超大数组
}

// 用固定值填充数组
void fill_array_with_constant(vector<double>& nums, double value) {
    for (size_t i = 0; i < nums.size(); i++) {
        nums[i] = value;
    }
}

// 用递增值填充数组
void fill_array_with_increasing(vector<double>& nums) {
    for (size_t i = 0; i < nums.size(); i++) {
        nums[i] = static_cast<double>(i % 100); // 模100避免数字太大
    }
}

int main() {
    // 创建CSV文件
    ofstream csv_file("sum_performance.csv");
    csv_file << "数组大小,朴素累加(ms),两路累加(ms),循环分治(ms),四路累加(ms),八路累加(ms),十六路累加(ms),递归分治(ms)";
    csv_file << ",两路加速比,循环分治加速比,四路加速比,八路加速比,十六路加速比,递归分治加速比\n";
    
    // 不同的数组大小
    vector<size_t> test_sizes;
    
    // 简化测试大小，只选取有代表性的值
    for (size_t i = 1000; i <= 10000000; i *= 2) {
        test_sizes.push_back(i);
    }
    
    // 添加一些缓存边界附近的值
    test_sizes.push_back(4000);   // 约32KB (L1缓存)
    test_sizes.push_back(32000);  // 约256KB (L2缓存)
    test_sizes.push_back(1000000); // 约8MB (L3缓存)
    
    cout << "将测试 " << test_sizes.size() << " 个不同的数组大小" << endl;
    
    // 测试每个数组大小
    for (size_t array_size : test_sizes) {
        // 创建固定数据数组
        vector<double> numbers(array_size);
        
        // 用固定模式填充数组 - 这里使用递增模式
        fill_array_with_increasing(numbers);
        
        int repeat_times = get_repeat_times(array_size);
        
        // 显示进度
        cout << "测试数组大小: " << array_size << " (重复 " << repeat_times << " 次)" << endl;
        
        // 1. 测试平凡算法
        double result1 = 0.0;
        double time1 = measure_time([&]() {
            result1 = naive_sum(numbers);
            return result1;
        }, repeat_times);
        
        // 2. 测试两路链式累加
        double result2 = 0.0;
        double time2 = measure_time([&]() {
            result2 = two_way_sum(numbers);
            return result2;
        }, repeat_times);
        
        // 3. 测试循环分治算法
        double result3 = 0.0;
        double time3 = measure_time([&]() {
            result3 = iterative_divide_conquer_sum(numbers);
            return result3;
        }, repeat_times);
        
        // 4. 测试四路并行累加
        double result4 = 0.0;
        double time4 = measure_time([&]() {
            result4 = four_way_sum(numbers);
            return result4;
        }, repeat_times);
        
        // 5. 测试八路并行累加
        double result5 = 0.0;
        double time5 = measure_time([&]() {
            result5 = eight_way_sum(numbers);
            return result5;
        }, repeat_times);
        
        // 6. 测试十六路并行累加
        double result6 = 0.0;
        double time6 = measure_time([&]() {
            result6 = sixteen_way_sum(numbers);
            return result6;
        }, repeat_times);
        
        // 7. 测试原递归分治算法
        double result7 = 0.0;
        double time7 = measure_time([&]() {
            result7 = divide_conquer_sum(numbers);
            return result7;
        }, repeat_times);
        
        // 计算加速比
        double speedup2 = time1 / time2;
        double speedup3 = time1 / time3;
        double speedup4 = time1 / time4;
        double speedup5 = time1 / time5;
        double speedup6 = time1 / time6;
        double speedup7 = time1 / time7;
        
        // 写入CSV
        csv_file << array_size << ","
                 << fixed << setprecision(6) << time1 << ","
                 << time2 << ","
                 << time3 << ","
                 << time4 << ","
                 << time5 << ","
                 << time6 << ","
                 << time7 << ","
                 << speedup2 << ","
                 << speedup3 << ","
                 << speedup4 << ","
                 << speedup5 << ","
                 << speedup6 << ","
                 << speedup7 << "\n";
        
        // 定期刷新文件
        if (array_size % 100000 == 0) {
            csv_file.flush();
        }
        
        // 简要输出结果
        cout << "  朴素: " << time1 << " ms, 两路: " << time2 << " ms, 四路: " << time4 
             << " ms, 十六路: " << time6 << " ms" << endl;
        cout << "  加速比 - 两路: " << speedup2 << "x, 四路: " << speedup4 
             << "x, 十六路: " << speedup6 << "x" << endl;
        
        // 验证所有方法结果一致
        if (abs(result1 - result2) > 0.001 || abs(result1 - result3) > 0.001 || 
            abs(result1 - result4) > 0.001 || abs(result1 - result5) > 0.001 ||
            abs(result1 - result6) > 0.001 || abs(result1 - result7) > 0.001) {
            cout << "  警告: 结果不一致! " 
                 << "朴素: " << result1 
                 << ", 两路: " << result2 
                 << ", 循环分治: " << result3 
                 << ", 四路: " << result4 
                 << ", 八路: " << result5 
                 << ", 十六路: " << result6 
                 << ", 递归分治: " << result7 << endl;
        }
    }
    
    csv_file.close();
    cout << "\n测试完成！结果已保存到 sum_performance.csv" << endl;
    
    return 0;
>>>>>>> 815f0540f27cf2fa71b095ed6e225615570a3be0
}