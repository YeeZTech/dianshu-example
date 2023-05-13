from classic_job import classic_job
import os
import common


def run_one(ele):
    name = ele['name']
    data = os.path.join(common.data_dir, ele['data'])
    parser = os.path.join(common.example_lib,
                          '{}_parser.signed.so'.format(name))
    plugin = os.path.join(common.example_lib,
                          'lib{}_csv_reader.so'.format(name))
    input_param = ele['param']
    cj = classic_job('stdeth', name, data, parser, plugin, input_param, {
        'request-use-js': True
    })
    cj.run()
    print("result is : ", cj.result)


def main():
    l = [
        {'name': 'gdp_query',
         'data': 'year-county-gdp.csv',
         'param': "江阴市",
         },
    ]
    [run_one(ele) for ele in l]


if __name__ == "__main__":
    main()
