#pragma once
#include "user_type.h"
#include "ypc/common/limits.h"
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

namespace txt {

class txt_batch_reader {
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

class typed_txt_batch_reader : public txt_batch_reader {
public:
  // TODO we should add more csv options here
  /// @extra_param should be a json string, like this
  // {file_path: "xxx"}
  typed_txt_batch_reader(const std::string &extra_param)
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
    m_max_batch_size = ypc::utc::max_item_size >> 1;
  }
  virtual int reset_for_read() {
    m_stream.reset(new std::ifstream(m_file_path));
    if (!m_stream->is_open()) {
      return -1;
    }
    return 0;
  }

  virtual int read_item_data(char *buf, int *len) {
    // read batch
    std::ifstream *is = m_stream.get();
    std::ifstream::pos_type cur = is->tellg();
    is->seekg(0, is->end);
    std::ifstream::pos_type left_size = is->tellg() - cur;
    is->clear();
    is->seekg(cur, is->beg);
    size_t batch_size = m_max_batch_size;
    if (left_size < batch_size) {
      batch_size = left_size;
    }
    std::string s(batch_size, '0');
    is->read(&s[0], batch_size);
    // serialize
    typedef ff::net::ntpackage<0, ::txt_batch> package_t;
    package_t v;
    v.set<::txt_batch>(s);
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
    is->seekg(0, is->end);
    std::ifstream::pos_type size = is->tellg();
    size_t batch_size = m_max_batch_size;
    size_t n = size / batch_size;
    if (size % batch_size) {
      n++;
    }
    // must clear ifstream before seekg
    is->clear();
    is->seekg(pos, is->beg);
    return n;
  }

protected:
  size_t m_max_batch_size;
  const std::string m_extra_param;
  std::string m_file_path;
  std::unique_ptr<std::ifstream> m_stream;
};
} // namespace txt

#define impl_txt_batch_reader(type)                                            \
  void *create_item_reader(const char *extra_param, int len) {                 \
    try {                                                                      \
      txt::txt_batch_reader *reader = new type(std::string(extra_param, len)); \
      return reader;                                                           \
    } catch (const std::exception &e) {                                        \
      std::cout << "create_item_reader got exception: " << e.what()            \
                << std::endl;                                                  \
      return nullptr;                                                          \
    }                                                                          \
  }                                                                            \
  int reset_for_read(void *handle) {                                           \
    txt::txt_batch_reader *reader = (txt::txt_batch_reader *)handle;           \
    return reader->reset_for_read();                                           \
  }                                                                            \
  int read_item_data(void *handle, char *buf, int *len) {                      \
    txt::txt_batch_reader *reader = (txt::txt_batch_reader *)handle;           \
    return reader->read_item_data(buf, len);                                   \
  }                                                                            \
  int close_item_reader(void *handle) {                                        \
    txt::txt_batch_reader *reader = (txt::txt_batch_reader *)handle;           \
    reader->close_item_reader();                                               \
    delete reader;                                                             \
    return 0;                                                                  \
  }                                                                            \
                                                                               \
  uint64_t get_item_number(void *handle) {                                     \
    txt::txt_batch_reader *reader = (txt::txt_batch_reader *)handle;           \
    return reader->get_item_number();                                          \
  }
