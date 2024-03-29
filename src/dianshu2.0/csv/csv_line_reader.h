#pragma once
#include "user_type.h"
#include "ypc/corecommon/package.h"
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <ff/util/ntobject.h>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void *create_item_reader(const char *extra_param, int len);

int reset_for_read(void *handle);
int read_item_data(void *handle, char *buf, int *len);
int close_item_reader(void *handle);
uint64_t get_item_number(void *handle);

#ifdef __cplusplus
}
#endif

namespace csv {

class csv_line_reader {
public:
  virtual int reset_for_read() = 0;
  virtual int read_item_data(char *buf, int *len) = 0;
  virtual int close_item_reader() = 0;
  virtual int get_item_number() = 0;
};

template <typename NT> struct ntobject_size { const static size_t size = 0; };
template <typename T1> struct ntobject_size<::ff::util::ntobject<T1>> {
  const static size_t size = 1;
};
template <typename T1, typename... ARGS>
struct ntobject_size<::ff::util::ntobject<T1, ARGS...>> {
  const static size_t size =
      1 + ntobject_size<::ff::util::ntobject<ARGS...>>::size;
};

class typed_csv_line_reader : public csv_line_reader {
public:
  // TODO we should add more csv options here
  /// @extra_param should be a json string, like this
  // {file_path: "xxx"}
  typed_csv_line_reader(const std::string &extra_param)
      : m_extra_param(extra_param), m_file_path(extra_param) {
    /*
    boost::property_tree::ptree pt;
    std::stringstream ss;
    ss << extra_param;
    boost::property_tree::json_parser::read_json(ss, pt);
    m_file_path = pt.get_child("file_path").get_value<std::string>();
*/
    m_stream.reset(new std::ifstream(m_file_path));
    if (!m_stream->is_open()) {
      throw std::runtime_error("file not exist");
    }
  }
  virtual int reset_for_read() {
    m_stream.reset(new std::ifstream(m_file_path));
    if (!m_stream->is_open()) {
      return -1;
    }
    return 0;
  }

  virtual int read_item_data(char *buf, int *len) {
    std::string s;
    std::getline(*m_stream.get(), s);
    // 检查行是否为空，如果是空行则跳过，直接读取下一行
    while (s.empty()) {
        if (m_stream->eof()) {
            // 文件已经结束
            return -1;
        }
        std::getline(*m_stream.get(), s);
    }
    typedef ff::net::ntpackage<0, ::csv_line> package_t;
    package_t v;
    v.set<::csv_line>(s);
    if (len) {
      ff::net::marshaler lm(ff::net::marshaler::length_retriver);
      v.arch(lm);
      *len = static_cast<int>(lm.get_length());
    }
    if (buf) {
      ff::net::marshaler sm(buf, *len, ff::net::marshaler::serializer);
      v.arch(sm);
    }
    return 0;
  }

  virtual int close_item_reader() {
    m_stream.reset();
    return 0;
  }

  virtual int get_item_number() {
    std::ifstream *is = m_stream.get();
    std::ifstream::pos_type pos = is->tellg();
    is->seekg(0, is->beg);
    uint64_t n = 0;
    std::string s = "s";
    while (!s.empty() && !is->eof()) {
      s.clear();
      std::getline(*is, s);
      if (!s.empty()) {
        n++;
      }
    }
    // must clear ifstream before seekg
    is->clear();
    is->seekg(pos, is->beg);
    return n;
  }

protected:
  const std::string m_extra_param;
  std::string m_file_path;
  std::unique_ptr<std::ifstream> m_stream;
};
} // namespace csv

#define impl_csv_line_reader(type)                                             \
  void *create_item_reader(const char *extra_param, int len) {                 \
    try {                                                                      \
      csv::csv_line_reader *reader = new type(std::string(extra_param, len));  \
      return reader;                                                           \
    } catch (const std::exception &e) {                                        \
      std::cout << "create_item_reader got exception: " << e.what()            \
                << std::endl;                                                  \
      return nullptr;                                                          \
    }                                                                          \
  }                                                                            \
  int reset_for_read(void *handle) {                                           \
    csv::csv_line_reader *reader = (csv::csv_line_reader *)handle;             \
    return reader->reset_for_read();                                           \
  }                                                                            \
  int read_item_data(void *handle, char *buf, int *len) {                      \
    csv::csv_line_reader *reader = (csv::csv_line_reader *)handle;             \
    return reader->read_item_data(buf, len);                                   \
  }                                                                            \
  int close_item_reader(void *handle) {                                        \
    csv::csv_line_reader *reader = (csv::csv_line_reader *)handle;             \
    reader->close_item_reader();                                               \
    delete reader;                                                             \
    return 0;                                                                  \
  }                                                                            \
                                                                               \
  uint64_t get_item_number(void *handle) {                                     \
    csv::csv_line_reader *reader = (csv::csv_line_reader *)handle;             \
    return reader->get_item_number();                                          \
  }
