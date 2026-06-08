import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import griddata

# 文件路径
# file_path = r"D:\Diff_Topo\开源\topodiff-Evaluate\3批量导入comsol再仿真\再仿真温度场\模型结构导入comsol仿真温度场_016.txt"
# file_path = r"D:\Diff_Topo\开源\topodiff-Evaluate\3批量导入comsol再仿真\再仿真温度场\模型结构导入comsol仿真温度场_018.txt"

file_path = r"D:\AI_Comsol\temperature_field.txt"





# 读取数据
data = np.loadtxt(file_path, comments='%', encoding='utf-8')

# 拆分三列
x = data[:, 0]
y = data[:, 1]
T = data[:, 2]

# 创建规则网格
xi = np.linspace(x.min(), x.max(), 500)  # 调整500可改变分辨率
yi = np.linspace(y.min(), y.max(), 500)
xi, yi = np.meshgrid(xi, yi)

# 网格化插值（最近邻插值边界最清晰）
zi = griddata((x, y), T, (xi, yi), method='nearest')
# 可选插值方法：
# method='linear'   - 线性插值（稍平滑）
# method='cubic'    - 三次样条（更平滑）
# method='nearest'  - 最近邻（边界最清晰）

# 画图
plt.figure(figsize=(8, 6))

# 使用pcolormesh绘制，设置shading='auto'或'flat'避免插值
mesh = plt.pcolormesh(xi, yi, zi, cmap='jet', shading='auto')

# 添加颜色条
cbar = plt.colorbar(mesh)
cbar.set_label('Temperature (K)')

# 坐标轴与标题
plt.xlabel('x')
plt.ylabel('y')
plt.title('Temperature Field (Gridded with Nearest Neighbor)')

# 保持比例一致
plt.axis('equal')

plt.tight_layout()
plt.show()