#include "cbd_parser.h"
#include "iodef.h"
#include "ypc/terminus/crypto_pack.h"
#include <boost/program_options.hpp>
#include <iostream>

boost::program_options::variables_map parse_command_line(int argc,
                                                         char *argv[]) {
  namespace bp = boost::program_options;
  bp::options_description all("CBD Analyzer options");

  // clang-format off
  all.add_options()
    ("help", "help message")
    ("input", bp::value<std::string>(), "input file")
    ("output", bp::value<std::string>(), "output file");
  // clang-format on

  boost::program_options::variables_map vm;
  boost::program_options::store(
      boost::program_options::parse_command_line(argc, argv, all), vm);

  if (vm.count("help")) {
    std::cout << all << std::endl;
    exit(-1);
  }
  return vm;
}

std::shared_ptr<cbd_parser> g_parser;

int main(int argc, char *argv[]) {
  boost::program_options::variables_map vm;
  try {
    vm = parse_command_line(argc, argv);
  } catch (...) {
    std::cerr << "invalid cmd line parameters!" << std::endl;
    return -1;
  }

  input_param_t input_param =
      ypc::ntjson::from_json_file<input_param_t>(vm["input"].as<std::string>());
  g_parser = std::make_shared<cbd_parser>(input_param);
  g_parser->parse();

  std::string output_fp = vm["output"].as<std::string>();
  try {
    std::ofstream os(output_fp, std::ios::out | std::ios::binary);
    const std::string &res = g_parser->get_result_str();
    std::cout << "result size: " << res.size() << std::endl;
    os.write(res.data(), res.size());
  } catch (const std::exception &e) {
    std::cerr << "cannot open " << output_fp << std::endl;
    return 1;
  }

  return 0;
}

extern "C" {
uint32_t km_session_request_ocall(sgx_dh_msg1_t *dh_msg1, uint32_t *session_id);
uint32_t km_exchange_report_ocall(sgx_dh_msg2_t *dh_msg2,
                                  sgx_dh_msg3_t *dh_msg3, uint32_t session_id);
uint32_t km_send_request_ocall(uint32_t session_id,
                               secure_message_t *req_message,
                               size_t req_message_size, size_t max_payload_size,
                               secure_message_t *resp_message,
                               size_t resp_message_size);
uint32_t km_end_session_ocall(uint32_t session_id);

uint32_t next_data_batch(const uint8_t *data_hash, uint32_t hash_size,
                         uint8_t **data, uint32_t *len);
void free_data_batch(uint8_t *data);

uint32_t ocall_exec_sql(const uint8_t *sql, uint32_t sql_size);
}

uint32_t km_session_request_ocall(sgx_dh_msg1_t *dh_msg1,
                                  uint32_t *session_id) {
  return g_parser->keymgr()->session_request(dh_msg1, session_id);
}
uint32_t km_exchange_report_ocall(sgx_dh_msg2_t *dh_msg2,
                                  sgx_dh_msg3_t *dh_msg3, uint32_t session_id) {
  return g_parser->keymgr()->exchange_report(dh_msg2, dh_msg3, session_id);
}
uint32_t km_send_request_ocall(uint32_t session_id,
                               secure_message_t *req_message,
                               size_t req_message_size, size_t max_payload_size,
                               secure_message_t *resp_message,
                               size_t resp_message_size) {
  return g_parser->keymgr()->generate_response(req_message, req_message_size,
                                               max_payload_size, resp_message,
                                               resp_message_size, session_id);
}
uint32_t km_end_session_ocall(uint32_t session_id) {
  return g_parser->keymgr()->end_session(session_id);
}

uint32_t next_data_batch(const uint8_t *data_hash, uint32_t hash_size,
                         uint8_t **data, uint32_t *len) {
  return g_parser->next_data_batch(data_hash, hash_size, data, len);
}
void free_data_batch(uint8_t *data) { g_parser->free_data_batch(data); }

uint32_t ocall_exec_sql(const uint8_t *sql, uint32_t sql_size) {
  std::fstream fs;
  fs.open("t_summary_org_info.sql", std::ios::out | std::ios::binary);
  if (!fs.is_open()) {
    throw std::runtime_error("open file failed!");
  }
  fs.write((const char *)sql, sql_size);
  fs.close();
  return 0;
}
