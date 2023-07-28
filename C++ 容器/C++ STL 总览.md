# C++ STL

## 1. 顺序容器

### 1.1 std::vector\<T>

### 1.2 std::deque\<T>

> double-ended queue, 双向队列

+ 支持快速增删
  + push_front / pop_front
  + push_back / pop_back

### 1.3 std::array\<T, N>

+ 基本与 T\[N] 完全相同
+ 不会退化成 T*
+ 支持常见容器方法
  + .at()
  + .front()
  + .back()
  + .size()

### 1.4 std::list\<T>

+ 支持快速增删，但不能快速访问

### 1.5 std::forward_list\<T>

## 2. 常见接口

### 2.1 c.at(i)

+ 比起 c\[i]，作下标越界检查

## 2. STL 算法

+ std::fill
+ std::sort
+ std::find
+ std::reverse
+ std::unique
