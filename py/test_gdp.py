from classic_job import classic_job
import os
import common

if __name__ == "__main__":
    name = "gdp"
    data = os.path.join(common.bin_dir, "quarter-province-gdp.csv")
    parser = os.path.join(common.lib_dir, "gdp_prediction_parser.signed.so")
    plugin = ''
    # county year query parameter
    # input_param = "\"[{\\\"type\\\":\\\"string\\\",\\\"value\\\":\\\"兰陵县\\\"},{\\\"type\\\":\\\"string\\\",\\\"value\\\":\\\"2020\\\"}]\""
    # province quarter query parameter
    # input_param = "\"[{\\\"type\\\":\\\"string\\\",\\\"value\\\":\\\"浙江省\\\"}]\""
    # predict parameter
    input_param = "\"[{\\\"type\\\":\\\"string\\\",\\\"value\\\":\\\"12321\\\"}]\""
    cj = classic_job(name, data, parser, plugin, input_param, {
        'config': 'column_6.csv.json'
    })
    cj.run()

    print("result is : ", cj.result)
