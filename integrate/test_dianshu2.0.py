import argparse
from classic_job import classic_job
import os
import common

def run_one(name, data, param):
    data_path = os.path.join(common.data_dir, data)
    parser = os.path.join(common.example_lib,
                          '{}_parser.signed.so'.format(name))
    plugin = os.path.join(common.example_lib, 'lib{}_reader.so'.format(name))
    cj = classic_job('stdeth', name, data_path, parser, plugin, param, {
        'request-use-js': True
    })
    cj.run()
    print("result is : ", cj.result)

def main():
    parser = argparse.ArgumentParser(description='Process some integers.')
    parser.add_argument('--function', help='Function to run (csv_evaluate or txt_evaluate)', choices=['csv_evaluate', 'txt_evaluate'])
    parser.add_argument('--data', help='Path to the data file')
    parser.add_argument('--param', help='Input parameter')
    args = parser.parse_args()

    if not args.function:
        print("Please provide the function name using --function option.")
        return

    if not args.data:
        print("Please provide the path to the data file using --data option.")
        return

    if args.function == 'csv_evaluate':
        param = "data_evaluate"
    elif args.function == 'txt_evaluate':
        param = "data_evaluate"

    run_one(args.function, args.data, param)

if __name__ == "__main__":
    main()
