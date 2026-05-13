# VisualStatistics

一个优雅的概率分布可视化应用，通过粒子动画和交互式图表，让抽象的统计学概念变得直观易懂。

## ✨ 特性

- **12种概率分布**：涵盖常见的连续型和离散型分布
- **粒子动画**：欢迎页的粒子模拟展示正态分布的形成过程
- **关系网络图**：可视化展示各种分布之间的数学关系
- **交互式详情页**：参数可调，实时预览分布曲线变化
- **3B1B风格**：深色主题配合青色高亮

## 📊 支持的分布

| 类别 | 分布名称 | 用途 |
|------|----------|------|
| 连续型 | 正态分布 (Normal) | 自然现象、测量误差 |
| 连续型 | 均匀分布 (Uniform) | 随机数生成、蒙特卡洛模拟 |
| 连续型 | 指数分布 (Exponential) | 可靠性工程、排队论 |
| 连续型 | 学生t分布 (Student's t) | 小样本统计推断 |
| 连续型 | 卡方分布 (Chi-Square) | 拟合优度检验、方差分析 |
| 连续型 | F分布 (F-Distribution) | 方差齐性检验、ANOVA |
| 连续型 | Beta分布 | 贝叶斯统计、概率估计 |
| 连续型 | 伽马分布 (Gamma) | 排队论、保险精算 |
| 离散型 | 二项分布 (Binomial) | 独立试验、质量控制 |
| 离散型 | 泊松分布 (Poisson) | 计数数据、话务量预测 |
| 离散型 | 几何分布 (Geometric) | 首次成功试验次数 |
| 离散型 | 超几何分布 (Hypergeometric) | 无放回抽样 |

## 🏗️ 架构设计

```
VisualStatistics/
├── main.cpp              # 应用入口
├── mainwindow.cpp        # 主窗口，页面管理器
├── qcustomplot.cpp       # 第三方图表库
├── math/
│   ├── distributionmodel.h   # 分布抽象基类和工厂模式
│   └── distributionmodel.cpp # 12种分布的具体实现
└── pages/
    ├── welcomepage.*     # 欢迎页，粒子动画
    ├── networkpage.*     # 关系网络图页面
    └── detailpage.*      # 分布详情页
```

### 核心设计模式

1. **策略模式 (Strategy Pattern)**：`BaseDistribution` 作为抽象基类，定义统一接口，各分布实现具体算法
2. **工厂模式 (Factory Pattern)**：`DistFactory` 负责创建各类分布实例
3. **观察者模式 (Observer Pattern)**：页面间通过信号槽机制进行通信

## 🎨 视觉风格

应用采用 **3Blue1Brown** 风格的深色主题：

- **背景色**: `#121212` (深灰色)
- **主色调**: `#00FFF2` (青色/蓝绿色)
- **文字色**: `#E0E0E0` (浅灰色)
- **发光效果**: 青色阴影和渐变

### 粒子动画原理

欢迎页的粒子模拟展示了中心极限定理的直观效果：

1. 粒子从顶部生成，X坐标服从正态分布
2. 粒子下落并堆积形成柱状图
3. 柱状图自动拟合为平滑的正态曲线
4. 粒子透明度随高度变化，增强立体感

## 🚀 快速开始

### 环境要求

- Qt 5.15+ 或 Qt 6.x
- C++17 编译器
- Qt WebEngine (用于渲染数学公式)

### 编译运行

```bash
# 克隆项目
git clone <repository-url>
cd VisualStatistics

# 使用 qmake 构建
qmake VisualStatistics.pro
make

# 运行应用
./VisualStatistics
```

## 📱 页面结构

### 1. 欢迎页 (Welcome Page)
- 粒子动画展示正态分布
- "EXPLORE" 按钮进入主界面

### 2. 关系网页面 (Network Page)
- 节点表示各种概率分布
- 连线表示分布间的数学关系（极限、特殊化、组合）
- 点击节点跳转到详情页

### 3. 详情页 (Detail Page)
- PDF/CDF 曲线实时绘制
- 参数滑块调节
- 分布数学表达式（LaTeX渲染）
- 分布描述和典型应用场景
- 相关分布快速跳转

## 🧮 数学实现

### 核心接口

```cpp
class BaseDistribution {
public:
    virtual double calculate(double x) const = 0;       // PDF计算
    virtual double calculateCDF(double x) const = 0;    // CDF计算
    virtual QPair<double, double> getSuggestedRange() const = 0;
    virtual void setParameters(double p1, double p2) = 0;
};
```

### 数值计算技巧

- **对数计算组合数**：防止二项分布和超几何分布计算时溢出
- **数值积分**：使用梯形法计算非标准CDF
- **Gamma函数**：使用 `std::tgamma` 实现精确计算

## 📄 许可证

本项目采用 MIT 许可证，详见 [LICENSE](LICENSE) 文件。

## 🤝 贡献

欢迎提交 Issue 和 Pull Request！

---

*用数据可视化的力量，让统计学变得更加直观* 🔬
