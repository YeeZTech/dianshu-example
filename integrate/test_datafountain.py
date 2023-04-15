from multistream_job import multistream_job
import json
import os
import common


if __name__ == "__main__":
    name = "datafountain"
    crypto = "stdeth"
    data_a = os.path.join(
        common.example_dir, "./dataset/ordered_simple_train_a.txt")
    data_b = os.path.join(
        common.example_dir, "./dataset/ordered_simple_train_b.txt")
    data = [data_a, data_b]
    parser = os.path.join(common.example_lib, "train_parser.signed.so")
    plugin = {
        data_a: os.path.join(common.example_lib, "libtrain_a_reader.so"),
        data_b: os.path.join(common.example_lib, "libtrain_b_reader.so"),
    }

    cj = multistream_job(crypto, name, data, parser, plugin, '00aa', {})
    cj.run()

    print("result is : ", cj.result)
