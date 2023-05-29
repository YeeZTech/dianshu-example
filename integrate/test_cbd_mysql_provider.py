import json
import os
from job_step import job_step
import common
from datetime import datetime


def show_time():
    return datetime.now().strftime('%Y/%m/%d %H:%M:%S.%f')


def get_first_key(crypto):
    return {'public-key': '765f92fcba56939c6f7845bf7cae3c5a2f9490e201e1774330fde9f7cb9d65f8753b6cc7f9bf9aefd54688ba3a8005e2ac8509e80e3575989deb534686e892d4'}


class multistream_job:
    def __init__(self, crypto, name, data_urls, parser_url, plugin_url, input_param, config={}):
        self.crypto = crypto
        self.name = name
        self.data_urls = data_urls
        self.parser_url = parser_url
        self.plugin_url = plugin_url
        self.input = input_param
        self.config = config
        self.all_outputs = list()

    def handle_input_data(self, data_url, param_hash):
        '''
        1. generate a key for the data
        2. encrypt the data
        3. gen forward msg
        4. gen allowance
        @return: the input obj and the allowance obj
        '''

        # 1. generate key
        data_key_file = data_url + ".data.key.json"
        data_shukey_json = job_step.gen_key(self.crypto, data_key_file)
        self.all_outputs.append(data_key_file)

        # 2. call data provider to seal data
        print("[{0}] 加密数据: {1}".format(show_time(), data_url))
        sealed_data_url = data_url + ".sealed"
        sealed_output = data_url + ".sealed.output"
        summary = {}
        summary['data-url'] = data_url
        summary['plugin-path'] = self.plugin_url[data_url]
        summary['sealed-data-url'] = sealed_data_url
        summary['sealed-output'] = sealed_output

        r = job_step.seal_data(self.crypto, data_url, self.plugin_url[data_url],
                               sealed_data_url, sealed_output, data_key_file)
        print("[{0}] 生成加密数据: {1}".format(show_time(), sealed_data_url))
        data_hash = job_step.read_data_hash(sealed_output)
        summary['data-hash'] = data_hash
        self.all_outputs.append(sealed_data_url)
        self.all_outputs.append(sealed_output)

        # use first pkey
        key = get_first_key(self.crypto)
        pkey = key['public-key']
        summary['tee-pkey'] = key['public-key']

        # 3. call terminusto generate forward message
        enclave_hash = job_step.read_parser_hash(self.parser_url)
        forward_result = data_url + ".shukey.foward.json"
        data_forward_json = job_step.forward_message(
            self.crypto, data_key_file, pkey, "", forward_result)
        self.all_outputs.append(forward_result)

        input_obj = {
            "input_data_url": sealed_data_url,
            "input_data_hash": data_hash,
            "shu_info": {
                "shu_pkey": data_shukey_json["public-key"],
                "encrypted_shu_skey": data_forward_json["encrypted_skey"],
                "shu_forward_signature": data_forward_json["forward_sig"],
                "enclave_hash": data_forward_json["enclave_hash"]
            },
            "public-key": data_shukey_json["public-key"],
            "tag": "0"
        }

        # 4. call terminus to generate allowance
        allowance_result = data_url + ".allowance.json"
        allowance_json = job_step.generate_allowance(
            self.crypto, param_hash, data_key_file, enclave_hash, pkey, data_hash, allowance_result)
        print("[{0}] 生成授权: {1}\n\n".format(show_time(), allowance_result))
        self.all_outputs.append(allowance_result)

        return input_obj, allowance_json

    def run(self):
        # 3.0 get param hash
        param_hash_output_url = self.name + "_param_hash.json"
        param_hash = job_step.hash_256(
            self.crypto, self.input, 'hex', param_hash_output_url, self.config)
        self.all_outputs.append(param_hash_output_url)

        # 3.1 call terminus to generate allowances
        input_data = []
        allowances = []
        for data_url in self.data_urls:
            in_data, allowance_data = self.handle_input_data(
                data_url, param_hash)
            input_data.append(in_data)
            allowances.append(allowance_data)


def cbd_mysql():
    name = "CBD-mysql"
    crypto = "stdeth"
    data_path = os.path.join(common.example_dir, './example/CBD-mysql/py/')
    data_t_org_info = os.path.join(data_path, './t_org_info.txt')
    data_t_tax = os.path.join(data_path, './t_tax.txt')
    data_dic_ent_type = os.path.join(data_path, './dic_ent_type.txt')
    data_dic_industry = os.path.join(data_path, './dic_industry.txt')
    data_dic_region = os.path.join(data_path, './dic_region.txt')
    data = [data_t_org_info, data_t_tax, data_dic_ent_type,
            data_dic_industry, data_dic_region]
    parser = os.path.join(common.example_lib,
                          "summary_org_info_parser.signed.so")
    plugin = {
        data_t_org_info: os.path.join(common.example_lib, "libt_org_info_csv_reader.so"),
        data_t_tax: os.path.join(common.example_lib, "libt_tax_csv_reader.so"),
        data_dic_ent_type: os.path.join(common.example_lib, "libdic_ent_type_csv_reader.so"),
        data_dic_industry: os.path.join(common.example_lib, "libdic_industry_csv_reader.so"),
        data_dic_region: os.path.join(common.example_lib, "libdic_region_csv_reader.so"),
    }

    cj = multistream_job(crypto, name, data, parser, plugin, '00aa', {})
    cj.run()


if __name__ == "__main__":
    cbd_mysql()
