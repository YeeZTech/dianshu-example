from multistream_job import multistream_job
import json
import os
import common


def car():
    name = "car"
    crypto = "stdeth"
    data_sjdx = os.path.join(common.example_dir, "./dataset/手机短信日志.csv")
    data_sjhd = os.path.join(common.example_dir, "./dataset/手机话单日志.csv")
    data_sjsw = os.path.join(common.example_dir, "./dataset/手机上网日志.csv")
    data_xlgj = os.path.join(common.example_dir, "./dataset/信令轨迹数据.csv")
    data = [data_sjdx, data_sjhd, data_sjsw, data_xlgj]
    parser = os.path.join(common.example_lib, "car_parser.signed.so")
    plugin = {
        data_sjdx: os.path.join(common.example_lib, "libsjdx_log_csv_reader.so"),
        data_sjhd: os.path.join(common.example_lib, "libsjhd_log_csv_reader.so"),
        data_sjsw: os.path.join(common.example_lib, "libsjsw_log_csv_reader.so"),
        data_xlgj: os.path.join(common.example_lib, "libxlgj_csv_reader.so"),
    }
    cj = multistream_job(crypto, name, data, parser, plugin, '00aa', {})
    cj.run()
    print("result is : ", cj.result)


def person_tel():
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


def main():
    car()


if __name__ == "__main__":
    main()
