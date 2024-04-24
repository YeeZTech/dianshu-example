

### TXT质量评估

#### 简介

输入txt格式的文件，本算法将对txt文件进行质量评估，将txt文件的前中后位置上的部分内容打包输出到一个json文件中。目前的算法中，当输入txt文件后，将返回该txt内容的前150字，尾部150字以及中间300字。当txt中内容太段（目前该限制为小于1500字）时，将改为取txt的前15%，后15%以及中间30%的内容。

注：在分析中，一个“字”表示一个 英文字母/汉字/符号/操作符 等内容。

#### 输出形式

```json
{
  "head": "#include \"user_type.h\"\n#include \"string_process.h\"\n#include \"ypc/core_t/analyzer/data_source.h\"\n#include \"ypc/stbox/ebyte.h\"\n#include \"ypc/stbox/stx_c",
  "mid": "一个batch的所需长度\n    int start_pos = total_size / 2 - 50; // 中间的100字节的起始位置\n    int mid_batch_no = start_pos / batch_max_size; // 中间的100字节所在的batch的编号\n    \n    start_pos = start_pos % batch_max_size; // 中间的100字节的起始位置在batch中的位置\n\n    LOG(INFO) << \"Batch Num: \" << batch_num;\n    LOG(INFO) << \"Start Position:",
  "tail": " process_string(middle_100) + \"\\\"}\";\n    return result;\n  }\n\nprotected:\n  std::vector<std::shared_ptr<ypc::data_source_with_dhash>> m_datasources;\n};\n",
  "total": "3474"
}

```

- **`head`**：txt头部150字内容

- **`cols`**：txt中部300字内容

- **`totalNulls`**：txt尾部150字内容.

- **`total`**：总字数统计

可以看到，当输出的字符中存在引号时，输出的引号前会自动添加反斜线**`  '\'  `**，这是为了json可以被成功解析。

