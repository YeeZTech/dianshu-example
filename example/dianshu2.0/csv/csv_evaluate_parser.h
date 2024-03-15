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

    int counter = 0; // 行数

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

    stbox::bytes result; // 返回结果
    int column = 0; // 列数
    int tCol = 0; // 临时列数
    int tRow = 0; // 临时行数
    int MaxColRowStart; // 最大列数的起始行
    int incomplete_row = 0; // 不完整的行数，暂时没在结果中返回
    
    std::vector<std::string> rowVec; //存储行中被逗号分隔的数据，用于计算列数
    std::string temp = "";
    for ( auto it : mo.values() ) {
      auto row = it.get<csv_line>();
      // LOG(INFO) << "rowdata: " << row;

      bool betweenQuotes = false;
      for (int i = 0; i < row.size(); i++) {
        if(row[i] == '\"'){
          betweenQuotes = !betweenQuotes;
          continue;
        }

        if (row[i] == ',' && !betweenQuotes) {
          rowVec.push_back(temp); // 逗号分隔的数据存入rowVec
          temp = ""; // 清空temp
        } else {
          temp += row[i]; 
        }
      }
      rowVec.push_back(temp); // 最后一个逗号分隔的数据存入rowVec
      tCol = rowVec.size(); // 计算列数

      if (tCol > column) {
        column = tCol; // 记录最大列数
        MaxColRowStart = tRow; // 记录最大列数的起始行,行数是从0开始的，所以第三行为最大列的话这里应该是2
      }

      // LOG(INFO) << "tCol: " << tCol << "   MaxColRowStart: " << MaxColRowStart;
      
      rowVec.clear(); // 清空rowVec
      tRow++; // 进入下一行的列数计算

    }

    LOG(INFO) << "MaxColRowStart: " << MaxColRowStart;
    LOG(INFO) << "column: " << column;
    
    std::vector<int> missNumRow(counter, 0); // 第x列缺失y行
    std::vector<int> missNumCol(column, 0);  // 第p行缺失q列

    std::vector<int> missNullNumRowCount(1002, 0); // 有y个缺失值的行数量统计
    std::vector<int> missNumColCount(1002, 0);  // 有q个缺失值的列数量统计

    int tempRow = 0; // 记录循环中的行数
    int totalNulls = 0; // 总缺失值数量

    for ( auto it : mo.values() ) {
      if(tempRow < MaxColRowStart){
        tempRow++;
        continue;
      }

      bool incomplete = false;
      auto row = it.get<csv_line>(); // 获取一整行数据

      std::vector<bool> rowVecBool(column, false); // 记录行中每列的缺失值情况，默认为缺失(false),不缺失时置为true

      int ti = 0; // 记录循环中的列数

      bool betweenQuotes = false;
      for (int i = 0; i < row.size(); i++) {
        
        if(row[i] == '\"'){
          betweenQuotes = !betweenQuotes;
          continue;
        }

        if(row[i] == ',' && !betweenQuotes){
          ti++;
        } else {
          if(rowVecBool[ti] == false){
            rowVecBool[ti] = true;
          }
        }
      }

      for (int i = 0; i < column; i++) {
        if (!rowVecBool[i]) {
          missNumCol[i]++; // 第i列++
          missNumRow[tempRow]++; // 第tempRow行++

          if(!incomplete){
            incomplete = true;
            incomplete_row++; // 不完整行，暂时没在结果中返回
          }
        }
      }

      std::string rowVec="";
      for(int i =0; i<column; i++){        
        if(rowVecBool[i]){
          rowVec += "1";
        } else {
          rowVec += "0";
        }

        rowVec += ",";
      }
      LOG(INFO) << "row" << tempRow << "  rowVec: " << rowVec;

      tempRow++; // 进入下一行的缺失值计算
    }


    result += stbox::bytes( "{\"rows\":" + std::to_string(counter - MaxColRowStart) + ",");
    result += stbox::bytes( "\"cols\":" + std::to_string(column) + ",");

    for(int i = 0; i < column; i++){
      totalNulls += missNumCol[i];
    }
    result += stbox::bytes( "\"totalNulls\":" + std::to_string(totalNulls) + ",");

    std::string nullProportion = std::to_string(((totalNulls*10000) / ((counter - MaxColRowStart) * column)) / 100.0);

    // 缺失值比例(保留两位小数)
    result += stbox::bytes( "\"NullProportion\":" + nullProportion.substr(0, nullProportion.length() - 4) + ",");



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

    missNumColCount[0] = column;

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
      
      if(i<MaxColRowStart){
        continue;
      }

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
