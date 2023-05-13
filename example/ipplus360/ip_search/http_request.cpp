#include <boost/format.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
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

uint32_t erase_child_node_if_exist(std::string &json,
                                   const std::string &child_key) {
  std::stringstream ss(json);
  boost::property_tree::ptree pt;
  try {
    boost::property_tree::json_parser::read_json(ss, pt);
  } catch (const std::exception &e) {
    throw std::runtime_error(boost::str(
        boost::format("Read json string failed! Error: %1%") % e.what()));
  }
  auto child = pt.get_child_optional(child_key);
  if (child) {
    pt.erase(child_key);
  }
  ss.clear();
  try {
    boost::property_tree::json_parser::write_json(ss, pt);
  } catch (const std::exception &e) {
    throw std::runtime_error(boost::str(
        boost::format("Write json string failed! Error: %1%") % e.what()));
  }
  json = ss.str();
  return 0;
}

uint32_t ocall_http_request(const char *service, uint32_t service_size,
                            const char *param, uint32_t param_size, char *resp,
                            uint32_t resp_size) {
  std::string sservice(service, service_size);
  std::string api_key = std::getenv("IPPLUS360_API_KEY");
  std::string url;
  if ("IP_SEARCH_DISTRICT" == sservice) {
    url = std::getenv("IPPLUS360_GEO_DISTRICT_URL") + api_key;
  } else if ("IP_SEARCH_STREET" == sservice) {
    url = std::getenv("IPPLUS360_GEO_STREET_URL") + api_key;
  }
  std::string ip = std::string(param, param_size);
  std::string full_url = boost::str(
      boost::format("%1%&ip=%2%&coordsys=WGS84&area=multi") % url % ip);
  std::string cmd = boost::str(boost::format("curl -s \"%1%\"") % full_url);

  std::string ret = get_stdout_from_command(cmd);
  std::vector<std::string> v = split_by_comma(ret, '\n');

  if (v.empty()) {
    return 1;
  }
  std::string &_resp = v.back();
  if (erase_child_node_if_exist(_resp, "charge")) {
    return 1;
  }
  memcpy(resp, &_resp[0], _resp.size());
  return 0;
}
