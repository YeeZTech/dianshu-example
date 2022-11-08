from classic_job import classic_job
import os
import common


def run_one(ele):
    name = ele['name']
    data = os.path.join(common.data_dir, ele['data'])
    parser = os.path.join(common.lib_dir, '{}_parser.signed.so'.format(name))
    plugin = os.path.join(common.lib_dir, 'lib{}_csv_reader.so'.format(name))
    input_param = ele['param']
    cj = classic_job('stdeth', name, data, parser, plugin, input_param, {
        'request-use-js': True
    })
    cj.run()
    print("result is : ", cj.result)


def main():
    l = [
        {'name': 'gdp_quarter_query',
         'data': 'quarter-province-gdp.csv',
         'param': "\"[{\\\"type\\\":\\\"string\\\",\\\"value\\\":\\\"浙江省\\\"}]\"",
         },
        {'name': 'gdp_prediction',
         'data': 'monthly-radiance.csv',
         'param': "\"[{\\\"type\\\":\\\"string\\\",\\\"value\\\":\\\"江苏省\\\"}]\"",
         },
        {'name': 'gdp_query',
         'data': 'year-county-gdp.csv',
         'param': "\"[{\\\"type\\\":\\\"string\\\",\\\"value\\\":\\\"兰陵县\\\"},{\\\"type\\\":\\\"string\\\",\\\"value\\\":\\\"2020\\\"}]\"",
         },
        {'name': 'radiance_query',
         'data': 'monthly-radiance.csv',
         'param': "\"[{\\\"type\\\":\\\"string\\\",\\\"value\\\":\\\"海淀区\\\"}]\"",
         },
    ]
    [run_one(ele) for ele in l]


if __name__ == "__main__":
    main()
