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
define_nt(video_frame, std::vector<unsigned char>); // 视频帧数据 
define_nt(total_duration, double); // 视频总时长
define_nt(frame_ts, double); // 视频帧时间戳
typedef ::ff::util::ntobject<total_duration, video_frame, frame_ts> video_frame_t; // 将视频帧数据和时间戳组合为一个对象


class mp4_evaluate_parser {
public:
  mp4_evaluate_parser(std::vector<std::shared_ptr<ypc::data_source_with_dhash>> &source)
      : m_datasources(source) {}

  inline stbox::bytes do_parse(const stbox::bytes &param) {
    LOG(INFO) << "params: " << param; // 打印日志：输出参数信息

    // 设置数据转换器，用于将原始数据转换为指定的数据片段项
    ypc::to_type<stbox::bytes, data_slice_item_t> converter(m_datasources[0].get());

    // 生成私钥
    stbox::bytes skey;
    ecc::gen_private_key(skey);
    // 将私钥转换为十六进制字符串
    std::string skey_hex(2 * skey.size(), '0');
    ypc::utc::internal::convert_bytes_to_hex(skey.data(), skey.size(), (uint8_t *)&skey_hex[0], skey_hex.size());
    // 根据私钥生成文件名
    std::string filename = skey_hex.substr(0, 8) + ".raw";
    
    ypc::cxxfile ofs;
    ofs.open(filename.c_str(), ypc::ios_base::out | ypc::ios_base::binary);
    bool flag = false;
    int count = 0;

    // 设置数据过滤器，用于处理数据片段
    hpda::processor::internal::filter_impl<data_slice_item_t> match(&converter, [&](const data_slice_item_t &v) {
      auto slice = v.get<::data_slice>(); // 获取数据片段
      ofs.write(slice.data(), slice.size()); // 将数据片段写入文件
      count++; 
      return false; 
    });

    hpda::output::internal::memory_output_impl<data_slice_item_t> mo(&match);
    mo.get_engine()->run(); 
    ofs.close(); 
    LOG(INFO) << "do parse done"; 
    LOG(INFO) << "count match time: " << count; 

    // 初始化JSON对象，用于存储结果
    json::JSON res;
    double total_duration = 0; 
    const int frame_capture_len = 3; // 定义每次获取帧的数量，目前设置为3，每次从视频中随机取出3帧图像

    for (int i = 0; i < frame_capture_len; i++) {
      uint8_t *data;
      uint32_t len;

      // 调用OCALL方法利用OpenCV从文件中获取帧
      auto ret = stbox::ocall_cast<uint32_t>(ocall_get_frame)(filename.c_str(), filename.size(), &data, &len);
      if (ret) {
        LOG(ERROR) << "ocall_get_frame_list ret: " << ret; // 处理失败，打印错误日志
      }
      LOG(INFO) << "ocall_get_frame_list succ, len: " << len; // 处理成功，打印日志
      // 从字节数据中解析出视频帧数据包
      auto pkg = ypc::make_package<typename ypc::cast_obj_to_package<video_frame_t>::type>::from_bytes(stbox::bytes(data, len));
      std::vector<unsigned char> img = pkg.get<::video_frame>();
      std::string s((const char *)img.data(), img.size()); // 将视频帧数据转换为字符串
      std::string base64_s = base64_encode(s); // 对视频帧数据进行Base64编码
      LOG(INFO) << "base64 size: " << base64_s.size(); // 打印Base64编码后的大小
      double frame_ts = pkg.get<::frame_ts>(); // 获取帧时间戳
      total_duration = pkg.get<::total_duration>(); // 更新视频总时长

      // 构建截取图像帧的JSON对象
      json::JSON ele;
      ele["picture"] = base64_s; // 添加图片的Base64编码
      ele["duration"] = std::to_string(int(frame_ts)); // 添加帧时间戳
      res["positions"].append(ele); 
    }

    res["totalDuration"] = std::to_string(int(total_duration)); // 添加视频总时长
    // 清空文件内容，避免数据残留
    ofs.open(filename.c_str(), ypc::ios_base::out | ypc::ios_base::trunc);
    ofs.close();

    // 将结果转换为字节形式并返回
    return stbox::bytes(res.dump());
  }

protected:
  std::vector<std::shared_ptr<ypc::data_source_with_dhash>> m_datasources;
};
