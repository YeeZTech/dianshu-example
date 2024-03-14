#include "user_type.h"
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

class csv_evaluate_parser {
public:
  csv_evaluate_parser(
      std::vector<std::shared_ptr<ypc::data_source_with_dhash>> &source)
      : m_datasources(source) {}

  inline stbox::bytes do_parse(const stbox::bytes &param) {
    LOG(INFO) << "param: " << param;
    ypc::to_type<stbox::bytes, csv_line_item_t> converter(
        m_datasources[0].get());

    int counter = 0;
    hpda::processor::internal::filter_impl<csv_line_item_t> match(
        &converter, [&](const csv_line_item_t &v) {
          counter++;
          // std::string line = v.get<::csv_line>();
          // LOG(INFO) << line;
          return true;
        });

    hpda::output::internal::memory_output_impl<csv_line_item_t> mo(&match);
    mo.get_engine()->run();
    LOG(INFO) << "do parse done";

    stbox::bytes result;
    int column = 0;
    int incomplete_row = 0;
    for ( auto it : mo.values() ) {
      auto row = it.get<csv_line>();
      // LOG(INFO) << "rowdata: " << row;
      std::vector<std::string> rowVec;
      std::string temp = "";
      for (int i = 0; i < row.size(); i++) {
        if (row[i] == ',') {
          rowVec.push_back(temp);
          temp = "";
        } else {
          temp += row[i];
        }
      }
      rowVec.push_back(temp);
      column = rowVec.size();
      break;
    }
    
    std::vector<int> missNumRow(counter, 0); // 具体行中缺失值的个数
    std::vector<int> missNumCol(column, 0);  // 具体列中缺失值的个数

    std::vector<int> missNumColCount(1002, 0);  // 有x个缺失值的列数量统计
    std::vector<int> missNullNumRowCount(1002, 0); // 有x个缺失值的行数量统计
    
    int tempRow = 0;
    int totalNulls = 0;

    for ( auto it : mo.values() ) {
      bool incomplete = false;
      auto row = it.get<csv_line>();
      std::vector<std::string> rowVec;
      std::string temp = "";
      for (int i = 0; i < row.size(); i++) {
        if (row[i] == ',') {
          rowVec.push_back(temp);
          temp = "";
        } else {
          temp += row[i];
        }
      }
      rowVec.push_back(temp);
      for (int i = 0; i < column; i++) {
        if (rowVec[i] == "") {
          missNumCol[i]++;
          missNumRow[tempRow]++;
          if(!incomplete){
            incomplete = true;
            incomplete_row++;
          }
        }
      }
      tempRow++;
    }




    result += stbox::bytes( "{\"rows\":" + std::to_string(counter) + ",");
    result += stbox::bytes( "\"cols\":" + std::to_string(column) + ",");

    for(int i = 0; i < column; i++){
      totalNulls += missNumCol[i];
    }
    result += stbox::bytes( "\"totalNulls\":" + std::to_string(totalNulls) + ",");

    // result += stbox::bytes( "\"col_nulls\":[" );
    // for(int i = 0; i < column; i++){
    //   result += stbox::bytes( "{\"col\":" + std::to_string(i+1) + ",\"col_null\":"+ std::to_string(missNumCol[i]) + "}");
    //   if(i!=column-1){
    //     result += stbox::bytes( ",");
    //   }
    // }
    // result += stbox::bytes( "],");


    // result += stbox::bytes( "\"row_nulls\":[" );
    // bool exist = false;
    // for(int i = 0; i < counter; i++){
    //   if(missNumRow[i] == 0){
    //     continue;
    //   }
    //   if(exist){
    //     result += stbox::bytes( ",");
    //   } else {
    //     exist = true;
    //   }
    //   result += stbox::bytes("{\"row\":" + std::to_string(i+1) + ",\"row_null\":"+ std::to_string(missNumRow[i])+"}");
    // }

    missNumColCount[0] = counter;

    for(int i = 0; i < column; i++){
      if(missNumCol[i] > 1000){
        missNumColCount[1001]++;
        missNumColCount[0]--;
      } else {
        missNumColCount[missNumCol[i]]++;
        missNumColCount[0]--;
      }
    }

    for(int i = 0; i < counter; i++){
      if(missNumRow[i] > 1000){
        missNullNumRowCount[1001]++;
      } else {
        missNullNumRowCount[missNumRow[i]]++;
      }
    }

    result += stbox::bytes( "\"col_nulls\":[" );
    for(int i = 0; i < 1002; i++){
      if(missNumColCount[i] == 0){
        continue;
      }
      result += stbox::bytes( "{\"null_num\":" + std::to_string(i) + ",\"count\":"+ std::to_string(missNumColCount[i]) + "}");
      if(i!=1001){
        result += stbox::bytes( ",");
      }
    }
    result += stbox::bytes( "],");



    result += stbox::bytes( "\"row_nulls\":[" );
    for(int i = 0; i < 1002; i++){
      if(missNullNumRowCount[i] == 0){
        continue;
      }
      result += stbox::bytes( "{\"null_num\":" + std::to_string(i) + ",\"count\":"+ std::to_string(missNullNumRowCount[i]) + "}");
      if(i!=1001){
        result += stbox::bytes( ",");
      }
    }



    result += stbox::bytes( "]}");
    return result;
  }

protected:
  std::vector<std::shared_ptr<ypc::data_source_with_dhash>> m_datasources;
};
