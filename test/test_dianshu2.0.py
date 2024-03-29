from classic_job import classic_job
import os
import common


def run_one(ele):
    name = ele['name']
    data = os.path.join(common.data_dir, ele['data'])
    parser = os.path.join(common.example_lib,
                          '{}_parser.signed.so'.format(name))
    plugin = os.path.join(
        common.example_lib, 'lib{}_evaluate_reader.so'.format(ele['reader-type']))
    input_param = ele['param']
    cj = classic_job('stdeth', name, data, parser, plugin, input_param, {
        'provider-use-js': True,
        'request-use-js': True,
        'remove-files': True,
        'offchain-result': True,
        'analyzer-type': ele['analyzer-type'],
    })
    cj.run()
    print("result len is : ", len(cj.result))


def main():
    l = [
        {
            'name': 'mp4_evaluate',
            'reader-type': 'txt',
            'analyzer-type': 'dianshu_analyzer',
            'data': 'piggy.mp4',
            'param': "\"[{\\\"type\\\":\\\"string\\\",\\\"value\\\":\\\"arg1\\\"},{\\\"type\\\":\\\"string\\\",\\\"value\\\":\\\"arg2\\\"}]\"",
        },

        {
            'name': 'txt_evaluate',
            'reader-type': 'txt',
            'analyzer-type': 'fid_analyzer',
            'data': 'rule.txt',
            'param': "\"[{\\\"type\\\":\\\"string\\\",\\\"value\\\":\\\"arg1\\\"},{\\\"type\\\":\\\"string\\\",\\\"value\\\":\\\"arg2\\\"}]\"",
        },

        {
            'name': 'download',
            'reader-type': 'txt',
            'analyzer-type': 'fid_analyzer',
            'data': 'dianshudata.csv',
            'param': "\"[{\\\"type\\\":\\\"string\\\",\\\"value\\\":\\\"arg1\\\"},{\\\"type\\\":\\\"string\\\",\\\"value\\\":\\\"arg2\\\"}]\"",
        },

        {
            'name': 'csv_evaluate',
            'reader-type': 'csv',
            'analyzer-type': 'fid_analyzer',
            'data': 'countries_of_the_world_3.1M_csv.csv',
            'param': "\"[{\\\"type\\\":\\\"string\\\",\\\"value\\\":\\\"arg1\\\"},{\\\"type\\\":\\\"string\\\",\\\"value\\\":\\\"arg2\\\"}]\"",
        },
    ]
    [run_one(ele) for ele in l]


if __name__ == "__main__":
    main()
