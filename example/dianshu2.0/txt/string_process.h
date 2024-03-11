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
        result += c;
    }

    return result;
}

#endif 
