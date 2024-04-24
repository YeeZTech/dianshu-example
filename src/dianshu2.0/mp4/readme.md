### 视频抽样程序



#### 简介

传入mp4格式的文件，本算法将读取mp4的内容并截取其中的数帧的画面编码成base64格式的json数据输出，输出图片不固定，在每次运行时都将随机选取图片输出，目前规定输出的内容视频总时间、三张视频截图（以Base64编码表示）以及其三张截图的截取时间，



#### 输出形式

```json
{
  "positions": [
    {
      "duration": "22"
      "picture":  "/9j/4AAQSkZJRgABAQAAAQABA......"
    },
    {
      "duration": "34" 
      "picture":  "/9j/4AAQSkZJRgABAQAAAQABA......"
    },
    {
      "duration": "45"
      "picture":  "/9j/4AAQSkZJRgABAQAAAQABA......"
    }
  ],
  "totalDuration": "80", 
}
```



- **totalDuration**：该字段中包含了所有视频截图的编码数据，每一条数据都代表一张视频截图的base64编码，可通过解码获取原图。
- **positions**：渲染结果的标签
  - **duration**：截取图片对应时间（以秒为单位表示）。
  - **picture**：该字段中包含了所有视频截图的编码数据，每一条数据都代表一张视频截图的base64编码，可通过解码获取原图。

