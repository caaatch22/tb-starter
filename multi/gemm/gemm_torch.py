import torch
import time

# 使用GPU进行计算，如果可用
device = torch.device("cuda")
print(device)
matrix_size = 1024 * 16
# 创建两个10000x10000的随机矩阵
A = torch.rand(matrix_size, matrix_size, device=device)
B = torch.rand(matrix_size, matrix_size, device=device)

# 记录开始时间
start_time = time.time()

# 矩阵相乘
C = torch.matmul(A, B)

# 计算并打印耗时
execution_time = time.time() - start_time
print(f"Matrix multiplication executed in {execution_time:.4f} seconds. GFlops: {2 * matrix_size ** 3 / (execution_time * 1e9):.2f}")

