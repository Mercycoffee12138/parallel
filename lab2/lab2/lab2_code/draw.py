import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns
import matplotlib

# 设置matplotlib支持中文显示
matplotlib.rcParams['font.sans-serif'] = ['SimHei']  # 指定默认字体为黑体
matplotlib.rcParams['axes.unicode_minus'] = False    # 解决保存图像时负号'-'显示为方块的问题

# 读取CSV文件 - 指定编码
df = pd.read_csv('sum_performance_001.csv', encoding='utf-8')  # 尝试utf-8编码

# 读取CSV文件
df = pd.read_csv('sum_performance_001.csv')

# 设置更好的可视化风格
plt.style.use('ggplot')
sns.set_palette("colorblind")
plt.rcParams.update({'font.size': 12})

# 执行所有分析
error_columns = ['两路相对误差', '循环分治相对误差', '四路相对误差', 
                '八路相对误差', '十六路相对误差', '递归分治相对误差']


# 创建一个函数来绘制相对误差的图表
def plot_relative_errors():
    # 创建一个图表
    fig, axes = plt.subplots(2, 1, figsize=(12, 14), sharex=True)
    
    # 第一个子图：标准刻度
    ax1 = axes[0]
    ax1.set_title('不同算法的相对误差 (标准刻度)', fontsize=14)
    ax1.plot(df['数组大小'], df['两路相对误差'], label='两路累加')
    ax1.plot(df['数组大小'], df['循环分治相对误差'], label='循环分治')
    ax1.plot(df['数组大小'], df['四路相对误差'], label='四路累加')
    ax1.plot(df['数组大小'], df['八路相对误差'], label='八路累加')
    ax1.plot(df['数组大小'], df['十六路相对误差'], label='十六路累加')
    ax1.plot(df['数组大小'], df['递归分治相对误差'], label='递归分治')
    ax1.set_ylabel('相对误差', fontsize=12)
    ax1.legend(loc='upper left')
    ax1.grid(True, alpha=0.3)
    
    # 第二个子图：对数刻度，更好地显示小值
    ax2 = axes[1]
    ax2.set_title('不同算法的相对误差 (对数刻度)', fontsize=14)
    ax2.semilogy(df['数组大小'], df['两路相对误差'], label='两路累加')
    ax2.semilogy(df['数组大小'], df['循环分治相对误差'], label='循环分治')
    ax2.semilogy(df['数组大小'], df['四路相对误差'], label='四路累加')
    ax2.semilogy(df['数组大小'], df['八路相对误差'], label='八路累加')
    ax2.semilogy(df['数组大小'], df['十六路相对误差'], label='十六路累加')
    ax2.semilogy(df['数组大小'], df['递归分治相对误差'], label='递归分治')
    ax2.set_ylabel('相对误差 (对数刻度)', fontsize=12)
    ax2.set_xlabel('数组大小', fontsize=12)
    ax2.legend(loc='upper left')
    ax2.grid(True, alpha=0.3)
    
    plt.tight_layout()
    plt.savefig('relative_errors_comparison.png', dpi=300)
    plt.show()

# 创建一个函数来分析缓存边界附近的精度差异
def plot_cache_boundaries_errors():
    # L1缓存边界附近 (约4K元素 = 32KB)
    l1_min, l1_max = 3500, 4500
    # L2缓存边界附近 (约32K元素 = 256KB)
    l2_min, l2_max = 30000, 34000
    # L3缓存边界附近 (约1M元素 = 8MB)
    l3_min, l3_max = 950000, 1050000
    
    fig, axes = plt.subplots(3, 1, figsize=(12, 18))
    
    # L1缓存边界附近
    l1_df = df[(df['数组大小'] >= l1_min) & (df['数组大小'] <= l1_max)]
    axes[0].set_title(f'L1缓存边界附近的相对误差 ({l1_min} - {l1_max} 元素)', fontsize=14)
    axes[0].semilogy(l1_df['数组大小'], l1_df['两路相对误差'], 'o-', label='两路累加')
    axes[0].semilogy(l1_df['数组大小'], l1_df['循环分治相对误差'], 'o-', label='循环分治')
    axes[0].semilogy(l1_df['数组大小'], l1_df['四路相对误差'], 'o-', label='四路累加')
    axes[0].semilogy(l1_df['数组大小'], l1_df['八路相对误差'], 'o-', label='八路累加')
    axes[0].semilogy(l1_df['数组大小'], l1_df['十六路相对误差'], 'o-', label='十六路累加')
    axes[0].semilogy(l1_df['数组大小'], l1_df['递归分治相对误差'], 'o-', label='递归分治')
    axes[0].set_ylabel('相对误差 (对数刻度)', fontsize=12)
    axes[0].legend(loc='upper left')
    axes[0].grid(True, alpha=0.3)
    
    # L2缓存边界附近
    l2_df = df[(df['数组大小'] >= l2_min) & (df['数组大小'] <= l2_max)]
    axes[1].set_title(f'L2缓存边界附近的相对误差 ({l2_min} - {l2_max} 元素)', fontsize=14)
    axes[1].semilogy(l2_df['数组大小'], l2_df['两路相对误差'], 'o-', label='两路累加')
    axes[1].semilogy(l2_df['数组大小'], l2_df['循环分治相对误差'], 'o-', label='循环分治')
    axes[1].semilogy(l2_df['数组大小'], l2_df['四路相对误差'], 'o-', label='四路累加')
    axes[1].semilogy(l2_df['数组大小'], l2_df['八路相对误差'], 'o-', label='八路累加')
    axes[1].semilogy(l2_df['数组大小'], l2_df['十六路相对误差'], 'o-', label='十六路累加')
    axes[1].semilogy(l2_df['数组大小'], l2_df['递归分治相对误差'], 'o-', label='递归分治')
    axes[1].set_ylabel('相对误差 (对数刻度)', fontsize=12)
    axes[1].legend(loc='upper left')
    axes[1].grid(True, alpha=0.3)
    
    # L3缓存边界附近
    l3_df = df[(df['数组大小'] >= l3_min) & (df['数组大小'] <= l3_max)]
    axes[2].set_title(f'L3缓存边界附近的相对误差 ({l3_min} - {l3_max} 元素)', fontsize=14)
    axes[2].semilogy(l3_df['数组大小'], l3_df['两路相对误差'], 'o-', label='两路累加')
    axes[2].semilogy(l3_df['数组大小'], l3_df['循环分治相对误差'], 'o-', label='循环分治')
    axes[2].semilogy(l3_df['数组大小'], l3_df['四路相对误差'], 'o-', label='四路累加')
    axes[2].semilogy(l3_df['数组大小'], l3_df['八路相对误差'], 'o-', label='八路累加')
    axes[2].semilogy(l3_df['数组大小'], l3_df['十六路相对误差'], 'o-', label='十六路累加')
    axes[2].semilogy(l3_df['数组大小'], l3_df['递归分治相对误差'], 'o-', label='递归分治')
    axes[2].set_ylabel('相对误差 (对数刻度)', fontsize=12)
    axes[2].set_xlabel('数组大小', fontsize=12)
    axes[2].legend(loc='upper left')
    axes[2].grid(True, alpha=0.3)
    
    plt.tight_layout()
    plt.savefig('cache_boundaries_errors.png', dpi=300)
    plt.show()

# 分析误差随数组大小变化的趋势
def plot_error_trends():
    # 创建数组大小的对数刻度的箱线图
    fig, ax = plt.subplots(figsize=(14, 8))
    
    # 创建数组大小的bins（按对数刻度）
    log_bins = np.logspace(np.log10(100), np.log10(10000000), 20)
    
    # 给每个数据点分配一个bin
    df['size_bin'] = pd.cut(df['数组大小'], bins=log_bins, labels=False)
    
    # 创建一个数据集，方便箱线图绘制
    boxplot_data = []
    bin_labels = []
    
    for bin_idx in sorted(df['size_bin'].unique()):
        bin_df = df[df['size_bin'] == bin_idx]
        if not bin_df.empty:
            bin_start = bin_df['数组大小'].min()
            bin_end = bin_df['数组大小'].max()
            bin_label = f"{bin_start:.0f}-{bin_end:.0f}"
            bin_labels.append(bin_label)
            
            # 收集该bin中的所有算法误差
            for algo in ['两路相对误差', '循环分治相对误差', '四路相对误差', 
                        '八路相对误差', '十六路相对误差', '递归分治相对误差']:
                for error in bin_df[algo]:
                    if error > 0:  # 避免log(0)
                        boxplot_data.append([bin_label, algo, error])
    
    # 转换为DataFrame
    boxplot_df = pd.DataFrame(boxplot_data, columns=['Bin', 'Algorithm', 'Error'])
    
    # 绘制箱线图
    sns.boxplot(x='Bin', y='Error', hue='Algorithm', data=boxplot_df, ax=ax, palette='colorblind')
    ax.set_yscale('log')
    ax.set_title('不同数组大小区间的相对误差分布', fontsize=14)
    ax.set_xlabel('数组大小区间', fontsize=12)
    ax.set_ylabel('相对误差 (对数刻度)', fontsize=12)
    plt.xticks(rotation=45)
    plt.legend(title='算法')
    plt.tight_layout()
    plt.savefig('error_distribution_by_size.png', dpi=300)
    plt.show()

# 比较每种算法的总体误差性能
def plot_overall_error_comparison():
    # 计算每种算法的平均误差
    error_columns = ['两路相对误差', '循环分治相对误差', '四路相对误差', 
                    '八路相对误差', '十六路相对误差', '递归分治相对误差']
    
    # 去掉误差为0的记录 (可能是无效记录)
    filtered_df = df[df[error_columns].sum(axis=1) > 0]
    
    # 计算平均值和标准差
    means = filtered_df[error_columns].mean()
    stds = filtered_df[error_columns].std()
    
    # 计算中位数
    medians = filtered_df[error_columns].median()
    
    # 计算数据范围 (1%分位数到99%分位数)
    q1 = filtered_df[error_columns].quantile(0.01)
    q99 = filtered_df[error_columns].quantile(0.99)
    
    # 绘制平均误差和分布
    fig, axes = plt.subplots(1, 2, figsize=(14, 7))
    
    # 平均值图表 (对数刻度)
    ax1 = axes[0]
    bars = ax1.bar(means.index, means.values)
    ax1.set_yscale('log')
    ax1.set_title('每种算法的平均相对误差', fontsize=14)
    ax1.set_ylabel('平均相对误差 (对数刻度)', fontsize=12)
    ax1.set_xticklabels([label.split('相对误差')[0] for label in means.index], rotation=45)
    
    # 在柱状图上添加误差棒 (平均值 ± 标准差)
    for i, bar in enumerate(bars):
        height = means.values[i]
        ax1.text(bar.get_x() + bar.get_width()/2., height,
                f'{height:.2e}', ha='center', va='bottom', rotation=0, fontsize=8)
    
    # 箱线图分布
    ax2 = axes[1]
    # 为了画箱线图，需要将数据重组
    data_for_boxplot = pd.melt(filtered_df, value_vars=error_columns, 
                               var_name='Algorithm', value_name='Error')
    sns.boxplot(x='Algorithm', y='Error', data=data_for_boxplot, ax=ax2, palette='colorblind')
    ax2.set_yscale('log')
    ax2.set_title('误差分布', fontsize=14)
    ax2.set_ylabel('相对误差 (对数刻度)', fontsize=12)
    ax2.set_xticklabels([label.split('相对误差')[0] for label in means.index], rotation=45)
    
    plt.tight_layout()
    plt.savefig('overall_error_comparison.png', dpi=300)
    plt.show()

# 添加一个函数专门分析小数组区间的相对误差
def plot_small_array_errors():
    # 定义小数组范围 (100-2000元素)
    small_min, small_max = 100, 2000
    
    # 筛选小数组数据
    small_df = df[(df['数组大小'] >= small_min) & (df['数组大小'] <= small_max)]
    
    # 创建一个2x1的子图布局
    fig, axes = plt.subplots(2, 1, figsize=(12, 14))
    
    # 第一个子图：标准刻度下的小数组误差
    ax1 = axes[0]
    ax1.set_title(f'小数组区间的相对误差 ({small_min}-{small_max}元素)', fontsize=14)
    ax1.plot(small_df['数组大小'], small_df['两路相对误差'], 'o-', label='两路累加')
    ax1.plot(small_df['数组大小'], small_df['循环分治相对误差'], 'o-', label='循环分治')
    ax1.plot(small_df['数组大小'], small_df['四路相对误差'], 'o-', label='四路累加')
    ax1.plot(small_df['数组大小'], small_df['八路相对误差'], 'o-', label='八路累加')
    ax1.plot(small_df['数组大小'], small_df['十六路相对误差'], 'o-', label='十六路累加')
    ax1.plot(small_df['数组大小'], small_df['递归分治相对误差'], 'o-', label='递归分治')
    ax1.set_ylabel('相对误差', fontsize=12)
    ax1.legend(loc='best')
    ax1.grid(True, alpha=0.3)
    
    # 第二个子图：对数刻度下的小数组误差
    ax2 = axes[1]
    ax2.set_title(f'小数组区间的相对误差 (对数刻度)', fontsize=14)
    ax2.semilogy(small_df['数组大小'], small_df['两路相对误差'], 'o-', label='两路累加')
    ax2.semilogy(small_df['数组大小'], small_df['循环分治相对误差'], 'o-', label='循环分治')
    ax2.semilogy(small_df['数组大小'], small_df['四路相对误差'], 'o-', label='四路累加')
    ax2.semilogy(small_df['数组大小'], small_df['八路相对误差'], 'o-', label='八路累加')
    ax2.semilogy(small_df['数组大小'], small_df['十六路相对误差'], 'o-', label='十六路累加')
    ax2.semilogy(small_df['数组大小'], small_df['递归分治相对误差'], 'o-', label='递归分治')
    ax2.set_ylabel('相对误差 (对数刻度)', fontsize=12)
    ax2.set_xlabel('数组大小', fontsize=12)
    ax2.legend(loc='best')
    ax2.grid(True, alpha=0.3)
    
    plt.tight_layout()
    plt.savefig('small_array_errors.png', dpi=300)
    plt.show()
    
    # 添加一个详细的热图来显示最小区间的误差模式
    plt.figure(figsize=(14, 8))
    # 准备热图数据 - 转置数据以便算法作为y轴
    heatmap_data = small_df[['数组大小'] + error_columns].set_index('数组大小').T
    # 使用更好的可视化配色方案
    cmap = sns.diverging_palette(220, 10, as_cmap=True)
    # 绘制热图
    sns.heatmap(heatmap_data, cmap=cmap, center=0, 
                annot=False, fmt='.2e', linewidths=.5, 
                cbar_kws={'label': '相对误差'})
    plt.title('小数组区间不同算法的相对误差热图', fontsize=14)
    plt.ylabel('算法', fontsize=12)
    plt.xlabel('数组大小', fontsize=12)
    plt.xticks(rotation=45)
    plt.tight_layout()
    plt.savefig('small_array_errors_heatmap.png', dpi=300)
    plt.show()

# 分析极小数组区间 (100-500元素)
def plot_tiny_array_errors():
    tiny_min, tiny_max = 100, 500
    tiny_df = df[(df['数组大小'] >= tiny_min) & (df['数组大小'] <= tiny_max)]
    
    # 创建一个带标记点的折线图，以便清晰显示每个数据点
    plt.figure(figsize=(14, 8))
    for col in error_columns:
        plt.plot(tiny_df['数组大小'], tiny_df[col], 'o-', label=col.split('相对误差')[0])
    
    plt.title(f'极小数组区间的相对误差比较 ({tiny_min}-{tiny_max}元素)', fontsize=14)
    plt.ylabel('相对误差', fontsize=12)
    plt.xlabel('数组大小', fontsize=12)
    plt.grid(True, alpha=0.3)
    plt.legend(loc='best')
    plt.tight_layout()
    plt.savefig('tiny_array_errors.png', dpi=300)
    plt.show()
    
    # 对数刻度版本
    plt.figure(figsize=(14, 8))
    for col in error_columns:
        plt.semilogy(tiny_df['数组大小'], tiny_df[col], 'o-', label=col.split('相对误差')[0])
    
    plt.title(f'极小数组区间的相对误差比较 (对数刻度, {tiny_min}-{tiny_max}元素)', fontsize=14)
    plt.ylabel('相对误差 (对数刻度)', fontsize=12)
    plt.xlabel('数组大小', fontsize=12)
    plt.grid(True, alpha=0.3)
    plt.legend(loc='best')
    plt.tight_layout()
    plt.savefig('tiny_array_errors_log.png', dpi=300)
    plt.show()

# 添加整体热力图分析
def plot_overall_heatmap():
    # 创建一个更大的图表用于热力图
    plt.figure(figsize=(18, 10))
    
    # 为了使热力图更加直观，我们对数据进行对数变换
    # 先对数组大小分组，避免热力图过于密集
    # 创建划分区间
    size_bins = [100, 200, 500, 1000, 2000, 5000, 10000, 20000, 50000, 100000, 500000, 1000000, 5000000, 10000000]
    size_labels = [f"{size_bins[i]}-{size_bins[i+1]}" for i in range(len(size_bins)-1)]
    
    # 将数组大小分配到各个区间
    df['size_group'] = pd.cut(df['数组大小'], bins=size_bins, labels=size_labels, right=False)
    
    # 计算每个组和每种算法的平均相对误差
    heatmap_df = df.groupby('size_group')[error_columns].mean().reset_index()
    
    # 将数据透视为适合热力图的格式
    heatmap_pivot = heatmap_df.set_index('size_group')[error_columns]
    
    # 使用更直观的标签名称
    heatmap_pivot.columns = [col.split('相对误差')[0] for col in heatmap_pivot.columns]
    
    # 对数变换误差值以便更好地可视化
    # 避免log(0)问题，添加一个极小值
    heatmap_data = np.log10(heatmap_pivot.values + 1e-20)
    
    # 创建热图
    ax = plt.gca()
    im = ax.imshow(heatmap_data.T, cmap='YlOrRd', aspect='auto')
    
    # 设置轴标签
    ax.set_yticks(np.arange(len(heatmap_pivot.columns)))
    ax.set_yticklabels(heatmap_pivot.columns)
    ax.set_xticks(np.arange(len(heatmap_pivot.index)))
    ax.set_xticklabels(heatmap_pivot.index)
    plt.xticks(rotation=45, ha='right')
    
    # 添加颜色条并设置标签
    cbar = plt.colorbar(im)
    cbar.set_label('相对误差 (log10 刻度)')
    
    # 为每个单元格添加文本标注
    for i in range(len(heatmap_pivot.index)):
        for j in range(len(heatmap_pivot.columns)):
            original_value = heatmap_pivot.iloc[i, j]
            text = ax.text(i, j, f'{original_value:.2e}',
                          ha="center", va="center", color="black" if heatmap_data.T[j, i] < -13 else "white",
                          fontsize=8)
    
    plt.title('不同数组大小区间和算法的相对误差热力图', fontsize=14)
    plt.xlabel('数组大小区间', fontsize=12)
    plt.ylabel('算法', fontsize=12)
    plt.tight_layout()
    plt.savefig('overall_error_heatmap.png', dpi=300)
    plt.show()
    
    # 添加一个特别关注在小数组区间的热力图
    small_size_bins = [100, 200, 300, 400, 500, 750, 1000, 1500, 2000, 3000, 4000, 5000]
    small_size_labels = [f"{small_size_bins[i]}-{small_size_bins[i+1]}" for i in range(len(small_size_bins)-1)]
    
    # 筛选小数组数据
    small_df = df[df['数组大小'] <= 5000].copy()
    small_df['size_group'] = pd.cut(small_df['数组大小'], bins=small_size_bins, labels=small_size_labels, right=False)
    
    # 计算每个组和每种算法的平均相对误差
    small_heatmap_df = small_df.groupby('size_group')[error_columns].mean().reset_index()
    
    # 将数据透视为适合热力图的格式
    small_heatmap_pivot = small_heatmap_df.set_index('size_group')[error_columns]
    
    # 使用更直观的标签名称
    small_heatmap_pivot.columns = [col.split('相对误差')[0] for col in small_heatmap_pivot.columns]
    
    # 对数变换误差值以便更好地可视化
    # 避免log(0)问题，添加一个极小值
    small_heatmap_data = np.log10(small_heatmap_pivot.values + 1e-20)
    
    # 创建新图表
    plt.figure(figsize=(18, 10))
    
    # 创建热图
    ax = plt.gca()
    im = ax.imshow(small_heatmap_data.T, cmap='YlOrRd', aspect='auto')
    
    # 设置轴标签
    ax.set_yticks(np.arange(len(small_heatmap_pivot.columns)))
    ax.set_yticklabels(small_heatmap_pivot.columns)
    ax.set_xticks(np.arange(len(small_heatmap_pivot.index)))
    ax.set_xticklabels(small_heatmap_pivot.index)
    plt.xticks(rotation=45, ha='right')
    
    # 添加颜色条并设置标签
    cbar = plt.colorbar(im)
    cbar.set_label('相对误差 (log10 刻度)')
    
    # 为每个单元格添加文本标注
    for i in range(len(small_heatmap_pivot.index)):
        for j in range(len(small_heatmap_pivot.columns)):
            original_value = small_heatmap_pivot.iloc[i, j]
            text = ax.text(i, j, f'{original_value:.2e}',
                          ha="center", va="center", color="black" if small_heatmap_data.T[j, i] < -13 else "white",
                          fontsize=8)
    
    plt.title('小数组区间和不同算法的相对误差热力图 (<=5000元素)', fontsize=14)
    plt.xlabel('数组大小区间', fontsize=12)
    plt.ylabel('算法', fontsize=12)
    plt.tight_layout()
    plt.savefig('small_array_overall_heatmap.png', dpi=300)
    plt.show()

# 首先分析小数组区间
plot_tiny_array_errors()      # 分析极小数组 (100-500)
plot_small_array_errors()     # 分析小数组 (100-2000)

# 执行所有分析
plot_relative_errors()
plot_cache_boundaries_errors()
plot_error_trends()
plot_overall_error_comparison()
plot_overall_heatmap()        # 添加整体热力图分析