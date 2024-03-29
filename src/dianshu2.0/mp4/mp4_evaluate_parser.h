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
define_nt(video_frame, std::vector<unsigned char>);
typedef ::ff::util::ntobject<video_frame> video_frame_t;
define_nt(video_frames, std::vector<video_frame_t>);
typedef ::ff::util::ntobject<video_frames> video_frame_list_t;

class mp4_evaluate_parser {
public:
  mp4_evaluate_parser(
      std::vector<std::shared_ptr<ypc::data_source_with_dhash>> &source)
      : m_datasources(source) {}

  inline stbox::bytes do_parse(const stbox::bytes &param) {
    LOG(INFO) << "params: " << param;
    ypc::to_type<stbox::bytes, data_slice_item_t> converter(
        m_datasources[0].get());

    stbox::bytes skey;
    ecc::gen_private_key(skey);
    std::string skey_hex(2 * skey.size(), '0');
    ypc::utc::internal::convert_bytes_to_hex(
        skey.data(), skey.size(), (uint8_t *)&skey_hex[0], skey_hex.size());
    std::string filename = skey_hex.substr(0, 8) + ".raw";
    ypc::cxxfile ofs;
    ofs.open(filename.c_str(), ypc::ios_base::out | ypc::ios_base::binary);
    bool flag = false;

    hpda::processor::internal::filter_impl<data_slice_item_t> match(
        &converter, [&](const data_slice_item_t &v) {
          auto slice = v.get<::data_slice>();
          ofs.write(slice.data(), slice.size());
          return false;
        });

    hpda::output::internal::memory_output_impl<data_slice_item_t> mo(&match);
    mo.get_engine()->run();
    ofs.close();
    LOG(INFO) << "do parse done";

    json::JSON res;
    const int frame_capture_len = 3;
    for (int i = 0; i < frame_capture_len; i++) {
      uint8_t *data;
      uint32_t len;
      auto ret = stbox::ocall_cast<uint32_t>(ocall_get_frame)(
          filename.c_str(), filename.size(), &data, &len);
      if (ret) {
        LOG(ERROR) << "ocall_get_frame_list ret: " << ret;
      }
      LOG(INFO) << "ocall_get_frame_list succ, len: " << len;
      auto pkg = ypc::make_package<typename ypc::cast_obj_to_package<
          video_frame_t>::type>::from_bytes(stbox::bytes(data, len));
      std::vector<unsigned char> img = pkg.get<::video_frame>();
      std::string s((const char *)img.data(), img.size());
      std::string base64_s = base64_encode(s);
      LOG(INFO) << "base64 size: " << base64_s.size();
      res["images"].append(base64_s);
    }
    // trunc the content of file
    ofs.open(filename.c_str(), ypc::ios_base::out | ypc::ios_base::trunc);
    ofs.close();

    return stbox::bytes(res.dump());
  }


protected:
  std::vector<std::shared_ptr<ypc::data_source_with_dhash>> m_datasources;
};
