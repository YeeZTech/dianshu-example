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
define_nt(pdf_page, std::vector<unsigned char>); // PDF页面数据
define_nt(total_pages, double); // PDF总页数
define_nt(page_number, double); // 截取PDF页面对应的页码
typedef ::ff::util::ntobject<pdf_page, total_pages, page_number> pdf_page_t;

class pdf_evaluate_parser {
public:
  pdf_evaluate_parser(
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
    double total_pages = 0;
    const int out_pages = 3; // Get 3 random pages
    std::vector<int> stored_pages;

    for (int i = 0; i < out_pages;) {
      uint8_t *data;
      uint32_t len;
      auto ret = stbox::ocall_cast<uint32_t>(ocall_get_page)(filename.c_str(), filename.size(), &data, &len);
      
      if (ret) {
        LOG(ERROR) << "ocall_get_page_list ret: " << ret;
        continue;
      }
      
      LOG(INFO) << "ocall_get_page_list succ, len: " << len;


      auto pkg = ypc::make_package<typename ypc::cast_obj_to_package<pdf_page_t>::type>::from_bytes(stbox::bytes(data, len));

      std::vector<unsigned char> page_data = pkg.get<::pdf_page>();

      std::string p_data(page_data.begin(), page_data.end());
      std::string base64_encoded_page = base64_encode(p_data);
      LOG(INFO) << "Base64 size: " << base64_encoded_page.size();

      double page_number = pkg.get<::page_number>();
      total_pages = pkg.get<::total_pages>();

      json::JSON ele;
      ele["type"] = "text";
      ele["label"] = "总页数";
      ele["value"] = std::to_string(int(total_pages))+ "页";
      res["result"].append(ele);

      // 如果pdf总页数只有一页，那么只需要存储一次页面数据，跳出循环即可
      if(total_pages == 1) {
        json::JSON ele;
        ele["type"] = "image";
        ele["label"] = "抽取位置";
        ele["value"] = base64_encoded_page;
        ele["desc"] = std::to_string(int(page_number + 1)) + "页";
        res["result"].append(ele);

        break;
      }
      else {
        bool is_stored = false;
        for(int j = 0; j < stored_pages.size(); j++) {
          if(stored_pages[j] == int(page_number)) {
            is_stored = true;
            break;
          }
        }

        if(is_stored) {
          continue;
        }

        stored_pages.push_back(int(page_number));
        
        json::JSON ele;
        ele["type"] = "image";
        ele["label"] = "抽取位置";
        ele["value"] = base64_encoded_page;
        ele["desc"] = std::to_string(int(page_number + 1)) + "页";
        res["result"].append(ele);
        i++;
      }

      // 如果pdf总页数为2，在获取了两页不同内容后跳出循环
      if(total_pages == stored_pages.size()) {
        break;
      }
    }


    ofs.open(filename.c_str(), ypc::ios_base::out | ypc::ios_base::trunc);
    ofs.close();

    return stbox::bytes(res.dump());
  }




protected:
  std::vector<std::shared_ptr<ypc::data_source_with_dhash>> m_datasources;
};
