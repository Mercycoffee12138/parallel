import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.font_manager as fm
import numpy as np

# 设置中文字体
plt.rcParams['font.sans-serif'] = ['SimHei']  # 用来正常显示中文标签
plt.rcParams['axes.unicode_minus'] = False    # 用来正常显示负号

# 读取CSV文件
df = pd.read_csv('matrix_performance_1_to_1500.csv')

# 创建图表
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(12, 10))

# 绘制算法执行时间比较
ax1.plot(df['矩阵大小'], df['平凡算法时间(ms)'], 'b-', label='平凡算法')
ax1.plot(df['矩阵大小'], df['优化算法时间(ms)'], 'r-', label='优化算法')
ax1.set_title('矩阵大小与算法执行时间关系')
ax1.set_xlabel('矩阵大小')
ax1.set_ylabel('执行时间(ms)')
ax1.legend()
ax1.grid(True)

# 对数据进行平滑处理以便更清晰地显示整体趋势
window_size = 30
df_smooth = df.copy()
df_smooth['平凡算法时间(ms)'] = df['平凡算法时间(ms)'].rolling(window=window_size, center=True).mean()
df_smooth['优化算法时间(ms)'] = df['优化算法时间(ms)'].rolling(window=window_size, center=True).mean()
df_smooth['加速比'] = df['加速比'].rolling(window=window_size, center=True).mean()

# 绘制加速比
ax2.plot(df['矩阵大小'], df['加速比'], 'g-', alpha=0.3, label='原始数据')
ax2.plot(df_smooth['矩阵大小'], df_smooth['加速比'], 'r-', label='平滑后的数据')
ax2.axhline(y=1, color='k', linestyle='--', label='加速比=1')
ax2.set_title('矩阵大小与加速比关系')
ax2.set_xlabel('矩阵大小')
ax2.set_ylabel('加速比')
ax2.legend()
ax2.grid(True)

# 裁剪极端值以使图表更可读
ax1.set_ylim(bottom=0, top=np.percentile(df['平凡算法时间(ms)'], 95))
ax2.set_ylim(bottom=0, top=np.percentile(df['加速比'], 95))

plt.tight_layout()
plt.savefig('matrix_performance_analysis.png', dpi=300)
plt.show()