<<<<<<< HEAD
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import matplotlib as mpl

# 设置中文字体
plt.rcParams['font.sans-serif'] = ['SimHei']  # 用来正常显示中文标签
plt.rcParams['axes.unicode_minus'] = False    # 用来正常显示负号

# 设置深色马卡龙色系配色 - 更深但仍然鲜艳的配色方案
deep_macaron_colors = [
    '#E84A5F',  # 深粉红色
    '#45ADA8',  # 深蓝绿色
    '#F7A278',  # 深杏色
    '#7AB98E',  # 深薄荷绿
]

# 读取CSV文件
df = pd.read_csv('matrix_performance.csv')

# 重命名列以匹配代码
df = df.rename(columns={
    '逐列访问时间(ms)': '平凡算法时间(ms)',
    '按行访问时间(ms)': '优化算法时间(ms)'
})

# 创建图表并设置柔和的背景色
plt.style.use('seaborn-v0_8-pastel')
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(12, 10), dpi=100)
fig.patch.set_facecolor('#F7F5F3')  # 米白色背景
ax1.set_facecolor('#F7F5F3')
ax2.set_facecolor('#F7F5F3')

# 绘制算法执行时间比较 - 使用深色马卡龙色系
ax1.plot(df['矩阵大小'], df['平凡算法时间(ms)'], color=deep_macaron_colors[0], linewidth=2.5, label='逐列访问算法')
ax1.plot(df['矩阵大小'], df['优化算法时间(ms)'], color=deep_macaron_colors[1], linewidth=2.5, label='按行访问算法')
ax1.set_title('矩阵大小与算法执行时间关系', fontsize=14, fontweight='bold', color='#444444')
ax1.set_xlabel('矩阵大小', fontsize=12, color='#444444')
ax1.set_ylabel('执行时间(ms)', fontsize=12, color='#444444')
ax1.legend(loc='upper left', fontsize=10)
ax1.grid(True, alpha=0.2, color='#DDDDDD')
ax1.tick_params(colors='#444444')  # 坐标轴刻度颜色

# 对数据进行平滑处理以便更清晰地显示整体趋势
window_size = 30
df_smooth = df.copy()
df_smooth['平凡算法时间(ms)'] = df['平凡算法时间(ms)'].rolling(window=window_size, center=True).mean()
df_smooth['优化算法时间(ms)'] = df['优化算法时间(ms)'].rolling(window=window_size, center=True).mean()
df_smooth['加速比'] = df['加速比'].rolling(window=window_size, center=True).mean()

# 绘制加速比 - 使用深色马卡龙色系
ax2.plot(df['矩阵大小'], df['加速比'], color=deep_macaron_colors[2], alpha=0.25, linewidth=1, label='原始数据')
ax2.plot(df_smooth['矩阵大小'], df_smooth['加速比'], color=deep_macaron_colors[3], linewidth=2.5, label='平滑后的数据')
ax2.axhline(y=1, color='#888888', linestyle='--', linewidth=1.5, label='加速比=1')
ax2.set_title('矩阵大小与加速比关系', fontsize=14, fontweight='bold', color='#444444')
ax2.set_xlabel('矩阵大小', fontsize=12, color='#444444')
ax2.set_ylabel('加速比', fontsize=12, color='#444444')
ax2.legend(loc='upper left', fontsize=10)
ax2.grid(True, alpha=0.2, color='#DDDDDD')
ax2.tick_params(colors='#444444')  # 坐标轴刻度颜色

# 裁剪极端值以使图表更可读
ax1.set_ylim(bottom=0, top=np.percentile(df['平凡算法时间(ms)'], 95) * 1.1)
ax2.set_ylim(bottom=0, top=np.percentile(df['加速比'], 95) * 1.1)

# 添加缓存线大小标记 - 使用更深的紫色
cache_color = '#8C82B4'  # 深紫色
cache_sizes = [32, 64, 256]  # 示例缓存大小（KB），对应的矩阵大小

for cache in cache_sizes:
    # 假设每个double占8字节，一个n*n的矩阵需要n*n*8字节
    matrix_size = int(np.sqrt(cache * 1024 / 8))
    if matrix_size <= max(df['矩阵大小']):
        ax1.axvline(x=matrix_size, color=cache_color, linestyle=':', linewidth=1.5, alpha=0.8)
        ax2.axvline(x=matrix_size, color=cache_color, linestyle=':', linewidth=1.5, alpha=0.8)
        
        # 添加带马卡龙风格背景的标注
        bbox_props = dict(boxstyle="round,pad=0.3", fc="#E8F0F7", ec=cache_color, alpha=0.8)
        ax1.text(matrix_size+5, ax1.get_ylim()[1]*0.85, f'L{cache_sizes.index(cache)+1}缓存\n{cache}KB', 
                 ha='left', va='top', rotation=0, color='#444444', bbox=bbox_props, fontsize=9)
        ax2.text(matrix_size+5, ax2.get_ylim()[1]*0.85, f'L{cache_sizes.index(cache)+1}缓存\n{cache}KB', 
                 ha='left', va='top', rotation=0, color='#444444', bbox=bbox_props, fontsize=9)

# 添加图表水印
fig.text(0.99, 0.01, '矩阵向量乘法性能分析', fontsize=10, color='#888888', ha='right', va='bottom', alpha=0.5)

# 添加一些时尚元素 - 为图表添加轻微的阴影效果
plt.tight_layout()

# 保存图表
plt.savefig('matrix_performance_analysis.png', dpi=300, bbox_inches='tight')
=======
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import matplotlib as mpl

# 设置中文字体
plt.rcParams['font.sans-serif'] = ['SimHei']  # 用来正常显示中文标签
plt.rcParams['axes.unicode_minus'] = False    # 用来正常显示负号

# 设置深色马卡龙色系配色 - 更深但仍然鲜艳的配色方案
deep_macaron_colors = [
    '#E84A5F',  # 深粉红色
    '#45ADA8',  # 深蓝绿色
    '#F7A278',  # 深杏色
    '#7AB98E',  # 深薄荷绿
]

# 读取CSV文件
df = pd.read_csv('matrix_performance.csv')

# 重命名列以匹配代码
df = df.rename(columns={
    '逐列访问时间(ms)': '平凡算法时间(ms)',
    '按行访问时间(ms)': '优化算法时间(ms)'
})

# 创建图表并设置柔和的背景色
plt.style.use('seaborn-v0_8-pastel')
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(12, 10), dpi=100)
fig.patch.set_facecolor('#F7F5F3')  # 米白色背景
ax1.set_facecolor('#F7F5F3')
ax2.set_facecolor('#F7F5F3')

# 绘制算法执行时间比较 - 使用深色马卡龙色系
ax1.plot(df['矩阵大小'], df['平凡算法时间(ms)'], color=deep_macaron_colors[0], linewidth=2.5, label='逐列访问算法')
ax1.plot(df['矩阵大小'], df['优化算法时间(ms)'], color=deep_macaron_colors[1], linewidth=2.5, label='按行访问算法')
ax1.set_title('矩阵大小与算法执行时间关系', fontsize=14, fontweight='bold', color='#444444')
ax1.set_xlabel('矩阵大小', fontsize=12, color='#444444')
ax1.set_ylabel('执行时间(ms)', fontsize=12, color='#444444')
ax1.legend(loc='upper left', fontsize=10)
ax1.grid(True, alpha=0.2, color='#DDDDDD')
ax1.tick_params(colors='#444444')  # 坐标轴刻度颜色

# 对数据进行平滑处理以便更清晰地显示整体趋势
window_size = 30
df_smooth = df.copy()
df_smooth['平凡算法时间(ms)'] = df['平凡算法时间(ms)'].rolling(window=window_size, center=True).mean()
df_smooth['优化算法时间(ms)'] = df['优化算法时间(ms)'].rolling(window=window_size, center=True).mean()
df_smooth['加速比'] = df['加速比'].rolling(window=window_size, center=True).mean()

# 绘制加速比 - 使用深色马卡龙色系
ax2.plot(df['矩阵大小'], df['加速比'], color=deep_macaron_colors[2], alpha=0.25, linewidth=1, label='原始数据')
ax2.plot(df_smooth['矩阵大小'], df_smooth['加速比'], color=deep_macaron_colors[3], linewidth=2.5, label='平滑后的数据')
ax2.axhline(y=1, color='#888888', linestyle='--', linewidth=1.5, label='加速比=1')
ax2.set_title('矩阵大小与加速比关系', fontsize=14, fontweight='bold', color='#444444')
ax2.set_xlabel('矩阵大小', fontsize=12, color='#444444')
ax2.set_ylabel('加速比', fontsize=12, color='#444444')
ax2.legend(loc='upper left', fontsize=10)
ax2.grid(True, alpha=0.2, color='#DDDDDD')
ax2.tick_params(colors='#444444')  # 坐标轴刻度颜色

# 裁剪极端值以使图表更可读
ax1.set_ylim(bottom=0, top=np.percentile(df['平凡算法时间(ms)'], 95) * 1.1)
ax2.set_ylim(bottom=0, top=np.percentile(df['加速比'], 95) * 1.1)

# 添加缓存线大小标记 - 使用更深的紫色
cache_color = '#8C82B4'  # 深紫色
cache_sizes = [32, 64, 256]  # 示例缓存大小（KB），对应的矩阵大小

for cache in cache_sizes:
    # 假设每个double占8字节，一个n*n的矩阵需要n*n*8字节
    matrix_size = int(np.sqrt(cache * 1024 / 8))
    if matrix_size <= max(df['矩阵大小']):
        ax1.axvline(x=matrix_size, color=cache_color, linestyle=':', linewidth=1.5, alpha=0.8)
        ax2.axvline(x=matrix_size, color=cache_color, linestyle=':', linewidth=1.5, alpha=0.8)
        
        # 添加带马卡龙风格背景的标注
        bbox_props = dict(boxstyle="round,pad=0.3", fc="#E8F0F7", ec=cache_color, alpha=0.8)
        ax1.text(matrix_size+5, ax1.get_ylim()[1]*0.85, f'L{cache_sizes.index(cache)+1}缓存\n{cache}KB', 
                 ha='left', va='top', rotation=0, color='#444444', bbox=bbox_props, fontsize=9)
        ax2.text(matrix_size+5, ax2.get_ylim()[1]*0.85, f'L{cache_sizes.index(cache)+1}缓存\n{cache}KB', 
                 ha='left', va='top', rotation=0, color='#444444', bbox=bbox_props, fontsize=9)

# 添加图表水印
fig.text(0.99, 0.01, '矩阵向量乘法性能分析', fontsize=10, color='#888888', ha='right', va='bottom', alpha=0.5)

# 添加一些时尚元素 - 为图表添加轻微的阴影效果
plt.tight_layout()

# 保存图表
plt.savefig('matrix_performance_analysis.png', dpi=300, bbox_inches='tight')
>>>>>>> 815f0540f27cf2fa71b095ed6e225615570a3be0
plt.show()