#include "ypc/sgx/usgx/http_request.h"
#include <boost/format.hpp>
#include <iostream>
#include <sstream>
#include <string.h>
#include <string>

std::string get_stdout_from_command(std::string &cmd) {
  std::string data;
  const int max_buffer = 256;
  char buffer[max_buffer];
  cmd.append(" 2>&1");

  FILE *stream = popen(cmd.c_str(), "r");
  if (stream) {
    while (!feof(stream)) {
      if (fgets(buffer, max_buffer, stream) != NULL) {
        data.append(buffer);
      }
    }
    pclose(stream);
  }
  return data;
}

std::vector<std::string> split_by_comma(const std::string &str, char comma) {
  std::vector<std::string> v;
  std::stringstream ss(str);
  std::string token;

  while (std::getline(ss, token, comma)) {
    v.push_back(token);
  }
  return v;
}

uint32_t ocall_http_request(const char *param, uint32_t param_size, char *resp,
                            uint32_t resp_size) {
  std::string api_key = std::getenv("IPPLUS360_API_KEY");
  std::string ip = std::string(param, param_size);
  std::string url =
      boost::str(boost::format("https://api.ipplus360.com/ip/geo/v1/street/psi/"
                               "?key=%1%&ip=%2%&coordsys=WGS84&area=multi") %
                 api_key % ip);
  std::string cmd = boost::str(boost::format("curl -s \"%1%\"") % url);

  std::string ret = get_stdout_from_command(cmd);
  std::vector<std::string> v = split_by_comma(ret, '\n');

  if (v.empty()) {
    return 1;
  }
  std::string &_resp = v.back();
  memcpy(resp, &_resp[0], _resp.size());
  return 0;
}
