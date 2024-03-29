// string_process.h
#ifndef STRING_PROCESS_H
#define STRING_PROCESS_H

#include <string>
#include <algorithm>

inline std::string process_string(const std::string &input) {
    std::string result;
    result.reserve(input.size());

    for (char c : input) {
        if (c == '\"') {
            result += '\\'; 
        }

        // 如果希望在输出的json文件中去除换行等文件操作符，可以取消下面的注释
        // 忽略掉的操作符分别为: 平制表, 换行, 垂直制表, 换页, 回车 
        // if (c>=9 && c<=13){
        //     continue;
        // }

        result += c;
    }

    return result;
}

// 输入一个字符串，返回去除字符串中文件操作符后的字符串长度
inline int get_string_length(const std::string &input) {
    int result = 0;
    for (char c : input) {
        if (c>=9 && c<=13){
            continue;
        }
        result++;
    }
    return result;
}

#endif 
