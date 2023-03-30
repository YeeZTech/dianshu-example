from multistream_job import multistream_job
import json
import os
import common


if __name__ == "__main__":
    name = "person_tel"
    crypto = "stdeth"
    data_user = os.path.join(common.example_dir, "./dataset/user.csv")
    data_station = os.path.join(common.example_dir, "./dataset/station.csv")
    data = [data_user, data_station]
    parser = os.path.join(common.example_lib, "person_tel_parser.signed.so")
    plugin = {
        data_user: os.path.join(common.example_lib, "libuser_reader.so"),
        data_station: os.path.join(common.example_lib, "libstation_reader.so"),
    }

    cj = multistream_job(crypto, name, data, parser, plugin, '00aa', {})
    cj.run()

    print("result is : ", cj.result)
