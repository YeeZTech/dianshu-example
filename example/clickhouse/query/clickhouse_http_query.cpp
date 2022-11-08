#include "clickhouse_http_query.h"
#include <boost/format.hpp>
#include <iostream>
#include <sstream>
#include <string.h>
#include <string>
#include <vector>

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

uint32_t ocall_clickhouse_http_query(const char *request, uint32_t req_size,
                                     char *response, uint32_t resp_size) {
  std::string uri = "http://default:111@10.10.10.10:8123/";
  std::string sql =
      boost::str(boost::format("select * from test.t_person_trajectory where "
                               "msisdn='%1%' limit 256") %
                 std::string(request, req_size));
  std::string cmd =
      boost::str(boost::format("curl -s '%1%' -d \"%2%\"") % uri % sql);
  std::cout << "cmd: " << cmd << std::endl;
  std::string resp = get_stdout_from_command(cmd);
  memcpy(response, &resp[0], resp.size());
  return 0;
}
