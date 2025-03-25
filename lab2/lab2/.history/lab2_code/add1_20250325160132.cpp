#include`
double sum_reduction(double* arr, int n) {
        // 创建一个临时数组用于存储中间结果
     double* temp = new double[n];
     memcpy(temp, arr, n * sizeof(double)); // 复制原始数据

        // 执行规约操作
     for (int m = n; m > 1; m = (m + 1) / 2) { // 向上取整确保处理奇数长度
     for (int i = 0; i < m / 2; i++) {
     temp[i] = temp[i * 2] + temp[i * 2 + 1];
     }
    
            // 处理奇数长度情况下的最后一个元素
     if (m % 2 == 1) {
     temp[m/2] = temp[m-1];
     m = m/2 + 1; // 调整下一轮的长度
     }
     }
     
    double result = temp[0]; // 结果在第一个元素
     delete[] temp; // 释放临时数组
     
     return result;
    }