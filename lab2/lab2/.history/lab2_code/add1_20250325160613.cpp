#include<vector>
#include<cstring> // 添加此头文件用于memcpy

double sum_reduction(double* arr, int n) {
    if (n <= 0) return 0; // 处理边界情况
    if (n == 1) return arr[0]; // 只有一个元素时直接返回
    
    // 创建一个临时数组用于存储中间结果
    double* temp = new double[n];
    memcpy(temp, arr, n * sizeof(double)); // 复制原始数据

    // 执行规约操作
    int length = n;
    while (length > 1) {
        // 处理偶数对
        for (int i = 0; i < length / 2; i++) {
            temp[i] = temp[i * 2] + temp[i * 2 + 1];
        }
        
        // 处理奇数长度情况下的最后一个元素
        if (length % 2 == 1) {
            temp[length / 2] = temp[length - 1];
            length = length / 2 + 1; // 调整下一轮的长度
        } else {
            length = length / 2;
        }
    }
    
    double result = temp[0]; // 结果在第一个元素
    delete[] temp; // 释放临时数组
    
    return result;
}