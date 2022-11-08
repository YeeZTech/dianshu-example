from classic_job import classic_job
import os
import common

if __name__ == "__main__":
    name = "gdp"
    data = os.path.join(common.data_dir, "year-county-gdp.csv")
    parser = os.path.join(common.lib_dir, "gdp_query_parser.signed.so")
    # plugin = os.path.join(common.lib_dir, "libcolumn_6_csv_reader.so")
    # input_param = "\"[{\\\"type\\\":\\\"string\\\",\\\"value\\\":\\\"潜江市\\\"},{\\\"type\\\":\\\"string\\\",\\\"value\\\":\\\"2020\\\"}]\""
    # cj = classic_job(name, data, parser, plugin, input_param, {
    # 'config': ''
    # })
    # cj.run()

    # print("result is : ", cj.result)
