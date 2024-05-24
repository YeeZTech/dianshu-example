#include "../json.hpp"
#include "user_type.h"
#include "string_process.h"
#include "ypc/core_t/analyzer/data_source.h"
#include "ypc/stbox/ebyte.h"
#include "ypc/stbox/stx_common.h"
#include "ypc/stbox/tsgx/log.h"

#include "ypc/corecommon/to_type.h"
#include <hpda/extractor/raw_data.h>
#include <hpda/output/memory_output.h>
#include <hpda/processor/query/filter.h>

define_nt(input_param, std::string);
typedef ff::net::ntpackage<0, input_param> nt_package_t;

class json_evaluate_parser {
public:
  json_evaluate_parser(
      std::vector<std::shared_ptr<ypc::data_source_with_dhash>> &source)
      : m_datasources(source) {}

  inline stbox::bytes do_parse(const stbox::bytes &param) {
    ypc::to_type<stbox::bytes, txt_batch_item_t> converter(
        m_datasources[0].get());
    
    hpda::processor::internal::filter_impl<txt_batch_item_t> match(
        &converter, [&](const txt_batch_item_t &v) {
          std::string batch = v.get<::txt_batch>();
          return true;
        });
    
    hpda::output::internal::memory_output_impl<txt_batch_item_t> mo(&match);
    mo.get_engine()->run();

    std::string batch;
    for (auto it : mo.values()) {
      batch += it.get<txt_batch>(); 
    //   LOG(INFO) << "Batch: " << batch;  

    }

    json::JSON json_format_data = json::JSON::Load(batch);


    long long nullCount = 0;
    int totalElements = countElements(json_format_data, nullCount);
    json::JSON topTenPercent = extractTopPercent(json_format_data, totalElements);

    // LOG(INFO) << "Total elements: " << totalElements << "Null values: " << nullCount;

    json::JSON res;

    json::JSON ele;
    ele["type"] = "text";
    ele["label"] = "键值对总数";
    ele["value"] = std::to_string(int(totalElements));
    res["result"].append(ele);

    json::JSON ele1;
    ele1["type"] = "text";
    ele1["label"] = "总缺失值";
    ele1["value"] = std::to_string(nullCount);
    res["result"].append(ele1);

    json::JSON ele2;
    ele2["type"] = "text";
    ele2["label"] = "缺失值占比";
    std::string nullProportion = std::to_string(((nullCount * 10000) / totalElements) / 100.0);
    ele2["value"] = nullProportion.substr(0, nullProportion.length() - 4) + "%";
    res["result"].append(ele2);

    json::JSON ele3;
    ele3["type"] = "json";
    ele3["label"] = "抽取10%键值对内容";
    ele3["value"] = topTenPercent;
    res["result"].append(ele3);
 

    return stbox::bytes(res.dump());
  }


  int countElements(const json::JSON& json, long long& nullCount) {
    int count = 0;
    switch (json.JSONType()) {
        case json::JSON::Class::Object: {
            for (auto& pair : json.ObjectRange()) {
                count += countElements(pair.second, nullCount);
            }
            break;
        }
        case json::JSON::Class::Array: {
            for (auto& item : json.ArrayRange()) {
                count += countElements(item, nullCount);
            }
            break;
        }
        case json::JSON::Class::Null:
            nullCount++;
            count++;
            break;
        case json::JSON::Class::String:
            if (json.ToString() == "") nullCount++;
            count++;
            break;
        default:
            count++;
            break;
    }
    return count;
}

json::JSON extractTopPercent(const json::JSON& json, int totalElements, double percent = 0.1) {
    static int currentCount = 0;
    int limit = static_cast<int>(totalElements * percent);

    if (json.JSONType() == json::JSON::Class::Object) {
        json::JSON newJson = json::Object();
        for (auto& pair : json.ObjectRange()) {
            if (currentCount >= limit) break;
            newJson[pair.first] = extractTopPercent(pair.second, totalElements, percent);
        }
        return newJson;
    } else if (json.JSONType() == json::JSON::Class::Array) {
        json::JSON newJson = json::Array();
        for (auto& item : json.ArrayRange()) {
            if (currentCount >= limit) break;
            newJson.append(extractTopPercent(item, totalElements, percent));
        }
        return newJson;
    } else {
        currentCount++;
        return json;
    }
}

protected:
  std::vector<std::shared_ptr<ypc::data_source_with_dhash>> m_datasources;
};
