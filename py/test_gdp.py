from classic_job import classic_job
import os
import common

if __name__ == "__main__":
    name = "gdp"
    # county year query parameter
    data = os.path.join(common.bin_dir, "year-county-gdp.csv")
    parser = os.path.join(common.lib_dir, "gdp_query_parser.signed.so")
    input_param = "\"[{\\\"type\\\":\\\"string\\\",\\\"value\\\":\\\"兰陵县\\\"},{\\\"type\\\":\\\"string\\\",\\\"value\\\":\\\"2020\\\"}]\""

    # province quarter query parameter
    data = os.path.join(common.bin_dir, "quarter-province-gdp.csv")
    parser = os.path.join(common.lib_dir, "gdp_quarter_query_parser.signed.so")
    input_param = "\"[{\\\"type\\\":\\\"string\\\",\\\"value\\\":\\\"浙江省\\\"}]\""

    # predict parameter
    data = os.path.join(common.bin_dir, "iris.data")
    parser = os.path.join(common.lib_dir, "gdp_prediction_parser.signed.so")
    input_param = "\"[{\\\"type\\\":\\\"string\\\",\\\"value\\\":\\\"12321\\\"}]\""

    # radiance query parameter
    data = os.path.join(common.bin_dir, "monthly-radiance.csv")
    parser = os.path.join(common.lib_dir, "radiance_query_parser.signed.so")
    input_param = "\"[{\\\"type\\\":\\\"string\\\",\\\"value\\\":\\\"花溪县\\\"}]\""

    plugin = ''
    cj = classic_job(name, data, parser, plugin, input_param, {
        'config': 'column_6.csv.json'
    })
    cj.run()

    print("result is : ", cj.result)
