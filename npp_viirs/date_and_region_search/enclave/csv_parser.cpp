#include "csv_parser.h"
#include "stbox/tsgx/log.h"
#include "user_type.h"
#include <string>

std::vector<std::string> split(const std::string &str,
                               const std::string &delim) {
  std::vector<std::string> res;
  if ("" == str) {
    return res;
  }
  std::string::size_type s = 0;
  std::string::size_type e = str.find(delim, s);
  while (e != str.npos) {
    auto ts = std::string(&str[s], e - s);
    res.push_back(ts);
    s = e + 1;
    e = str.find(delim, s);
  }
  res.push_back(std::string(&str[s], str.size() - s));
  return res;
}

int parse_item_data(const uint8_t *data, int len, void *item) {
  user_item_t *uitem = (user_item_t *)item;
  std::string str((const char *)data, len);
  std::vector<std::string> result = split(str, ",");
  if (result.size() != 4) {
    return -1;
  }
  uitem->set<::date_item>(result[0]);
  uitem->set<::region_item>(result[1]);
  uitem->set<::total_r>(result[2]);
  uitem->set<::sample_cnt>(result[3]);
  return 0;
}
