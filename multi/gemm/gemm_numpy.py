import numpy as np
import time

# 矩阵大小
matrix_size = 1600

# 初始化矩阵 A 和 B，元素范围在 [0, 1] 之间
A = np.random.rand(matrix_size, matrix_size)
B = np.random.rand(matrix_size, matrix_size)

print(A[0:2, 0:2])
print(B[0:2, 0:2])


# 执行矩阵乘法并记录执行时间
start_time = time.time()
C = A @ B
end_time = time.time()

# 计算矩阵乘法的执行时间
execution_time = end_time - start_time

# 打印执行时间和结果
print(f"Matrix multiplication executed in {execution_time:.4f} seconds.")
print(C[0:2, 0:2])
