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
        # {'name': 'gdp_quarter_query',
        # 'data': 'quarter-province-gdp.csv',
        # 'param': "\"[{\\\"type\\\":\\\"string\\\",\\\"value\\\":\\\"33\\\"}]\"",
        # },
        # {'name': 'gdp_quarter_query',
        # 'data': 'quarter-province-gdp.csv',
        # 'param': "\"[{\\\"type\\\":\\\"string\\\",\\\"value\\\":\\\"11\\\"}]\"",
        # },
        {'name': 'gdp_quarter_query',
         'data': 'quarter-province-gdp.csv',
         'param': "\"[{\\\"type\\\":\\\"string\\\",\\\"value\\\":\\\"00\\\"}]\"",
         },
        # {'name': 'gdp_quarter_query',
        # 'data': 'quarter-province-gdp.csv',
        # 'param': "\"[{\\\"type\\\":\\\"string\\\",\\\"value\\\":\\\"全国\\\"}]\"",
        # },

        # {'name': 'gdp_query',
        # 'data': 'year-county-gdp.csv',
        # 'param': "\"[{\\\"type\\\":\\\"string\\\",\\\"value\\\":\\\"江阴市\\\"}]\"",
        # },
        # TODO need dataset of provinces and cities
        {'name': 'gdp_query_v2',
         'data': 'year-county-gdp-v2.csv',
         'param': "\"[{\\\"type\\\":\\\"string\\\",\\\"value\\\":\\\"361123\\\"}]\"",
         },

        {'name': 'gdp_prediction_v2',
         'data': 'gdp-prediction.csv',
         'param': "\"[{\\\"type\\\":\\\"string\\\",\\\"value\\\":\\\"210421\\\"}]\"",
         },
        # {'name': 'gdp_prediction_v2',
        # 'data': 'gdp-prediction.csv',
        # 'param': "\"[{\\\"type\\\":\\\"string\\\",\\\"value\\\":\\\"310151\\\"}]\"",
        # },

        # {'name': 'radiance_query',
        # 'data': 'monthly-radiance.csv',
        # 'param': "\"[{\\\"type\\\":\\\"string\\\",\\\"value\\\":\\\"海淀区\\\"}]\"",
        # },
        # {'name': 'radiance_query',
        # 'data': 'monthly-radiance.csv',
        # 'param': "\"[{\\\"type\\\":\\\"string\\\",\\\"value\\\":\\\"宜昌市\\\"}]\"",
        # },
        # {'name': 'radiance_query',
        # 'data': 'monthly-radiance.csv',
        # 'param': "\"[{\\\"type\\\":\\\"string\\\",\\\"value\\\":\\\"62\\\"}]\"",
        # },
        # {'name': 'radiance_query',
        # 'data': 'monthly-radiance.csv',
        # 'param': "\"[{\\\"type\\\":\\\"string\\\",\\\"value\\\":\\\"2112\\\"}]\"",
        # },
        # {'name': 'radiance_query',
        # 'data': 'monthly-radiance.csv',
        # 'param': "\"[{\\\"type\\\":\\\"string\\\",\\\"value\\\":\\\"210281\\\"}]\"",
        # },
        {'name': 'radiance_query',
         'data': 'monthly-radiance.csv',
         'param': "\"[{\\\"type\\\":\\\"string\\\",\\\"value\\\":\\\"110108\\\"}]\"",
         },
    ]
    [run_one(ele) for ele in l]


if __name__ == "__main__":
    main()
