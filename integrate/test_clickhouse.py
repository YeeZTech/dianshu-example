from classic_job import classic_job
import os
import common

if __name__ == "__main__":
    name = "clickhouse"
    data = os.path.join(common.bin_dir, "msisdn.data")
    parser = os.path.join(common.lib_dir, "clickhouse_query.signed.so")
    plugin = os.path.join(common.lib_dir, "libmsisdn_csv_reader.so")
    # input_param = "\"[{\\\"type\\\":\\\"string\\\",\\\"value\\\":\\\"8618000000002\\\"}]\""
    input_param = "8617300000002"
    cj = classic_job(name, data, parser, plugin, input_param)
    cj.run()

    print("result is : ", cj.result)
