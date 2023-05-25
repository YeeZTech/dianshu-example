from multistream_job import multistream_job
import os
import common
import json


def run_one(ele):
    name = ele['name']
    data_a = os.path.join(
        common.example_dir, "./example/multi_statistics/py/value_a.csv")
    data_b = os.path.join(
        common.example_dir, "./example/multi_statistics/py/value_b.csv")
    data = [data_a, data_b]
    parser = os.path.join(common.example_lib,
                          '{}_parser.signed.so'.format(name))
    reader = os.path.join(common.example_lib,
                          'libmulti_statistics_csv_reader.so'.format(name))
    plugin = {data_a: reader, data_b: reader}
    input_param = ele['param']
    cj = multistream_job('stdeth', name, data, parser, plugin, input_param)
    cj.run()
    print("result is : ", cj.result)


def main():
    l = [
        {
            'name': 'max',
            'param': "00"
        },
        {
            'name': 'mean',
            'param': "00"
        },
        {
            'name': 'variance',
            'param': "00"
        },
        # {
        # 'name': 'psi',
        # 'param': "00"
        # },
    ]
    [run_one(ele) for ele in l]


if __name__ == "__main__":
    main()
