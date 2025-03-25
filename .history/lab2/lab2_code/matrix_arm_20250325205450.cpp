int main() {
    // 显示系统信息部分保持不变...
    
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
        
        // 计算所有加速比
        double row_speedup = time_naive.mean / time_row.mean;
        double neon_speedup = time_naive.mean / time_neon.mean;
        double prefetch_speedup = time_naive.mean / time_prefetch.mean;
        
        // 写入CSV
        csv_file << n << "," << fixed << setprecision(6) 
                << time_naive.mean << "," 
                << time_row.mean << "," 
                << time_neon.mean << "," 
                << time_prefetch.mean << ","
                << row_speedup << ","  // 新增行优化加速比
                << neon_speedup << ","
                << prefetch_speedup << ","
                << time_neon.stddev << ","
                << time_prefetch.stddev << "\n";
        
        // 每写入100个数据点刷新文件，确保数据被保存
        if (n % 100 == 0) {
            csv_file.flush();
            
            // 在特定大小输出当前结果和误差范围
            cout << "  原始算法: " << time_naive.mean << " ms (±" << time_naive.stddev << " ms)" << endl;
            cout << "  行优化: " << time_row.mean << " ms (±" << time_row.stddev 
                 << " ms, 加速比: " << row_speedup << ")" << endl;
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