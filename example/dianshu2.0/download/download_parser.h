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
using ntt = ypc::nt<stbox::bytes>;

class download_parser {
public:
  using blockfile_t = ypc::blockfile<0x4788d13e7fefe21f, 2, 1024 * 1024, 256>;

  download_parser(
      std::vector<std::shared_ptr<ypc::data_source_with_dhash>> &source)
      : m_datasources(source) {}

  inline stbox::bytes do_parse(const stbox::bytes &param) {
    LOG(INFO) << "params: " << param;

    stbox::bytes skey;
    ecc::gen_private_key(skey);
    stbox::bytes pkey;
    ecc::generate_pkey_from_skey(skey, pkey);

    std::string pkey_hex(2 * pkey.size(), '0');
    ypc::utc::internal::convert_bytes_to_hex(
        pkey.data(), pkey.size(), (uint8_t *)&pkey_hex[0], pkey_hex.size());
    std::string result_file = pkey_hex.substr(0, 8) + ".result.sealed";

    ypc::to_type<stbox::bytes, data_slice_item_t> converter(
        m_datasources[0].get());

    stbox::bytes result_hash;
    ecc::hash_256(stbox::bytes("Fidelius"), result_hash);
    int counter = 0;

    blockfile_t fw;
    fw.open_for_write(result_file.c_str());
    std::vector<stbox::bytes> batch;
    size_t batch_size = 0;
    hpda::processor::internal::filter_impl<data_slice_item_t> match(
        &converter, [&](const data_slice_item_t &v) {
          counter++;
          typename ypc::cast_obj_to_package<data_slice_item_t>::type pkg;
          pkg.set<::data_slice>(v.get<::data_slice>());
          auto slice = ypc::make_bytes<stbox::bytes>::for_package(pkg);
          batch.push_back(slice);
          batch_size += slice.size();
          if (batch_size >= ypc::utc::max_item_size) {
            write_batch(fw, batch, pkey);
            batch.clear();
            batch_size = 0;
          }

          // calculate data hash
          stbox::bytes t = result_hash + slice;
          ecc::hash_256(t, result_hash);
          return false;
        });

    hpda::output::internal::memory_output_impl<data_slice_item_t> mo(&match);
    mo.get_engine()->run();
    LOG(INFO) << "batch count: " << counter;
    if (!batch.empty()) {
      write_batch(fw, batch, pkey);
      batch.clear();
      batch_size = 0;
    }
    fw.close();
    LOG(INFO) << "do parse done";

    ntt::offchain_result_package_t pkg;
    pkg.set<ntt::encrypted_result>(result_hash);
    pkg.set<ntt::pkey>(pkey);
    pkg.set<ntt::result_encrypt_key>(skey);
    return ypc::make_bytes<stbox::bytes>::for_package(pkg);
  }

protected:
  void write_batch(blockfile_t &fw, const std::vector<stbox::bytes> &batch,
                   const stbox::bytes &public_key) {
    ntt::batch_data_pkg_t pkg;
    stbox::bytes s;
    stbox::bytes batch_str =
        ypc::make_bytes<stbox::bytes>::for_package<ntt::batch_data_pkg_t,
                                                   ntt::batch_data>(batch);
    uint32_t status = ecc::encrypt_message_with_prefix(
        public_key, batch_str, ypc::utc::crypto_prefix_arbitrary, s);
    if (status != 0u) {
      throw std::runtime_error("encrypt message failed!");
    }
    fw.append_item((const char *)s.data(), s.size());
  }

protected:
  std::vector<std::shared_ptr<ypc::data_source_with_dhash>> m_datasources;
};
