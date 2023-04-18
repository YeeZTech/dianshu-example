#include "../enclave/common_type.h"
#include "ypc/common/crypto_prefix.h"
#include "ypc/core/byte.h"
#include "ypc/core/filesystem.h"
#include "ypc/core/poption_require.h"
#include "ypc/corecommon/nt_cols.h"
#include "ypc/corecommon/package.h"
#include "ypc/terminus/crypto_pack.h"
#include <boost/property_tree/json_parser.hpp>

boost::program_options::variables_map parse_command_line(int argc,
                                                         char *argv[]) {
  namespace bp = boost::program_options;
  bp::options_description all("Result Decryption options");

  // clang-format off
  all.add_options()
  ("help", "help message")
  ("use-privatekey-file", bp::value<std::string>(), "private key file")
  ("encrypted-result-file", bp::value<std::string>(), "encrypted result file")
  ("package-type", bp::value<std::string>(), "company or industry")
  ("output", bp::value<std::string>(), "output file");
  // clang-format on

  boost::program_options::variables_map vm;
  boost::program_options::store(
      boost::program_options::parse_command_line(argc, argv, all), vm);

  if (vm.count("help")) {
    std::cout << all << std::endl;
    exit(-1);
  }
  if (!vm.count("use-privatekey-file") || !vm.count("encrypted-result-file") ||
      !vm.count("package-type")) {
    std::cerr << "parameters should include `use-privatekey-file`, "
                 "`encrypted-result-file` and `package-type`"
              << std::endl;
    exit(-1);
  }
  return vm;
}

ypc::bytes get_param_val(const boost::program_options::variables_map &vm,
                         const std::string &param, const std::string &key) {
  ypc::po(vm).require(ypc::opt(param));
  boost::property_tree::ptree pt;
  boost::property_tree::json_parser::read_json(vm[param].as<std::string>(), pt);
  return pt.get<ypc::bytes>(key);
}

int main(int argc, char *argv[]) {
  boost::program_options::variables_map vm;
  try {
    vm = parse_command_line(argc, argv);
  } catch (...) {
    std::cerr << "invalid cmd line parameters!" << std::endl;
    return -1;
  }

  auto private_key = get_param_val(vm, "use-privatekey-file", "private-key");
  auto encrypted_result =
      get_param_val(vm, "encrypted-result-file", "encrypted-result");

  auto ptr = ypc::terminus::intel_sgx_and_eth_compatible();
  auto result = ptr->ecc_decrypt(encrypted_result, private_key,
                                 ypc::utc::crypto_prefix_arbitrary);

  std::stringstream ss;
  ss << result;
  if (vm.count("output") != 0u) {
    std::string output_path =
        ypc::complete_path(vm["output"].as<std::string>());
    boost::property_tree::ptree pt;
    pt.put("result", ss.str());
    boost::property_tree::json_parser::write_json(output_path, pt);
  } else {
    // std::cout << ss.str() << std::endl;
    auto package_type = vm["package-type"].as<std::string>();
    if (package_type == "company") {
      auto pkg = ypc::make_package<summary_pkg_t>::from_bytes(result);
      for (auto &it : pkg.get<summary_table_t>()) {
        std::cout << it.get<id>() << ',' << it.get<company_code>() << ','
                  << it.get<company_name>() << ',' << it.get<industry_name>()
                  << ',' << it.get<company_type>() << ','
                  << it.get<located_in>() << ',' << it.get<register_address>()
                  << ',' << it.get<working_address>() << ','
                  << it.get<tax_income>() << ',' << it.get<district_income>()
                  << ',' << it.get<year_month>() << std::endl;
      }
    }
    if (package_type == "industry") {
      auto pkg = ypc::make_package<industry_summary_pkg_t>::from_bytes(result);
      for (auto &it : pkg.get<industry_summary_table_t>()) {
        std::cout << it.get<industry_name>() << ',' << it.get<company_count>()
                  << ',' << it.get<company_tax_count>() << ','
                  << it.get<tax_income>() << ',' << it.get<district_income>()
                  << std::endl;
      }
    }
  }
  return 0;
}
