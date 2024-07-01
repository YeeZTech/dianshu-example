#include "../json.hpp"
#include "dianshu_parser_t.h"
#include "user_type.h"

#include "ypc/common/byte/bytes_common.h"
#include "ypc/common/crypto_prefix.h"
#include "ypc/core_t/analyzer/data_source.h"
#include "ypc/core_t/file/blockfile.h"
#include "ypc/corecommon/crypto/stdeth.h"
#include "ypc/corecommon/to_type.h"
#include "ypc/stbox/ebyte.h"
#include "ypc/stbox/stx_common.h"
#include "ypc/stbox/tsgx/log.h"
#include "ypc/stbox/tsgx/ocall.h"

#include <hpda/extractor/raw_data.h>
#include <hpda/output/memory_output.h>
#include <hpda/processor/query/filter.h>


using ecc = ypc::crypto::eth_sgx_crypto;
using ntt = ypc::nt<stbox::bytes>;

define_nt(input_param, std::string);
typedef ff::net::ntpackage<0, input_param> nt_package_t;

define_nt(flat_arr, std::vector<int8_t>);
define_nt(row_s, int);
define_nt(row_e, int);
typedef ::ff::util::ntobject<::row_s,::row_e,::flat_arr> flat_arr_t;

define_nt(row_num, int); // 总行数
define_nt(col_num, int); // 总列数
define_nt(total_nulls, long long); // 总缺失值数量
define_nt(null_proportion, std::string); // 缺失值比例
define_nt(heat_map, std::vector<unsigned char>); // 热力图
define_nt(missNullNumRowCount, std::vector<int>); // 行缺失值数量
define_nt(missNumColCount, std::vector<int>); // 列缺失值数量
define_nt(csv_file_path, std::string); // CSV文件路径
typedef ::ff::util::ntobject<row_num, col_num, total_nulls, null_proportion, heat_map, missNullNumRowCount, missNumColCount, csv_file_path> csv_info_t;

std::string format_with_comma(long long value) {
  std::string numStr = std::to_string(value);
  int insertPosition = numStr.length() - 3;
  int startPos = (numStr[0] == '-') ? 1 : 0;
  while (insertPosition > startPos) {
    numStr.insert(insertPosition, ",");
    insertPosition -= 3;
  }
  return numStr;
}

class tsv_evaluate_parser_heat_map {
public:
  tsv_evaluate_parser_heat_map(
      std::vector<std::shared_ptr<ypc::data_source_with_dhash>> &source)
      : m_datasources(source) {}

  inline stbox::bytes do_parse(const stbox::bytes &param) {

    LOG(INFO) << "param: " << param;

    // 设置数据转换器，用于将原始数据转换为指定的数据片段项
    ypc::to_type<stbox::bytes, data_slice_item_t> converter(
        m_datasources[0].get());

    // 生成私钥
    stbox::bytes skey;
    ecc::gen_private_key(skey);
    // 将私钥转换为十六进制字符串
    std::string skey_hex(2 * skey.size(), '0');
    ypc::utc::internal::convert_bytes_to_hex(
        skey.data(), skey.size(), (uint8_t *)&skey_hex[0], skey_hex.size());
    // 根据私钥生成文件名
    std::string filename = skey_hex.substr(0, 8) + ".raw";

    LOG(INFO) << "filename: " << filename;

    ypc::cxxfile ofs;
    ofs.open(filename.c_str(), ypc::ios_base::out | ypc::ios_base::binary);
    int count = 0;
    // 设置数据过滤器，用于处理数据片段
    hpda::processor::internal::filter_impl<data_slice_item_t> match(
        &converter, [&](const data_slice_item_t &v) {
          auto slice = v.get<::data_slice>();    // 获取数据片段
          ofs.write(slice.data(), slice.size()); // 将数据片段写入文件
          count++;
          return false;
        });

    hpda::output::internal::memory_output_impl<data_slice_item_t> mo(&match);
    mo.get_engine()->run();
    ofs.close();
    LOG(INFO) << "do parse done";
    LOG(INFO) << "count match time: " << count;

    typename ypc::cast_obj_to_package<csv_info_t>::type pkg;
    pkg.set<::csv_file_path>(filename);

    // -4 表示是TSV的热力图
    int tag = -4; // 表示需要生成热力图
    uint8_t* res_buf;
    size_t res_buf_len;

    auto file = ypc::make_bytes<stbox::bytes>::for_package(pkg);
    auto ret = stbox::ocall_cast<uint32_t>(ocall_array_to_image)(
        file.data(), file.size(), tag, &res_buf, &res_buf_len);


    // 反序列化
    auto res_pkg = ypc::make_package<typename ypc::cast_obj_to_package<csv_info_t>::type>::from_bytes(stbox::bytes(res_buf, res_buf_len));

    json::JSON pack_j;

    std::vector<unsigned char> heat_map_data = res_pkg.get<::heat_map>();
    std::string p_data(heat_map_data.begin(), heat_map_data.end());
    std::string base64_img = base64_encode(p_data);

    // 添加图像数据到JSON对象
    json::JSON ele1;
    ele1["type"] = "text";
    ele1["label"] = "行数";
    ele1["value"] = format_with_comma(res_pkg.get<::row_num>());
    pack_j["result"].append(ele1);

    json::JSON ele2;
    ele2["type"] = "text";
    ele2["label"] = "列数";
    ele2["value"] = format_with_comma(res_pkg.get<::col_num>());
    pack_j["result"].append(ele2);

    json::JSON ele3;
    ele3["type"] = "text";
    ele3["label"] = "总缺失值";
    ele3["value"] = format_with_comma(res_pkg.get<::total_nulls>());
    pack_j["result"].append(ele3);

    json::JSON ele4;
    ele4["type"] = "text";
    ele4["label"] = "总缺失值占比";
    ele4["value"] = res_pkg.get<::null_proportion>() + "%";
    pack_j["result"].append(ele4);

    json::JSON ele;
    ele["type"] = "image";
    ele["label"] = "缺失值热力图";
    ele["value"] = base64_img;
    pack_j["result"].append(ele);

    return stbox::bytes(pack_j.dump());
  }

protected:
  std::vector<std::shared_ptr<ypc::data_source_with_dhash>> m_datasources;
};
