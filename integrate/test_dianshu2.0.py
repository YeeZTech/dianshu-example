from classic_job import classic_job
import os
import common


def run_one(ele):
    name = ele['name']
    data = os.path.join(common.data_dir, ele['data'])
    parser = os.path.join(common.example_lib,
                          '{}_parser.signed.so'.format(name))
    # plugin = os.path.join(common.example_lib, 'lib{}_reader.so'.format(name))
    plugin = os.path.join(common.example_lib, 'libtxt_evaluate_reader.so')
    input_param = ele['param']
    cj = classic_job('stdeth', name, data, parser, plugin, input_param, {
        'request-use-js': True,
        'remove-files': True,
        'offchain-result': True,
    })
    cj.run()
    print("result is : ", cj.result)


def main():
    l = [
        # {'name': 'csv_evaluate',
        # 'data': 'dianshudata.csv',
        # 'param': "\"[{\\\"type\\\":\\\"string\\\",\\\"value\\\":\\\"江阴市\\\"}]\"",
        # },
        # {'name': 'txt_evaluate',
        # 'data': 'dianshudata.csv',
        # 'param': "\"[{\\\"type\\\":\\\"string\\\",\\\"value\\\":\\\"江阴市\\\"}]\"",
        # },
        {'name': 'download',
         'data': 'dianshudata.csv',
         'param': "\"[{\\\"type\\\":\\\"string\\\",\\\"value\\\":\\\"arg1\\\"},{\\\"type\\\":\\\"string\\\",\\\"value\\\":\\\"arg2\\\"}]\"",
         },
    ]
    [run_one(ele) for ele in l]


if __name__ == "__main__":
    main()
