### csv质量评估

#### **简介**

​	输入csv格式的文件，本算法将对文件的数据进行质量评估的统计，并在算法结束后将统计结果输出。统计结果包括csv的行数，列数，总缺失值数，列缺失值数量统计、行缺失值数量统计。算法将以列数最多的行为起点统计缺失值情况。



#### 输出形式

输出的结果将是一个json的形式，形如：

```json
{
  "rows": 58,
  "cols": 6,
  "totalNulls": 48,
  "NullProportion": 13.79,
  "col_nulls": [
    { "null_num": 6, "count": 1 },
    { "null_num": 7, "count": 1 },
    { "null_num": 8, "count": 2 },
    { "null_num": 9, "count": 1 },
    { "null_num": 10, "count": 1 }
  ],
  "row_nulls": [
    { "null_num": 0, "count": 34 },
    { "null_num": 1, "count": 16 },
    { "null_num": 2, "count": 2 },
    { "null_num": 3, "count": 1 },
    { "null_num": 4, "count": 1 },
    { "null_num": 5, "count": 3 },
    { "null_num": 6, "count": 1 }
  ]
}

```

- **`rows`**：csv中的总行数

- **`cols`**：csv中的总列数

- **`totalNulls`**：总缺失值的数量

- **`NullProportion`**：缺失值百分比，该缺失值精确到小数点后两位。

- **`col_nulls`**：每列中有多少个缺失行的数量统计

  - **`null_num`**：表示缺失x行
  - **`count`**：缺失x行的数据共有多少列

  **注**：若缺失x行的数量为0时**`(count=0)`，将不会输出**；`null_num > 1000` 时的统计将统一存储在`null_num=1001`的条目中。这两个条件对缺失列的数据统计同样适用。

- **`row_nulls`**：每行中有多少个缺失列的数量统计

  - **`null_num`**：表示缺失x列
  - **`count`**：缺失x列的数据共有多少行

