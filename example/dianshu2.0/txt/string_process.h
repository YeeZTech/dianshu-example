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
        // if (c>=9 && c<=13){ // 9水平制表 10换行 11垂直制表 12换页 13回车 
        //     continue;
        // }
        result += c;
    }

    return result;
}

#endif 
