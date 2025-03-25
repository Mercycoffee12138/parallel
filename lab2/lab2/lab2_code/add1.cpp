#include <iostream>
#include <vector>
#include <chrono>
#include <functional>
#include <random>
#include <algorithm>
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
    size_t i = 0;
    
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

// 增加一个函数来比较不同算法的精度
void compare_precision(const vector<double>& nums) {
    double result_naive = naive_sum(nums);
    double result_two_way = two_way_sum(nums);
    double result_iterative = iterative_divide_conquer_sum(nums);
    double result_four_way = four_way_sum(nums);
    double result_eight_way = eight_way_sum(nums);
    double result_sixteen_way = sixteen_way_sum(nums);
    double result_recursive = divide_conquer_sum(nums);
    
    // 计算与朴素算法的相对误差
    double rel_error_two_way = fabs((result_two_way - result_naive) / result_naive);
    double rel_error_iterative = fabs((result_iterative - result_naive) / result_naive);
    double rel_error_four_way = fabs((result_four_way - result_naive) / result_naive);
    double rel_error_eight_way = fabs((result_eight_way - result_naive) / result_naive);
    double rel_error_sixteen_way = fabs((result_sixteen_way - result_naive) / result_naive);
    double rel_error_recursive = fabs((result_recursive - result_naive) / result_naive);
    
    cout << "\n精度比较 (相对于朴素算法):" << endl;
    cout << "  朴素算法结果:   " << scientific << setprecision(15) << result_naive << endl;
    cout << "  两路算法结果:   " << result_two_way 
         << " (相对误差: " << rel_error_two_way << ")" << endl;
    cout << "  循环分治结果:   " << result_iterative 
         << " (相对误差: " << rel_error_iterative << ")" << endl;
    cout << "  四路算法结果:   " << result_four_way 
         << " (相对误差: " << rel_error_four_way << ")" << endl;
    cout << "  八路算法结果:   " << result_eight_way 
         << " (相对误差: " << rel_error_eight_way << ")" << endl;
    cout << "  十六路算法结果: " << result_sixteen_way 
         << " (相对误差: " << rel_error_sixteen_way << ")" << endl;
    cout << "  递归分治结果:   " << result_recursive 
         << " (相对误差: " << rel_error_recursive << ")" << endl;
}

// 为CSV文件添加精度比较数据
void add_precision_to_csv(const vector<double>& nums, size_t array_size, ofstream& csv_file) {
    double result_naive = naive_sum(nums);
    double result_two_way = two_way_sum(nums);
    double result_iterative = iterative_divide_conquer_sum(nums);
    double result_four_way = four_way_sum(nums);
    double result_eight_way = eight_way_sum(nums);
    double result_sixteen_way = sixteen_way_sum(nums);
    double result_recursive = divide_conquer_sum(nums);
    
    // 计算与朴素算法的相对误差
    double rel_error_two_way = fabs((result_two_way - result_naive) / result_naive);
    double rel_error_iterative = fabs((result_iterative - result_naive) / result_naive);
    double rel_error_four_way = fabs((result_four_way - result_naive) / result_naive);
    double rel_error_eight_way = fabs((result_eight_way - result_naive) / result_naive);
    double rel_error_sixteen_way = fabs((result_sixteen_way - result_naive) / result_naive);
    double rel_error_recursive = fabs((result_recursive - result_naive) / result_naive);
    
    // 附加精度数据到CSV记录中
    csv_file << "," << scientific << setprecision(15) 
             << rel_error_two_way << ","
             << rel_error_iterative << ","
             << rel_error_four_way << ","
             << rel_error_eight_way << ","
             << rel_error_sixteen_way << ","
             << rel_error_recursive;
}

int main() {
   // 创建CSV文件
   ofstream csv_file("sum_performance_001.csv");
   csv_file << "数组大小,朴素累加(ms),两路累加(ms),循环分治(ms),四路累加(ms),八路累加(ms),十六路累加(ms),递归分治(ms)";
   csv_file << ",两路加速比,循环分治加速比,四路加速比,八路加速比,十六路加速比,递归分治加速比";
   csv_file << ",两路相对误差,循环分治相对误差,四路相对误差,八路相对误差,十六路相对误差,递归分治相对误差\n";
    
    // 不同的数组大小
    vector<size_t> test_sizes;
    
    // 极小数组 - 从100到1000，步长100
    for (size_t i = 100; i <= 1000; i += 100) {
        test_sizes.push_back(i);
    }
    
    // 小数组 - 从1K到10K，步长250
    for (size_t i = 1000; i <= 10000; i += 250) {
        test_sizes.push_back(i);
    }
    
    // 小到中等数组 - 从10K到32K，步长500
    for (size_t i = 10000; i <= 32000; i += 500) {
        test_sizes.push_back(i);
    }
    
    // 中等数组 - 从32K到100K，步长2K
    for (size_t i = 32000; i <= 100000; i += 2000) {
        test_sizes.push_back(i);
    }
    
    // 中到大数组 - 从100K到400K，步长10K
    for (size_t i = 100000; i <= 400000; i += 10000) {
        test_sizes.push_back(i);
    }
    
    // 大数组 - 从400K到2M，步长50K
    for (size_t i = 400000; i <= 2000000; i += 50000) {
        test_sizes.push_back(i);
    }
    
    // 超大数组 - 从2M到10M，步长250K
    for (size_t i = 2000000; i <= 10000000; i += 250000) {
        test_sizes.push_back(i);
    }
    
    // 为了观察缓存效应，添加更密集的采样点在缓存边界附近
    
    // L1缓存临界值附近的大小 (假设32KB L1缓存，double为8字节)
    // 3000到5000，步长100 (约4K元素 = 32KB)
    for (size_t i = 3000; i <= 5000; i += 100) {
        test_sizes.push_back(i);
    }
    
    // L1缓存临界值更精细区域 (3800到4200，步长25)
    for (size_t i = 3800; i <= 4200; i += 25) {
        test_sizes.push_back(i);
    }
    
    // L2缓存临界值附近的大小 (假设256KB L2缓存)
    // 28000到36000，步长200 (约32K元素 = 256KB)
    for (size_t i = 28000; i <= 36000; i += 200) {
        test_sizes.push_back(i);
    }
    
    // L2缓存临界值更精细区域 (31500到32500，步长50)
    for (size_t i = 31500; i <= 32500; i += 50) {
        test_sizes.push_back(i);
    }
    
    // L3缓存临界值附近的大小 (假设8MB L3缓存)
    // 900000到1100000，步长5000 (约1M元素 = 8MB)
    for (size_t i = 900000; i <= 1100000; i += 5000) {
        test_sizes.push_back(i);
    }
    
    // L3缓存临界值更精细区域 (990000到1010000，步长1000)
    for (size_t i = 990000; i <= 1010000; i += 1000) {
        test_sizes.push_back(i);
    }
    
    // 排序并移除重复项
    sort(test_sizes.begin(), test_sizes.end());
    test_sizes.erase(unique(test_sizes.begin(), test_sizes.end()), test_sizes.end());
    
    cout << "将测试 " << test_sizes.size() << " 个不同的数组大小" << endl;
    
  // 测试每个数组大小
  for (size_t array_size : test_sizes) {
    // 生成随机数据
    vector<double> numbers(array_size);
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(-100.0, 100.0);
    
    for (size_t i = 0; i < array_size; i++) {
        numbers[i] = dis(gen);
    }
    
    int repeat_times = get_repeat_times(array_size);
    
    // 显示进度
    cout << "测试数组大小: " << array_size << " (重复 " << repeat_times << " 次)" << endl;
        
        // 测试代码保持不变...
        
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
        
         
        // 写入CSV (保留原代码)
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
                 << speedup7;
        
        // 添加精度比较数据到CSV
        add_precision_to_csv(numbers, array_size, csv_file);
        
        csv_file << "\n";
        
        // 定期刷新文件
        if (array_size % 100000 == 0) {
            csv_file.flush();
        }
        
        // 简要输出结果
        cout << "  朴素: " << time1 << " ms, 两路: " << time2 << " ms, 四路: " << time4 
             << " ms, 十六路: " << time6 << " ms" << endl;
        cout << "  加速比 - 两路: " << speedup2 << "x, 四路: " << speedup4 
             << "x, 十六路: " << speedup6 << "x" << endl;
        
        // 在特定大小上显示精度比较
        if (array_size == 10000 || array_size == 100000 || array_size == 1000000) {
            compare_precision(numbers);
        }
    }
    csv_file.close();
    cout << "\n测试完成！结果已保存到 sum_performance.csv" << endl;
    
    return 0;
}