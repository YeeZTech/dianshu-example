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

#include <hpda/extractor/raw_data.h>
#include <hpda/output/memory_output.h>
#include <hpda/processor/query/filter.h>

using ecc = ypc::crypto::eth_sgx_crypto;
define_nt(param1, std::string);
define_nt(param2, std::string);
typedef ff::net::ntpackage<0, ::param1, ::param2> params_pkg_t;

class download_parser {
public:
  using blockfile_t = ypc::blockfile<0x4788d13e7fefe21f, 1024 * 1024,
                                     256 * ::ypc::utc::max_item_size>;

  download_parser(
      std::vector<std::shared_ptr<ypc::data_source_with_dhash>> &source)
      : m_datasources(source) {}

  inline stbox::bytes do_parse(const stbox::bytes &param) {
    LOG(INFO) << "params: " << param;
    auto pkg = ypc::make_package<params_pkg_t>::from_bytes(param);
    auto data_hash = pkg.get<::param1>();
    auto shu_pkey = pkg.get<::param2>();
    LOG(INFO) << "param1: " << data_hash;
    LOG(INFO) << "param2: " << shu_pkey;
    std::string result_file =
        data_hash.substr(0, 8) + '-' + shu_pkey.substr(0, 8) + ".result.sealed";

    ypc::to_type<stbox::bytes, download_batch_item_t> converter(
        m_datasources[0].get());

    stbox::bytes result_hash;
    ecc::hash_256(stbox::bytes("Fidelius"), result_hash);
    int counter = 0;

    blockfile_t fw;
    fw.open_for_write(result_file.c_str());
    stbox::bytes pkey(64);
    ypc::utc::internal::convert_hex_to_bytes(&shu_pkey[0], shu_pkey.size(),
                                             pkey.data(), pkey.size());

    hpda::processor::internal::filter_impl<download_batch_item_t> match(
        &converter, [&](const download_batch_item_t &v) {
          counter++;
          std::string batch = v.get<::download_batch>();
          // LOG(INFO) << batch;
          // calculate data hash
          stbox::bytes b_batch(batch);
          stbox::bytes t = result_hash + b_batch;
          ecc::hash_256(t, result_hash);

          // encrypt and write to file
          stbox::bytes cipher;
          ecc::encrypt_message_with_prefix(
              pkey, b_batch, ypc::utc::crypto_prefix_arbitrary, cipher);
          fw.append_item(cipher.data(), cipher.size());
          return false;
        });

    hpda::output::internal::memory_output_impl<download_batch_item_t> mo(
        &match);
    mo.get_engine()->run();
    LOG(INFO) << "batch count: " << counter;
    fw.close();
    LOG(INFO) << "do parse done";

    stbox::bytes result(64);
    ypc::utc::internal::convert_bytes_to_hex(
        result_hash.data(), result_hash.size(), result.data(), result.size());
    return result;
  }

protected:
  std::vector<std::shared_ptr<ypc::data_source_with_dhash>> m_datasources;
};
