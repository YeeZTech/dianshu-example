### csv质量评估-热力图

#### **简介**

​	输入csv格式的文件，本算法将对文件的数据进行质量评估的统计，并生成算法对应的缺失热力图。


#### 输出形式

```json
{
"result": [
    {
      "type": "text",
      "label": "行数",
      "value": "11"
    },
    {
      "type": "text",
      "label": "列数",
      "value": "4"
    },
    {
      "type": "text",
      "label": "总缺失值",
      "value": "9"
    },
    {
      "type": "text",
      "label": "总缺失值占比",
      "value": "20.45%"
    },
    {
      "type": "image",
      "label": "缺失值热力图",
      "value": "/9j/4AAQSkZJRg......"
    }
  ]
}
```

输出结果将在一个json的`result`字段中按顺序渲染在页面上：


- **type**：渲染结果的类型，现有text（文本）以及image（图片）类型。
- **lable**：渲染结果的标签
- **value**：已经处理好的PDF抽取页面的base64编码，可通过解码获取抽取的PDF页面截图（已添加水印）。
