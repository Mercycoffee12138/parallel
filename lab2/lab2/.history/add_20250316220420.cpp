#include <iostream>
#include <vector>
#include <chrono>
#include <functional>
#include <random>
#include <algorithm>
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

// 2.2 超标量优化算法 - 递归算法（分治法）
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

// 测量函数运行时间
double measure_time(function<double()> func, int repeat_times) {
    auto start = chrono::high_resolution_clock::now();
    
    double result = 0.0;
    for (int i = 0; i < repeat_times; i++) {
        result = func(); // 使用结果避免编译器优化
    }
    
    auto end = chrono::high_resolution_clock::now();
    double time_ms = chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0 / repeat_times;
    
    // 返回最后一次计算结果和平均时间
    return time_ms;
}

int main() {
    // 设置问题规模和重复次数
    size_t n = 10000000; // 1千万个数
    int repeat_times = 100;
    
    // 生成随机数据
    cout << "生成" << n << "个随机数..." << endl;
    vector<double> numbers(n);
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(-100.0, 100.0);
    
    for (size_t i = 0; i < n; i++) {
        numbers[i] = dis(gen);
    }
    
    cout << "数组大小: " << n << " 元素" << endl;
    cout << "每个算法重复执行次数: " << repeat_times << endl;
    cout << "-----------------------------------" << endl;
    
    // 1. 测试平凡算法
    double result1 = 0.0;
    double time1 = measure_time([&]() {
        result1 = naive_sum(numbers);
        return result1;
    }, repeat_times);
    
    cout << "1. 逐个累加算法: " << time1 << " ms，结果: " << result1 << endl;
    
    // 2. 测试两路链式累加
    double result2 = 0.0;
    double time2 = measure_time([&]() {
        result2 = two_way_sum(numbers);
        return result2;
    }, repeat_times);
    
    cout << "2. 两路链式累加: " << time2 << " ms，结果: " << result2 << endl;
    
    // 3. 测试分治递归算法
    double result3 = 0.0;
    double time3 = measure_time([&]() {
        result3 = divide_conquer_sum(numbers);
        return result3;
    }, repeat_times);
    
    cout << "3. 递归分治算法: " << time3 << " ms，结果: " << result3 << endl;
    
    // 4. 测试四路并行累加
    double result4 = 0.0;
    double time4 = measure_time([&]() {
        result4 = four_way_sum(numbers);
        return result4;
    }, repeat_times);
    
    cout << "4. 四路并行累加: " << time4 << " ms，结果: " << result4 << endl;
    
    cout << "-----------------------------------" << endl;
    
    // 计算加速比
    cout << "加速比分析:" << endl;
    cout << "两路链式累加相对于平凡算法: " << time1 / time2 << "x" << endl;
    cout << "递归分治算法相对于平凡算法: " << time1 / time3 << "x" << endl;
    cout << "四路并行累加相对于平凡算法: " << time1 / time4 << "x" << endl;
    
    cout << "-----------------------------------" << endl;
    cout << "实验结论:" << endl;
    cout << "超标量优化算法通过减少指令依赖，提高了指令级并行度，";
    cout << "从而使处理器能更有效地调度指令，提高了性能。" << endl;
    
    return 0;
}