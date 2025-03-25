import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# 读取CSV数据
df = pd.read_csv('matrix_cache_analysis.csv')
stride_df = pd.read_csv('stride_analysis.csv')

# 创建图形
plt.figure(figsize=(18, 15))

# 图1: 矩阵大小与执行时间关系
plt.subplot(2, 2, 1)
plt.plot(df['矩阵大小'], df['列访问时间(ms)'], 'ro-', label='列访问 (缓存不友好)')
plt.plot(df['矩阵大小'], df['行访问时间(ms)'], 'bo-', label='行访问 (缓存友好)')
plt.xlabel('矩阵大小 (n)')
plt.ylabel('执行时间 (ms)')
plt.title('矩阵大小与执行时间关系')
plt.grid(True)
plt.legend()
plt.xscale('log', base=2)
plt.yscale('log')

# 图2: 矩阵大小与加速比关系
plt.subplot(2, 2, 2)
plt.plot(df['矩阵大小'], df['加速比'], 'go-')
plt.xlabel('矩阵大小 (n)')
plt.ylabel('加速比')
plt.title('矩阵大小与加速比关系')
plt.grid(True)
plt.xscale('log', base=2)

# 图3: 内存占用与加速比关系
plt.subplot(2, 2, 3)
plt.plot(df['内存占用(KB)'], df['加速比'], 'mo-')
plt.axvline(x=32, color='r', linestyle='--', label='L1缓存边界 (32KB)')
plt.axvline(x=256, color='g', linestyle='--', label='L2缓存边界 (256KB)')
plt.axvline(x=8192, color='b', linestyle='--', label='L3缓存边界 (8MB)')
plt.xlabel('内存占用 (KB)')
plt.ylabel('加速比')
plt.title('内存占用与加速比关系')
plt.grid(True)
plt.legend()
plt.xscale('log')

# 图4: 步长与执行时间关系
plt.subplot(2, 2, 4)
plt.plot(stride_df['步长'], stride_df['1024x1024矩阵访问时间(ms)'], 'co-')
plt.xlabel('内存访问步长')
plt.ylabel('执行时间 (ms)')
plt.title('内存访问步长与执行时间关系 (1024x1024矩阵)')
plt.grid(True)
plt.xscale('log', base=2)

plt.tight_layout()
plt.savefig('cache_analysis_results.png', dpi=300)
plt.show()

# 额外分析：找出性能突变点
print("分析性能突变点（可能的缓存边界）：")

# 计算加速比的变化率
df['加速比变化率'] = df['加速比'].pct_change()

# 找出变化率较大的点
threshold = 0.2  # 变化率阈值
potential_cache_boundaries = df[abs(df['加速比变化率']) > threshold]

if not potential_cache_boundaries.empty:
    print("可能的缓存边界点：")
    print(potential_cache_boundaries[['矩阵大小', '内存占用(KB)', '加速比', '加速比变化率']])
else:
    print("未发现明显的缓存边界点")