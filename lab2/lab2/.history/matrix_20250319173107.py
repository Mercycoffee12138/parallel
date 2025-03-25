import numpy as np
import matplotlib.pyplot as plt
import time

def naive_matrix_vector(matrix, vec):
    """
    平凡算法：逐列访问（缓存不友好）
    """
    n = len(matrix)
    result = [0.0] * n
    
    for col in range(n):
        sum_val = 0.0
        for row in range(n):
            sum_val += matrix[row][col] * vec[row]  # 按列访问
        result[col] = sum_val
        
    return result

def optimized_matrix_vector(matrix, vec):
    """
    优化算法：按行访问（缓存友好）
    """
    n = len(matrix)
    result = [0.0] * n
    
    for row in range(n):
        vec_val = vec[row]  # 缓存向量元素
        for col in range(n):
            result[col] += matrix[row][col] * vec_val  # 按行访问
            
    return result

def measure_performance(sizes):
    """
    测量不同矩阵大小下两种算法的执行时间
    """
    naive_times = []
    optimized_times = []
    
    for n in sizes:
        # 创建随机矩阵和向量
        matrix = [[np.random.random() for _ in range(n)] for _ in range(n)]
        vector = [np.random.random() for _ in range(n)]
        
        # 测量平凡算法时间
        start_time = time.time()
        naive_matrix_vector(matrix, vector)
        naive_time = time.time() - start_time
        naive_times.append(naive_time)
        
        # 测量优化算法时间
        start_time = time.time()
        optimized_matrix_vector(matrix, vector)
        optimized_time = time.time() - start_time
        optimized_times.append(optimized_time)
        
        print(f"矩阵大小: {n}x{n}")
        print(f"平凡算法 (按列访问): {naive_time:.6f} 秒")
        print(f"优化算法 (按行访问): {optimized_time:.6f} 秒")
        print(f"加速比: {naive_time/optimized_time:.2f}x\n")
    
    return naive_times, optimized_times

def plot_results(sizes, naive_times, optimized_times):
    """
    绘制性能比较图
    """
    plt.figure(figsize=(12, 10))
    
    # 绘制执行时间对比
    plt.subplot(2, 1, 1)
    plt.plot(sizes, naive_times, 'o-', color='red', label='按列访问 (缓存不友好)')
    plt.plot(sizes, optimized_times, 's-', color='blue', label='按行访问 (缓存友好)')
    plt.xlabel('矩阵大小 (n)')
    plt.ylabel('执行时间 (秒)')
    plt.title('矩阵-向量乘法: 执行时间比较')
    plt.grid(True)
    plt.legend()
    
    # 绘制加速比
    plt.subplot(2, 1, 2)
    speedups = [naive_times[i]/optimized_times[i] for i in range(len(sizes))]
    plt.plot(sizes, speedups, 'o-', color='green')
    plt.xlabel('矩阵大小 (n)')
    plt.ylabel('加速比')
    plt.title('优化算法相对于平凡算法的加速比')
    plt.grid(True)
    
    plt.tight_layout()
    plt.savefig('matrix_vector_performance.png')
    plt.show()

if __name__ == "__main__":
    # 测试不同大小的矩阵
    sizes = [10,20,30,40,50,60,70,80,90,100,200,300,400, 500,600, 1000, 1500, 2000, 2500, 3000]
    
    # 对于较大的矩阵，可能需要减少测试规模
    print("开始性能测量...")
    naive_times, optimized_times = measure_performance(sizes)
    
    # 绘制结果
    plot_results(sizes, naive_times, optimized_times)
    
    # 验证两种算法的结果是否一致
    n = 10
    matrix = [[np.random.random() for _ in range(n)] for _ in range(n)]
    vector = [np.random.random() for _ in range(n)]
    
    result1 = naive_matrix_vector(matrix, vector)
    result2 = optimized_matrix_vector(matrix, vector)
    
    # 检查结果是否一致（考虑浮点误差）
    if np.allclose(result1, result2, rtol=1e-10):
        print("验证通过：两种算法产生相同结果")
    else:
        print("验证失败：两种算法结果不同")