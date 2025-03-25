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

# 设置通用的图表参数 - 使用白色背景
def setup_common_style(fig, ax):
    fig.patch.set_facecolor('white')  # 图表整体背景设为白色
    ax.set_facecolor('white')         # 坐标轴区域背景设为白色
    ax.grid(True, alpha=0.3, linestyle='--', color='#CCCCCC')
    ax.set_xscale('log')  # 所有图表都使用对数x轴
    return ax

# 添加缓存边界线
def add_cache_lines(ax, min_size=0, max_size=float('inf')):
    for size, label in cache_sizes:
        if min_size <= size <= max_size and size <= ax.get_xlim()[1]:
            ax.axvline(x=size, color='#D32F2F', linestyle=':', alpha=0.7, linewidth=1.5)
            # 对于对数坐标，调整文本位置
            ax.text(size*1.1, ax.get_ylim()[1]*0.9, label, 
                   rotation=90, color='#D32F2F', alpha=0.9, fontsize=9)

# 添加水印
def add_watermark(fig, text):
    fig.text(0.99, 0.01, text, color='#888888', 
             fontsize=10, ha='right', va='bottom', alpha=0.7)

# 图表1: 对数坐标下的执行时间比较
fig1, ax1 = plt.subplots(figsize=(12, 8), dpi=100)
ax1 = setup_common_style(fig1, ax1)
ax1.set_title('不同数组大小的算法执行时间 ', fontsize=15, fontweight='bold', color='#333333')
ax1.set_xlabel('数组大小 ', fontsize=12, color='#333333')
ax1.set_ylabel('执行时间 (ms)', fontsize=12, color='#333333')

for i, col in enumerate(columns_to_plot):
    # 直接使用原始数据点绘图，不进行分组平滑
    ax1.plot(df['数组大小'], df[col], 
             color=morrandi_colors[i], linewidth=1.5, marker='.', markersize=4, 
             label=labels[i], alpha=0.8)

ax1.set_yscale('log')  # y轴也使用对数
ax1.legend(loc='upper left', fontsize=10)
add_cache_lines(ax1)
add_watermark(fig1, '向量求和算法执行时间分析 ')
plt.tight_layout()
plt.savefig('sum_execution_time_log.png', dpi=300, bbox_inches='tight')
plt.show(block=False)  # 非阻塞显示

# 图表2: 加速比对比
fig2, ax2 = plt.subplots(figsize=(12, 8), dpi=100)
ax2 = setup_common_style(fig2, ax2)
ax2.set_title('各优化算法相对于朴素累加的加速比 ', fontsize=15, fontweight='bold', color='#333333')
ax2.set_xlabel('数组大小 ', fontsize=12, color='#333333')
ax2.set_ylabel('加速比', fontsize=12, color='#333333')

# 速度比列
speedup_columns = ['两路加速比', '循环分治加速比', '四路加速比', 
                  '八路加速比', '十六路加速比', '递归分治加速比']
speedup_labels = ['两路累加', '循环分治', '四路累加', '八路累加', '十六路累加', '递归分治']

for i, col in enumerate(speedup_columns):
    # 直接使用原始数据点绘图，不进行分组平滑
    ax2.plot(df['数组大小'], df[col], 
             color=morrandi_colors[i+1], linewidth=1.5, marker='.', markersize=4, 
             label=speedup_labels[i], alpha=0.8)

# 添加加速比=1的参考线
ax2.axhline(y=1, color='#999999', linestyle='--', linewidth=1.5)
ax2.legend(loc='best', fontsize=10)
ax2.set_ylim(bottom=0, top=8)  # 限制y轴范围，忽略极端异常值
add_cache_lines(ax2)
add_watermark(fig2, '向量求和算法加速比分析 ')
plt.tight_layout()
plt.savefig('sum_speedup_ratios.png', dpi=300, bbox_inches='tight')
plt.show(block=False)  # 非阻塞显示

# 图表3: 小规模数据的性能比较
fig3, ax3 = plt.subplots(figsize=(12, 8), dpi=100)
ax3 = setup_common_style(fig3, ax3)
ax3.set_title('小规模数据的算法性能对比 (不超过10000个元素)', fontsize=15, fontweight='bold', color='#333333')
ax3.set_xlabel('数组大小', fontsize=12, color='#333333')
ax3.set_ylabel('执行时间 (ms)', fontsize=12, color='#333333')

# 过滤小规模数据
small_df = df[df['数组大小'] <= 10000]

for i, col in enumerate(columns_to_plot):
    # 直接使用原始数据点绘图
    ax3.plot(small_df['数组大小'], small_df[col], 
             color=morrandi_colors[i], linewidth=1.5, marker='o', markersize=4, 
             label=labels[i], alpha=0.8)

ax3.legend(loc='upper left', fontsize=10)
add_cache_lines(ax3, max_size=10000)  # 只显示L1缓存边界
add_watermark(fig3, '小规模数据算法性能对比')
plt.tight_layout()
plt.savefig('sum_small_dataset_performance.png', dpi=300, bbox_inches='tight')
plt.show(block=False)  # 改为非阻塞，这样可以同时看到所有图表

# 新增图表: 大规模数据的性能比较
fig7, ax7 = plt.subplots(figsize=(12, 8), dpi=100)
ax7 = setup_common_style(fig7, ax7)
ax7.set_title('大规模数据的算法性能对比 (超过100000个元素)', fontsize=15, fontweight='bold', color='#333333')
ax7.set_xlabel('数组大小', fontsize=12, color='#333333')
ax7.set_ylabel('执行时间 (ms)', fontsize=12, color='#333333')

# 过滤大规模数据
large_df = df[df['数组大小'] >= 100000]

for i, col in enumerate(columns_to_plot):
    # 直接使用原始数据点绘图
    ax7.plot(large_df['数组大小'], large_df[col], 
             color=morrandi_colors[i], linewidth=1.5, marker='o', markersize=4, 
             label=labels[i], alpha=0.8)

ax7.legend(loc='upper left', fontsize=10)
# 添加L3缓存边界线
ax7.axvline(x=1000000, color='#D32F2F', linestyle=':', alpha=0.7, linewidth=1.5)
ax7.text(1050000, ax7.get_ylim()[1]*0.95, 'L3 缓存边界 (~8MB)', 
         color='#D32F2F', alpha=0.9, fontsize=9)

add_watermark(fig7, '大规模数据算法性能对比')
plt.tight_layout()
plt.savefig('sum_large_dataset_performance.png', dpi=300, bbox_inches='tight')
plt.show(block=False)  # 非阻塞显示

# 图表4: L1缓存边界详细分析
fig4, ax4 = plt.subplots(figsize=(12, 8), dpi=100)
ax4 = setup_common_style(fig4, ax4)
ax4.set_title('L1缓存边界附近的算法性能 (3000-5000元素)', fontsize=15, fontweight='bold', color='#333333')
ax4.set_xlabel('数组大小', fontsize=12, color='#333333')
ax4.set_ylabel('执行时间 (ms)', fontsize=12, color='#333333')

# 关闭对数坐标，使用线性坐标以便更好观察边界效应
ax4.set_xscale('linear')

# 过滤L1缓存边界附近的数据
l1_cache_df = df[(df['数组大小'] >= 3000) & (df['数组大小'] <= 5000)]

for i, col in enumerate(columns_to_plot):
    # 直接使用原始数据点绘图
    ax4.plot(l1_cache_df['数组大小'], l1_cache_df[col], 
             color=morrandi_colors[i], linewidth=1.5, marker='o', markersize=4, 
             label=labels[i], alpha=0.8)

ax4.legend(loc='upper left', fontsize=10)
# 添加L1缓存边界线
ax4.axvline(x=4000, color='#D32F2F', linestyle=':', alpha=0.7, linewidth=1.5)
ax4.text(4050, ax4.get_ylim()[1]*0.95, 'L1 缓存边界 (~32KB)', 
         color='#D32F2F', alpha=0.9, fontsize=9)

add_watermark(fig4, 'L1缓存边界性能分析')
plt.tight_layout()
plt.savefig('sum_l1_cache_boundary.png', dpi=300, bbox_inches='tight')
plt.show(block=False)  # 非阻塞显示

# 图表5: L2缓存边界详细分析
fig5, ax5 = plt.subplots(figsize=(12, 8), dpi=100)
ax5 = setup_common_style(fig5, ax5)
ax5.set_title('L2缓存边界附近的算法性能 (28000-36000元素)', fontsize=15, fontweight='bold', color='#333333')
ax5.set_xlabel('数组大小', fontsize=12, color='#333333')
ax5.set_ylabel('执行时间 (ms)', fontsize=12, color='#333333')

# 关闭对数坐标，使用线性坐标以便更好观察边界效应
ax5.set_xscale('linear')

# 过滤L2缓存边界附近的数据
l2_cache_df = df[(df['数组大小'] >= 28000) & (df['数组大小'] <= 36000)]

for i, col in enumerate(columns_to_plot):
    # 直接使用原始数据点绘图
    ax5.plot(l2_cache_df['数组大小'], l2_cache_df[col], 
             color=morrandi_colors[i], linewidth=1.5, marker='o', markersize=4, 
             label=labels[i], alpha=0.8)

ax5.legend(loc='upper left', fontsize=10)
# 添加L2缓存边界线
ax5.axvline(x=32000, color='#D32F2F', linestyle=':', alpha=0.7, linewidth=1.5)
ax5.text(32200, ax5.get_ylim()[1]*0.95, 'L2 缓存边界 (~256KB)', 
         color='#D32F2F', alpha=0.9, fontsize=9)

add_watermark(fig5, 'L2缓存边界性能分析')
plt.tight_layout()
plt.savefig('sum_l2_cache_boundary.png', dpi=300, bbox_inches='tight')
plt.show(block=False)  # 非阻塞显示

# 图表6: L3缓存边界详细分析
fig6, ax6 = plt.subplots(figsize=(12, 8), dpi=100)
ax6 = setup_common_style(fig6, ax6)
ax6.set_title('L3缓存边界附近的算法性能 (900000-1100000元素)', fontsize=15, fontweight='bold', color='#333333')
ax6.set_xlabel('数组大小', fontsize=12, color='#333333')
ax6.set_ylabel('执行时间 (ms)', fontsize=12, color='#333333')

# 关闭对数坐标，使用线性坐标以便更好观察边界效应
ax6.set_xscale('linear')

# 过滤L3缓存边界附近的数据
l3_cache_df = df[(df['数组大小'] >= 900000) & (df['数组大小'] <= 1100000)]

for i, col in enumerate(columns_to_plot):
    # 直接使用原始数据点绘图
    ax6.plot(l3_cache_df['数组大小'], l3_cache_df[col], 
             color=morrandi_colors[i], linewidth=1.5, marker='o', markersize=4, 
             label=labels[i], alpha=0.8)

ax6.legend(loc='upper left', fontsize=10)
# 添加L3缓存边界线
ax6.axvline(x=1000000, color='#D32F2F', linestyle=':', alpha=0.7, linewidth=1.5)
ax6.text(1010000, ax6.get_ylim()[1]*0.95, 'L3 缓存边界 (~8MB)', 
         color='#D32F2F', alpha=0.9, fontsize=9)

add_watermark(fig6, 'L3缓存边界性能分析')
plt.tight_layout()
plt.savefig('sum_l3_cache_boundary.png', dpi=300, bbox_inches='tight')
plt.show(block=False)  # 非阻塞显示

# 添加终极大规模数据性能比较 (线性坐标)
fig8, ax8 = plt.subplots(figsize=(12, 8), dpi=100)
# 直接使用通用样式设置
ax8 = setup_common_style(fig8, ax8)
# 重置为线性坐标
ax8.set_xscale('linear')
ax8.set_title('超大规模数据的算法性能对比 (线性坐标)', fontsize=15, fontweight='bold', color='#333333')
ax8.set_xlabel('数组大小', fontsize=12, color='#333333')
ax8.set_ylabel('执行时间 (ms)', fontsize=12, color='#333333')

# 过滤超大规模数据 (可能需要调整阈值以适应您的数据)
very_large_df = df[df['数组大小'] >= 1000000]

for i, col in enumerate(columns_to_plot):
    # 直接使用原始数据点绘图
    ax8.plot(very_large_df['数组大小'], very_large_df[col], 
             color=morrandi_colors[i], linewidth=1.5, marker='o', markersize=4, 
             label=labels[i], alpha=0.8)

ax8.legend(loc='upper left', fontsize=10)
add_watermark(fig8, '超大规模数据算法性能对比 (线性坐标)')
plt.tight_layout()
plt.savefig('sum_very_large_dataset_performance.png', dpi=300, bbox_inches='tight')
plt.show()  # 最后使用阻塞显示，防止程序结束

# 保持程序运行，直到用户按回车结束
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

# 设置通用的图表参数 - 使用白色背景
def setup_common_style(fig, ax):
    fig.patch.set_facecolor('white')  # 图表整体背景设为白色
    ax.set_facecolor('white')         # 坐标轴区域背景设为白色
    ax.grid(True, alpha=0.3, linestyle='--', color='#CCCCCC')
    ax.set_xscale('log')  # 所有图表都使用对数x轴
    return ax

# 添加缓存边界线
def add_cache_lines(ax, min_size=0, max_size=float('inf')):
    for size, label in cache_sizes:
        if min_size <= size <= max_size and size <= ax.get_xlim()[1]:
            ax.axvline(x=size, color='#D32F2F', linestyle=':', alpha=0.7, linewidth=1.5)
            # 对于对数坐标，调整文本位置
            ax.text(size*1.1, ax.get_ylim()[1]*0.9, label, 
                   rotation=90, color='#D32F2F', alpha=0.9, fontsize=9)

# 添加水印
def add_watermark(fig, text):
    fig.text(0.99, 0.01, text, color='#888888', 
             fontsize=10, ha='right', va='bottom', alpha=0.7)

# 图表1: 对数坐标下的执行时间比较
fig1, ax1 = plt.subplots(figsize=(12, 8), dpi=100)
ax1 = setup_common_style(fig1, ax1)
ax1.set_title('不同数组大小的算法执行时间 ', fontsize=15, fontweight='bold', color='#333333')
ax1.set_xlabel('数组大小 ', fontsize=12, color='#333333')
ax1.set_ylabel('执行时间 (ms)', fontsize=12, color='#333333')

for i, col in enumerate(columns_to_plot):
    # 直接使用原始数据点绘图，不进行分组平滑
    ax1.plot(df['数组大小'], df[col], 
             color=morrandi_colors[i], linewidth=1.5, marker='.', markersize=4, 
             label=labels[i], alpha=0.8)

ax1.set_yscale('log')  # y轴也使用对数
ax1.legend(loc='upper left', fontsize=10)
add_cache_lines(ax1)
add_watermark(fig1, '向量求和算法执行时间分析 ')
plt.tight_layout()
plt.savefig('sum_execution_time_log.png', dpi=300, bbox_inches='tight')
plt.show(block=False)  # 非阻塞显示

# 图表2: 加速比对比
fig2, ax2 = plt.subplots(figsize=(12, 8), dpi=100)
ax2 = setup_common_style(fig2, ax2)
ax2.set_title('各优化算法相对于朴素累加的加速比 ', fontsize=15, fontweight='bold', color='#333333')
ax2.set_xlabel('数组大小 ', fontsize=12, color='#333333')
ax2.set_ylabel('加速比', fontsize=12, color='#333333')

# 速度比列
speedup_columns = ['两路加速比', '循环分治加速比', '四路加速比', 
                  '八路加速比', '十六路加速比', '递归分治加速比']
speedup_labels = ['两路累加', '循环分治', '四路累加', '八路累加', '十六路累加', '递归分治']

for i, col in enumerate(speedup_columns):
    # 直接使用原始数据点绘图，不进行分组平滑
    ax2.plot(df['数组大小'], df[col], 
             color=morrandi_colors[i+1], linewidth=1.5, marker='.', markersize=4, 
             label=speedup_labels[i], alpha=0.8)

# 添加加速比=1的参考线
ax2.axhline(y=1, color='#999999', linestyle='--', linewidth=1.5)
ax2.legend(loc='best', fontsize=10)
ax2.set_ylim(bottom=0, top=8)  # 限制y轴范围，忽略极端异常值
add_cache_lines(ax2)
add_watermark(fig2, '向量求和算法加速比分析 ')
plt.tight_layout()
plt.savefig('sum_speedup_ratios.png', dpi=300, bbox_inches='tight')
plt.show(block=False)  # 非阻塞显示

# 图表3: 小规模数据的性能比较
fig3, ax3 = plt.subplots(figsize=(12, 8), dpi=100)
ax3 = setup_common_style(fig3, ax3)
ax3.set_title('小规模数据的算法性能对比 (不超过10000个元素)', fontsize=15, fontweight='bold', color='#333333')
ax3.set_xlabel('数组大小', fontsize=12, color='#333333')
ax3.set_ylabel('执行时间 (ms)', fontsize=12, color='#333333')

# 过滤小规模数据
small_df = df[df['数组大小'] <= 10000]

for i, col in enumerate(columns_to_plot):
    # 直接使用原始数据点绘图
    ax3.plot(small_df['数组大小'], small_df[col], 
             color=morrandi_colors[i], linewidth=1.5, marker='o', markersize=4, 
             label=labels[i], alpha=0.8)

ax3.legend(loc='upper left', fontsize=10)
add_cache_lines(ax3, max_size=10000)  # 只显示L1缓存边界
add_watermark(fig3, '小规模数据算法性能对比')
plt.tight_layout()
plt.savefig('sum_small_dataset_performance.png', dpi=300, bbox_inches='tight')
plt.show(block=False)  # 改为非阻塞，这样可以同时看到所有图表

# 新增图表: 大规模数据的性能比较
fig7, ax7 = plt.subplots(figsize=(12, 8), dpi=100)
ax7 = setup_common_style(fig7, ax7)
ax7.set_title('大规模数据的算法性能对比 (超过100000个元素)', fontsize=15, fontweight='bold', color='#333333')
ax7.set_xlabel('数组大小', fontsize=12, color='#333333')
ax7.set_ylabel('执行时间 (ms)', fontsize=12, color='#333333')

# 过滤大规模数据
large_df = df[df['数组大小'] >= 100000]

for i, col in enumerate(columns_to_plot):
    # 直接使用原始数据点绘图
    ax7.plot(large_df['数组大小'], large_df[col], 
             color=morrandi_colors[i], linewidth=1.5, marker='o', markersize=4, 
             label=labels[i], alpha=0.8)

ax7.legend(loc='upper left', fontsize=10)
# 添加L3缓存边界线
ax7.axvline(x=1000000, color='#D32F2F', linestyle=':', alpha=0.7, linewidth=1.5)
ax7.text(1050000, ax7.get_ylim()[1]*0.95, 'L3 缓存边界 (~8MB)', 
         color='#D32F2F', alpha=0.9, fontsize=9)

add_watermark(fig7, '大规模数据算法性能对比')
plt.tight_layout()
plt.savefig('sum_large_dataset_performance.png', dpi=300, bbox_inches='tight')
plt.show(block=False)  # 非阻塞显示

# 图表4: L1缓存边界详细分析
fig4, ax4 = plt.subplots(figsize=(12, 8), dpi=100)
ax4 = setup_common_style(fig4, ax4)
ax4.set_title('L1缓存边界附近的算法性能 (3000-5000元素)', fontsize=15, fontweight='bold', color='#333333')
ax4.set_xlabel('数组大小', fontsize=12, color='#333333')
ax4.set_ylabel('执行时间 (ms)', fontsize=12, color='#333333')

# 关闭对数坐标，使用线性坐标以便更好观察边界效应
ax4.set_xscale('linear')

# 过滤L1缓存边界附近的数据
l1_cache_df = df[(df['数组大小'] >= 3000) & (df['数组大小'] <= 5000)]

for i, col in enumerate(columns_to_plot):
    # 直接使用原始数据点绘图
    ax4.plot(l1_cache_df['数组大小'], l1_cache_df[col], 
             color=morrandi_colors[i], linewidth=1.5, marker='o', markersize=4, 
             label=labels[i], alpha=0.8)

ax4.legend(loc='upper left', fontsize=10)
# 添加L1缓存边界线
ax4.axvline(x=4000, color='#D32F2F', linestyle=':', alpha=0.7, linewidth=1.5)
ax4.text(4050, ax4.get_ylim()[1]*0.95, 'L1 缓存边界 (~32KB)', 
         color='#D32F2F', alpha=0.9, fontsize=9)

add_watermark(fig4, 'L1缓存边界性能分析')
plt.tight_layout()
plt.savefig('sum_l1_cache_boundary.png', dpi=300, bbox_inches='tight')
plt.show(block=False)  # 非阻塞显示

# 图表5: L2缓存边界详细分析
fig5, ax5 = plt.subplots(figsize=(12, 8), dpi=100)
ax5 = setup_common_style(fig5, ax5)
ax5.set_title('L2缓存边界附近的算法性能 (28000-36000元素)', fontsize=15, fontweight='bold', color='#333333')
ax5.set_xlabel('数组大小', fontsize=12, color='#333333')
ax5.set_ylabel('执行时间 (ms)', fontsize=12, color='#333333')

# 关闭对数坐标，使用线性坐标以便更好观察边界效应
ax5.set_xscale('linear')

# 过滤L2缓存边界附近的数据
l2_cache_df = df[(df['数组大小'] >= 28000) & (df['数组大小'] <= 36000)]

for i, col in enumerate(columns_to_plot):
    # 直接使用原始数据点绘图
    ax5.plot(l2_cache_df['数组大小'], l2_cache_df[col], 
             color=morrandi_colors[i], linewidth=1.5, marker='o', markersize=4, 
             label=labels[i], alpha=0.8)

ax5.legend(loc='upper left', fontsize=10)
# 添加L2缓存边界线
ax5.axvline(x=32000, color='#D32F2F', linestyle=':', alpha=0.7, linewidth=1.5)
ax5.text(32200, ax5.get_ylim()[1]*0.95, 'L2 缓存边界 (~256KB)', 
         color='#D32F2F', alpha=0.9, fontsize=9)

add_watermark(fig5, 'L2缓存边界性能分析')
plt.tight_layout()
plt.savefig('sum_l2_cache_boundary.png', dpi=300, bbox_inches='tight')
plt.show(block=False)  # 非阻塞显示

# 图表6: L3缓存边界详细分析
fig6, ax6 = plt.subplots(figsize=(12, 8), dpi=100)
ax6 = setup_common_style(fig6, ax6)
ax6.set_title('L3缓存边界附近的算法性能 (900000-1100000元素)', fontsize=15, fontweight='bold', color='#333333')
ax6.set_xlabel('数组大小', fontsize=12, color='#333333')
ax6.set_ylabel('执行时间 (ms)', fontsize=12, color='#333333')

# 关闭对数坐标，使用线性坐标以便更好观察边界效应
ax6.set_xscale('linear')

# 过滤L3缓存边界附近的数据
l3_cache_df = df[(df['数组大小'] >= 900000) & (df['数组大小'] <= 1100000)]

for i, col in enumerate(columns_to_plot):
    # 直接使用原始数据点绘图
    ax6.plot(l3_cache_df['数组大小'], l3_cache_df[col], 
             color=morrandi_colors[i], linewidth=1.5, marker='o', markersize=4, 
             label=labels[i], alpha=0.8)

ax6.legend(loc='upper left', fontsize=10)
# 添加L3缓存边界线
ax6.axvline(x=1000000, color='#D32F2F', linestyle=':', alpha=0.7, linewidth=1.5)
ax6.text(1010000, ax6.get_ylim()[1]*0.95, 'L3 缓存边界 (~8MB)', 
         color='#D32F2F', alpha=0.9, fontsize=9)

add_watermark(fig6, 'L3缓存边界性能分析')
plt.tight_layout()
plt.savefig('sum_l3_cache_boundary.png', dpi=300, bbox_inches='tight')
plt.show(block=False)  # 非阻塞显示

# 添加终极大规模数据性能比较 (线性坐标)
fig8, ax8 = plt.subplots(figsize=(12, 8), dpi=100)
# 直接使用通用样式设置
ax8 = setup_common_style(fig8, ax8)
# 重置为线性坐标
ax8.set_xscale('linear')
ax8.set_title('超大规模数据的算法性能对比 (线性坐标)', fontsize=15, fontweight='bold', color='#333333')
ax8.set_xlabel('数组大小', fontsize=12, color='#333333')
ax8.set_ylabel('执行时间 (ms)', fontsize=12, color='#333333')

# 过滤超大规模数据 (可能需要调整阈值以适应您的数据)
very_large_df = df[df['数组大小'] >= 1000000]

for i, col in enumerate(columns_to_plot):
    # 直接使用原始数据点绘图
    ax8.plot(very_large_df['数组大小'], very_large_df[col], 
             color=morrandi_colors[i], linewidth=1.5, marker='o', markersize=4, 
             label=labels[i], alpha=0.8)

ax8.legend(loc='upper left', fontsize=10)
add_watermark(fig8, '超大规模数据算法性能对比 (线性坐标)')
plt.tight_layout()
plt.savefig('sum_very_large_dataset_performance.png', dpi=300, bbox_inches='tight')
plt.show()  # 最后使用阻塞显示，防止程序结束

# 保持程序运行，直到用户按回车结束
>>>>>>> 815f0540f27cf2fa71b095ed6e225615570a3be0
input("按回车键结束程序...")