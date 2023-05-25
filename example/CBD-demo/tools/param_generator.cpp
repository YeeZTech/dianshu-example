#include "../enclave/common_type.h"
#include "ypc/common/crypto_prefix.h"
#include "ypc/core/byte.h"
#include "ypc/core/filesystem.h"
#include "ypc/core/poption_require.h"
#include "ypc/core/privacy_data_reader.h"
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
  ("company-label-url", bp::value<std::string>(), "company label url")
  ("company-label-plugin", bp::value<std::string>(), "shared library for reading data")
  ("industry-url", bp::value<std::string>(), "industry url")
  ("industry-plugin", bp::value<std::string>(), "shared library for reading data")
  ("county-url", bp::value<std::string>(), "county url")
  ("county-plugin", bp::value<std::string>(), "shared library for reading data")
  ("output", bp::value<std::string>(), "output file");
  // clang-format on

  boost::program_options::variables_map vm;
  boost::program_options::store(
      boost::program_options::parse_command_line(argc, argv, all), vm);

  if (vm.count("help")) {
    std::cout << all << std::endl;
    exit(-1);
  }
  if (vm.count("company-label-url") == 0u ||
      vm.count("company-label-plugin") == 0u) {
    std::cerr << "company label url or plugin not specified!" << std::endl;
    exit(-1);
  }
  if (vm.count("industry-url") == 0u || vm.count("industry-plugin") == 0u) {
    std::cerr << "industry url or plugin not specified!" << std::endl;
    exit(-1);
  }
  if (vm.count("county-url") == 0u || vm.count("county-plugin") == 0u) {
    std::cerr << "county url or plugin not specified!" << std::endl;
    exit(-1);
  }
  if (vm.count("output") == 0u) {
    std::cerr << "output not specified" << std::endl;
    exit(-1);
  }
  return vm;
}

define_nt(company_label_table, std::vector<ypc::bytes>);
define_nt(industry_table, std::vector<ypc::bytes>);
define_nt(county_table, std::vector<ypc::bytes>);
typedef ff::net::ntpackage<0, company_label_table, industry_table, county_table>
    param_pkg_t;

std::vector<ypc::bytes> read_data_table(const std::string &data_url,
                                        const std::string &plugin_path) {
  std::vector<ypc::bytes> data_table;
  ypc::privacy_data_reader reader(plugin_path, data_url);
  uint64_t item_number = reader.get_item_number();
  uint64_t counter = 1;
  ypc::bytes item_data = reader.read_item_data();
  data_table.push_back(item_data);
  while (!item_data.empty() && counter < item_number) {
    item_data = reader.read_item_data();
    data_table.push_back(item_data);
    counter++;
  }
  return data_table;
}

int main(int argc, char *argv[]) {
  boost::program_options::variables_map vm;
  try {
    vm = parse_command_line(argc, argv);
  } catch (...) {
    std::cerr << "invalid cmd line parameters!" << std::endl;
    return -1;
  }

  param_pkg_t pkg;
  // company label
  std::string company_label_url = vm["company-label-url"].as<std::string>();
  std::string company_label_plugin =
      vm["company-label-plugin"].as<std::string>();
  pkg.set<::company_label_table>(
      read_data_table(company_label_url, company_label_plugin));
  // industry
  std::string industry_url = vm["industry-url"].as<std::string>();
  std::string industry_plugin = vm["industry-plugin"].as<std::string>();
  pkg.set<::industry_table>(read_data_table(industry_url, industry_plugin));
  // county
  std::string county_url = vm["county-url"].as<std::string>();
  std::string county_plugin = vm["county-plugin"].as<std::string>();
  pkg.set<::county_table>(read_data_table(county_url, county_plugin));
  if (vm.count("output") != 0u) {
    std::stringstream ss;
    ss << ypc::make_bytes<ypc::bytes>::for_package(pkg);
    std::string output_path =
        ypc::complete_path(vm["output"].as<std::string>());
    boost::property_tree::ptree pt;
    pt.put("params", ss.str());
    boost::property_tree::json_parser::write_json(output_path, pt);
  }
  return 0;
}
