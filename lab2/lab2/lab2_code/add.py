<<<<<<< HEAD
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import matplotlib as mpl

# 设置中文字体
plt.rcParams['font.sans-serif'] = ['SimHei']  # 用来正常显示中文标签
plt.rcParams['axes.unicode_minus'] = False    # 用来正常显示负号

# 定义莫兰迪柔和色系 - 更加舒适的视觉体验
morrandi_colors = [
    '#A1887F',  # 褐灰色 - 朴素累加
    '#90A4AE',  # 蓝灰色 - 两路累加
    '#9CCC65',  # 黄绿色 - 循环分治
    '#7986CB',  # 蓝紫色 - 四路累加
    '#FF8A65',  # 橙色 - 八路累加
    '#4DB6AC',  # 青绿色 - 十六路累加
    '#BA68C8',  # 紫色 - 递归分治
]

# 读取CSV文件
df = pd.read_csv('sum_performance.csv')

# 过滤掉有异常值的行（如包含负数的时间）
df = df[df.iloc[:, 1:8].min(axis=1) > 0]

# 定义共享的列和标签
columns_to_plot = ['朴素累加(ms)', '两路累加(ms)', '循环分治(ms)', 
                  '四路累加(ms)', '八路累加(ms)', '十六路累加(ms)', '递归分治(ms)']
labels = ['朴素累加', '两路累加', '循环分治', '四路累加', '八路累加', '十六路累加', '递归分治']

# 定义缓存边界
cache_sizes = [
    (4000, 'L1 缓存边界 (~32KB)'),
    (32000, 'L2 缓存边界 (~256KB)'),
    (1000000, 'L3 缓存边界 (~8MB)')
]

# 设置通用的图表参数
def setup_common_style(fig, ax):
    fig.patch.set_facecolor('#F5F5F5')
    ax.set_facecolor('#F5F5F5')
    ax.grid(True, alpha=0.3, linestyle='--', color='#CCCCCC')
    return ax

# 添加缓存边界线
def add_cache_lines(ax, min_size=0, max_size=float('inf')):
    for size, label in cache_sizes:
        if min_size <= size <= max_size and size <= ax.get_xlim()[1]:
            ax.axvline(x=size, color='#D32F2F', linestyle=':', alpha=0.7, linewidth=1.5)
            ax.text(size*1.05, ax.get_ylim()[1]*0.9, label, 
                   rotation=90, color='#D32F2F', alpha=0.9, fontsize=9)

# 添加水印
def add_watermark(fig, text):
    fig.text(0.99, 0.01, text, color='#888888', 
             fontsize=10, ha='right', va='bottom', alpha=0.7)

# 图表1: 对数坐标下的执行时间比较
fig1, ax1 = plt.subplots(figsize=(12, 8), dpi=100)
ax1 = setup_common_style(fig1, ax1)
ax1.set_title('不同数组大小的算法执行时间 (对数坐标)', fontsize=15, fontweight='bold', color='#333333')
ax1.set_xlabel('数组大小', fontsize=12, color='#333333')
ax1.set_ylabel('执行时间 (ms, 对数尺度)', fontsize=12, color='#333333')

for i, col in enumerate(columns_to_plot):
    # 按数组大小分组并计算中位数，避免异常值影响
    smoothed_data = df.groupby(pd.cut(df['数组大小'], 100))[col].median().reset_index()
    smoothed_data['数组大小'] = smoothed_data['数组大小'].apply(lambda x: x.mid)
    
    ax1.plot(smoothed_data['数组大小'], smoothed_data[col], 
             color=morrandi_colors[i], linewidth=2, label=labels[i])

ax1.set_xscale('log')
ax1.set_yscale('log')
ax1.legend(loc='upper left', fontsize=10)
add_cache_lines(ax1)
add_watermark(fig1, '向量求和算法执行时间分析 (对数坐标)')
plt.tight_layout()
plt.savefig('sum_execution_time_log.png', dpi=300, bbox_inches='tight')
plt.show()

# 图表2: 加速比对比
fig2, ax2 = plt.subplots(figsize=(12, 8), dpi=100)
ax2 = setup_common_style(fig2, ax2)
ax2.set_title('各优化算法相对于朴素累加的加速比', fontsize=15, fontweight='bold', color='#333333')
ax2.set_xlabel('数组大小', fontsize=12, color='#333333')
ax2.set_ylabel('加速比', fontsize=12, color='#333333')

# 速度比列
speedup_columns = ['两路加速比', '循环分治加速比', '四路加速比', 
                  '八路加速比', '十六路加速比', '递归分治加速比']
speedup_labels = ['两路累加', '循环分治', '四路累加', '八路累加', '十六路累加', '递归分治']

for i, col in enumerate(speedup_columns):
    # 同样使用分组中位数平滑数据
    smoothed_data = df.groupby(pd.cut(df['数组大小'], 100))[col].median().reset_index()
    smoothed_data['数组大小'] = smoothed_data['数组大小'].apply(lambda x: x.mid)
    
    ax2.plot(smoothed_data['数组大小'], smoothed_data[col], 
             color=morrandi_colors[i+1], linewidth=2, label=speedup_labels[i])

# 添加加速比=1的参考线
ax2.axhline(y=1, color='#999999', linestyle='--', linewidth=1.5)
ax2.set_xscale('log')
ax2.legend(loc='best', fontsize=10)
ax2.set_ylim(bottom=0, top=8)  # 限制y轴范围，忽略极端异常值
add_cache_lines(ax2)
add_watermark(fig2, '向量求和算法加速比分析')
plt.tight_layout()
plt.savefig('sum_speedup_ratios.png', dpi=300, bbox_inches='tight')
plt.show()

# 图表3: 小规模数据的性能比较
fig3, ax3 = plt.subplots(figsize=(12, 8), dpi=100)
ax3 = setup_common_style(fig3, ax3)
ax3.set_title('小规模数据的算法性能对比 (不超过10000个元素)', fontsize=15, fontweight='bold', color='#333333')
ax3.set_xlabel('数组大小', fontsize=12, color='#333333')
ax3.set_ylabel('执行时间 (ms)', fontsize=12, color='#333333')

# 过滤小规模数据
small_df = df[df['数组大小'] <= 10000]

for i, col in enumerate(columns_to_plot):
    ax3.plot(small_df['数组大小'], small_df[col], 
             color=morrandi_colors[i], linewidth=2, marker='o', markersize=3, label=labels[i])

ax3.legend(loc='upper left', fontsize=10)
add_cache_lines(ax3, max_size=10000)  # 只显示L1缓存边界
add_watermark(fig3, '小规模数据算法性能对比')
plt.tight_layout()
plt.savefig('sum_small_dataset_performance.png', dpi=300, bbox_inches='tight')
=======
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import matplotlib as mpl

# 设置中文字体
plt.rcParams['font.sans-serif'] = ['SimHei']  # 用来正常显示中文标签
plt.rcParams['axes.unicode_minus'] = False    # 用来正常显示负号

# 定义莫兰迪柔和色系 - 更加舒适的视觉体验
morrandi_colors = [
    '#A1887F',  # 褐灰色 - 朴素累加
    '#90A4AE',  # 蓝灰色 - 两路累加
    '#9CCC65',  # 黄绿色 - 循环分治
    '#7986CB',  # 蓝紫色 - 四路累加
    '#FF8A65',  # 橙色 - 八路累加
    '#4DB6AC',  # 青绿色 - 十六路累加
    '#BA68C8',  # 紫色 - 递归分治
]

# 读取CSV文件
df = pd.read_csv('sum_performance.csv')

# 过滤掉有异常值的行（如包含负数的时间）
df = df[df.iloc[:, 1:8].min(axis=1) > 0]

# 定义共享的列和标签
columns_to_plot = ['朴素累加(ms)', '两路累加(ms)', '循环分治(ms)', 
                  '四路累加(ms)', '八路累加(ms)', '十六路累加(ms)', '递归分治(ms)']
labels = ['朴素累加', '两路累加', '循环分治', '四路累加', '八路累加', '十六路累加', '递归分治']

# 定义缓存边界
cache_sizes = [
    (4000, 'L1 缓存边界 (~32KB)'),
    (32000, 'L2 缓存边界 (~256KB)'),
    (1000000, 'L3 缓存边界 (~8MB)')
]

# 设置通用的图表参数
def setup_common_style(fig, ax):
    fig.patch.set_facecolor('#F5F5F5')
    ax.set_facecolor('#F5F5F5')
    ax.grid(True, alpha=0.3, linestyle='--', color='#CCCCCC')
    return ax

# 添加缓存边界线
def add_cache_lines(ax, min_size=0, max_size=float('inf')):
    for size, label in cache_sizes:
        if min_size <= size <= max_size and size <= ax.get_xlim()[1]:
            ax.axvline(x=size, color='#D32F2F', linestyle=':', alpha=0.7, linewidth=1.5)
            ax.text(size*1.05, ax.get_ylim()[1]*0.9, label, 
                   rotation=90, color='#D32F2F', alpha=0.9, fontsize=9)

# 添加水印
def add_watermark(fig, text):
    fig.text(0.99, 0.01, text, color='#888888', 
             fontsize=10, ha='right', va='bottom', alpha=0.7)

# 图表1: 对数坐标下的执行时间比较
fig1, ax1 = plt.subplots(figsize=(12, 8), dpi=100)
ax1 = setup_common_style(fig1, ax1)
ax1.set_title('不同数组大小的算法执行时间 (对数坐标)', fontsize=15, fontweight='bold', color='#333333')
ax1.set_xlabel('数组大小', fontsize=12, color='#333333')
ax1.set_ylabel('执行时间 (ms, 对数尺度)', fontsize=12, color='#333333')

for i, col in enumerate(columns_to_plot):
    # 按数组大小分组并计算中位数，避免异常值影响
    smoothed_data = df.groupby(pd.cut(df['数组大小'], 100))[col].median().reset_index()
    smoothed_data['数组大小'] = smoothed_data['数组大小'].apply(lambda x: x.mid)
    
    ax1.plot(smoothed_data['数组大小'], smoothed_data[col], 
             color=morrandi_colors[i], linewidth=2, label=labels[i])

ax1.set_xscale('log')
ax1.set_yscale('log')
ax1.legend(loc='upper left', fontsize=10)
add_cache_lines(ax1)
add_watermark(fig1, '向量求和算法执行时间分析 (对数坐标)')
plt.tight_layout()
plt.savefig('sum_execution_time_log.png', dpi=300, bbox_inches='tight')
plt.show()

# 图表2: 加速比对比
fig2, ax2 = plt.subplots(figsize=(12, 8), dpi=100)
ax2 = setup_common_style(fig2, ax2)
ax2.set_title('各优化算法相对于朴素累加的加速比', fontsize=15, fontweight='bold', color='#333333')
ax2.set_xlabel('数组大小', fontsize=12, color='#333333')
ax2.set_ylabel('加速比', fontsize=12, color='#333333')

# 速度比列
speedup_columns = ['两路加速比', '循环分治加速比', '四路加速比', 
                  '八路加速比', '十六路加速比', '递归分治加速比']
speedup_labels = ['两路累加', '循环分治', '四路累加', '八路累加', '十六路累加', '递归分治']

for i, col in enumerate(speedup_columns):
    # 同样使用分组中位数平滑数据
    smoothed_data = df.groupby(pd.cut(df['数组大小'], 100))[col].median().reset_index()
    smoothed_data['数组大小'] = smoothed_data['数组大小'].apply(lambda x: x.mid)
    
    ax2.plot(smoothed_data['数组大小'], smoothed_data[col], 
             color=morrandi_colors[i+1], linewidth=2, label=speedup_labels[i])

# 添加加速比=1的参考线
ax2.axhline(y=1, color='#999999', linestyle='--', linewidth=1.5)
ax2.set_xscale('log')
ax2.legend(loc='best', fontsize=10)
ax2.set_ylim(bottom=0, top=8)  # 限制y轴范围，忽略极端异常值
add_cache_lines(ax2)
add_watermark(fig2, '向量求和算法加速比分析')
plt.tight_layout()
plt.savefig('sum_speedup_ratios.png', dpi=300, bbox_inches='tight')
plt.show()

# 图表3: 小规模数据的性能比较
fig3, ax3 = plt.subplots(figsize=(12, 8), dpi=100)
ax3 = setup_common_style(fig3, ax3)
ax3.set_title('小规模数据的算法性能对比 (不超过10000个元素)', fontsize=15, fontweight='bold', color='#333333')
ax3.set_xlabel('数组大小', fontsize=12, color='#333333')
ax3.set_ylabel('执行时间 (ms)', fontsize=12, color='#333333')

# 过滤小规模数据
small_df = df[df['数组大小'] <= 10000]

for i, col in enumerate(columns_to_plot):
    ax3.plot(small_df['数组大小'], small_df[col], 
             color=morrandi_colors[i], linewidth=2, marker='o', markersize=3, label=labels[i])

ax3.legend(loc='upper left', fontsize=10)
add_cache_lines(ax3, max_size=10000)  # 只显示L1缓存边界
add_watermark(fig3, '小规模数据算法性能对比')
plt.tight_layout()
plt.savefig('sum_small_dataset_performance.png', dpi=300, bbox_inches='tight')
>>>>>>> 815f0540f27cf2fa71b095ed6e225615570a3be0
plt.show()