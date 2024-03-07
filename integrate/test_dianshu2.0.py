import argparse
from classic_job import classic_job
import os
import common

def run_one(ele, data):
    name = ele['name']
    data_path = os.path.join(common.data_dir, data)
    parser = os.path.join(common.example_lib,
                          '{}_parser.signed.so'.format(name))
    plugin = os.path.join(common.example_lib, 'lib{}_reader.so'.format(name))
    input_param = ele['param']
    cj = classic_job('stdeth', name, data_path, parser, plugin, input_param, {
        'request-use-js': True
    })
    cj.run()
    print("result is : ", cj.result)

def main():
    parser = argparse.ArgumentParser(description='Process some integers.')
    parser.add_argument('--csv', help='Path to the CSV file')
    args = parser.parse_args()

    if not args.csv:
        print("Please provide the path to the CSV file using --csv option.")
        return

    l = [
        {'name': 'csv_evaluate',
        'param': "data_evaluate",
        },
        # {'name': 'txt_evaluate',
        #  'param': "\"[{\\\"type\\\":\\\"string\\\",\\\"value\\\":\\\"江阴市\\\"}]\"",
        #  },
    ]
    [run_one(ele, args.csv) for ele in l]

if __name__ == "__main__":
    main()
