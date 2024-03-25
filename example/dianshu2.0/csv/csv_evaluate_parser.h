#include "../json.hpp"
#include "parseCSVLine.h"
#include "user_type.h"
#include "ypc/core_t/analyzer/data_source.h"
#include "ypc/corecommon/to_type.h"
#include "ypc/stbox/ebyte.h"
#include "ypc/stbox/stx_common.h"
#include "ypc/stbox/tsgx/log.h"
#include <hpda/extractor/raw_data.h>
#include <hpda/output/memory_output.h>
#include <hpda/processor/query/filter.h>

define_nt(input_param, std::string);
typedef ff::net::ntpackage<0, input_param> nt_package_t;

class csv_evaluate_parser {
public:
  csv_evaluate_parser(
      std::vector<std::shared_ptr<ypc::data_source_with_dhash>> &source)
      : m_datasources(source) {}

  inline stbox::bytes do_parse(const stbox::bytes &param) {
    LOG(INFO) << "param: " << param;
    ypc::to_type<stbox::bytes, csv_line_item_t> converter(
        m_datasources[0].get());

    int counter = 0;    // 行数
    int column = 0;     // 列数
    int tCol = 0;       // 临时列
    int tRow = 0;       // 临时行
    int MaxColRowStart; // 最大列数的起始行

    hpda::processor::internal::filter_impl<csv_line_item_t> match(
        &converter, [&](const csv_line_item_t &v) {
          counter++;
          return true;
        });

    hpda::output::internal::memory_output_impl<csv_line_item_t> mo(&match);
    mo.get_engine()->run();
    LOG(INFO) << "do parse done";

    std::vector<std::string> rowVec;
    for (auto it : mo.values()) {
      auto row = it.get<csv_line>();

      std::vector<std::string> fields =
          parseCSVLine(row); // 逗号分隔的数据存入fields
      tCol = fields.size();  // 计算列数

      if (tCol > column) {
        column = tCol;         // 记录最大列数
        MaxColRowStart = tRow; // 记录最大列数的起始行,行数从0开始
      }

      rowVec.clear(); // 清空rowVec
      tRow++;         // 进入下一行的列数计算
    }

    std::vector<int> missNumRow(counter, 0);       // 第x列缺失y行
    std::vector<int> missNumCol(column, 0);        // 第p行缺失q列
    std::vector<int> missNullNumRowCount(1002, 0); // 有y个缺失值的行数量
    std::vector<int> missNumColCount(1002, 0); // 有q个缺失值的列数量

    int tempRow = 0;    // 记录循环中的行数
    int totalNulls = 0; // 总缺失值数量

    for (auto it : mo.values()) {
      if (tempRow < MaxColRowStart) {
        tempRow++;
        continue;
      }

      bool incomplete = false;
      auto row = it.get<csv_line>(); // 获取一整行数据

      std::vector<std::string> fields =
          parseCSVLine(row); // 逗号分隔的数据存入fields

      std::vector<bool> rowVecBool(column,
                                   false); // 记录缺失值情况，默认为缺失(false)
      int ti = 0;                          // 记录循环中的列数

      for (int i = 0; i < fields.size(); i++) {
        if (fields[i] != "" && fields[i] != "\r") {
          rowVecBool[i] = true; // 不缺失
        } else {
          missNumCol[i]++;       // 第i列++
          missNumRow[tempRow]++; // 第tempRow行++
        }
      }

      tempRow++; // 进入下一行的缺失值计算
    }

    for (int i = 0; i < column; i++) {
      totalNulls += missNumCol[i];
    }

    std::string nullProportion = std::to_string(
        ((totalNulls * 10000) / ((counter - MaxColRowStart) * column)) / 100.0);

    missNumColCount[0] = column;
    for (int i = 0; i < column; i++) {
      if (missNumCol[i] > 1000) {
        missNumColCount[1001]++;
        missNumColCount[0]--;
      } else {
        missNumColCount[missNumCol[i]]++;
        missNumColCount[0]--;
      }
    }

    for (int i = 0; i < counter; i++) {
      if (i < MaxColRowStart) {
        continue;
      }
      if (missNumRow[i] > 1000) {
        missNullNumRowCount[1001]++;
      } else {
        missNullNumRowCount[missNumRow[i]]++;
      }
    }

    json::JSON pack_j;
    pack_j["rows"] = counter - MaxColRowStart;
    pack_j["cols"] = column;
    pack_j["totalNulls"] = totalNulls;
    pack_j["NullProportion"] =
        nullProportion.substr(0, nullProportion.length() - 4);

    json::JSON t_nulls;
    for (int i = 0; i < 1002; ++i) {
      if (missNumColCount[i] != 0) {
        t_nulls["null_num"] = i;
        t_nulls["count"] = missNumColCount[i];
        pack_j["col_nulls"].append(t_nulls);
      }

      if (missNullNumRowCount[i] != 0) {
        t_nulls["count"] = missNullNumRowCount[i];
        t_nulls["null_num"] = i;
        pack_j["row_nulls"].append(t_nulls);
      }
    }

    return stbox::bytes(pack_j.dump());
  }

protected:
  std::vector<std::shared_ptr<ypc::data_source_with_dhash>> m_datasources;
};
