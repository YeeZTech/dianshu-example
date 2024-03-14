# import argparse
# from classic_job import classic_job
# import os
# import common

# def run_one(name, data, param):
#     data_path = os.path.join(common.data_dir, data)
#     parser = os.path.join(common.example_lib,
#                           '{}_parser.signed.so'.format(name))
#     plugin = os.path.join(common.example_lib, 'lib{}_reader.so'.format(name))
#     cj = classic_job('stdeth', name, data_path, parser, plugin, param, {
#         'request-use-js': True
#     })
#     cj.run()
#     print("result is : ", cj.result)

# def main():
#     parser = argparse.ArgumentParser(description='Process some integers.')
#     parser.add_argument('--function', help='Function to run (csv_evaluate or txt_evaluate)', choices=['csv_evaluate', 'txt_evaluate'])
#     parser.add_argument('--data', help='Path to the data file')
#     parser.add_argument('--param', help='Input parameter')
#     args = parser.parse_args()

#     if not args.function:
#         print("Please provide the function name using --function option.")
#         return

#     if not args.data:
#         print("Please provide the path to the data file using --data option.")
#         return

#     if args.function == 'csv_evaluate':
#         param = "data_evaluate"
#     elif args.function == 'txt_evaluate':
#         param = "data_evaluate"

#     run_one(args.function, args.data, param)

# if __name__ == "__main__":
#     main()


from classic_job import classic_job
import os
import common


def run_one(ele):
    name = ele['name']
    data = os.path.join(common.data_dir, ele['data'])
    parser = os.path.join(common.example_lib,
                          '{}_parser.signed.so'.format(name))
    plugin = os.path.join(common.example_lib, 'lib{}_reader.so'.format(name))
    input_param = ele['param']
    cj = classic_job('stdeth', name, data, parser, plugin, input_param, {
        'request-use-js': True
    })
    cj.run()
    print("result is : ", cj.result)


def main():
    l = [
        # {'name': 'txt_evaluate',
        # 'data': '代码评估测试2.txt',
        # 'param': "TXT质量评估:小于100字节的内容",
        # },
        
        # {'name': 'txt_evaluate',
        # 'data': '代码评估测试3.txt',
        # 'param': "TXT质量评估:一个batch内的内容",
        # },
        
        # {'name': 'txt_evaluate',
        #  'data': '代码评估测试.txt',
        #  'param': "TXT质量评估:多个batch的内容",
        # },

        # {'name': 'txt_evaluate',
        # 'data': 'txt评估含引号.txt',
        # 'param': "TXT质量评估:包含特殊字符的txt内容",
        # },

        {'name': 'csv_evaluate',
        'data': 'dianshudata.txt',
        'param': "CSV质量评估",
        },
    ]
    [run_one(ele) for ele in l]


if __name__ == "__main__":
    main()