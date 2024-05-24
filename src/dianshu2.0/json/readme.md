### JSON质量评估

#### 简介

该程序用于对JSON格式的数据进行质量验证。程序将统计该文件键值对的总数，缺失值数量及其占比，并随机抽取10%的内容（最多100个键值对）以生成质量验证报告。

#### 输出形式

```json
{
  "result" : [{
      "label" : "键值对总数",
      "type" : "text",
      "value" : "530"
    }, {
      "label" : "总缺失值",
      "type" : "text",
      "value" : "38"
    }, {
      "label" : "缺失值占比",
      "type" : "text",
      "value" : "7.16%"
    }, {
      "label" : "抽取10%键值对内容",
      "type" : "json",
      "value" : {
        "intents" : [{
            "context_set" : "",
            "patterns" : ["Hi", "How are you?", "Is anyone there?", "Hello", "Good day", "What's up", "how are ya", "heyy", "whatsup", "??? ??? ??"],
            "responses" : ["Hello!", "Good to see you again!", "Hi there, how can I help?"],
            "tag" : "greeting"
          }, {
            "context_set" : "",
            "patterns" : ["cya", "see you", "bye bye", "See you later", "Goodbye", "I am Leaving", "Bye", "Have a Good day", "talk to you later", "ttyl", "i got to go", "gtg"],
            "responses" : ["Sad to see you go :(", "Talk to you later", "Goodbye!", "Come back soon"],
            "tag" : "goodbye"
          }, {
            "context_set" : "",
            "patterns" : ["what is the name of your developers", "what is the name of your creators", "what is the name of the developers", "what is the name of the creators", "who created you", "your developers", "your creators", "who are your developers", "developers", "you are made by", "you are made by whom", "who created you", "who create you", "creators", "who made you", "who designed you"],
            "responses" : ["College students"],
            "tag" : "creator"
          }, {
            "context_set" : ""
          }]
      }
    }]
}
```

- **type**：渲染结果的类型，本验证程序中有text（文本）以及json类型，json类型的内容将被渲染为json代码块。

- **lable**：渲染结果的标签
- **value**：对应具体内容，改程序中包括键值对总数、总缺失数、缺失值占比、抽取10%键值对内容。
