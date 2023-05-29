import json
import os
from job_step import job_step
import common
from datetime import datetime


def show_time():
    return datetime.now().strftime('%Y/%m/%d %H:%M:%S.%f')


def get_first_key(crypto):
    return {'public-key': '765f92fcba56939c6f7845bf7cae3c5a2f9490e201e1774330fde9f7cb9d65f8753b6cc7f9bf9aefd54688ba3a8005e2ac8509e80e3575989deb534686e892d4'}


def generate_params(company_label_url, company_label_plugin, industry_url, industry_plugin, county_url, county_plugin, params_output):
    param = {
        "company-label-url": company_label_url,
        "company-label-plugin": company_label_plugin,
        "industry-url": industry_url,
        "industry-plugin": industry_plugin,
        "county-url": county_url,
        "county-plugin": county_plugin,
        "output": params_output,
    }
    r = common.generate_params(**param)
    with open(params_output, 'r') as of:
        return json.load(of)


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
        # data_shukey_json = job_step.gen_key(self.crypto, data_key_file)
        with open(data_key_file, 'r') as of:
            data_shukey_json = json.load(of)
        self.all_outputs.append(data_key_file)

        # 2. call data provider to seal data
        sealed_data_url = data_url + ".sealed"
        sealed_output = data_url + ".sealed.output"
        summary = {}
        summary['data-url'] = data_url
        summary['plugin-path'] = self.plugin_url[data_url]
        summary['sealed-data-url'] = sealed_data_url
        summary['sealed-output'] = sealed_output

        # r = job_step.seal_data(self.crypto, data_url, self.plugin_url[data_url],
        # sealed_data_url, sealed_output, data_key_file)
        data_hash = job_step.read_data_hash(sealed_output)
        summary['data-hash'] = data_hash
        self.all_outputs.append(sealed_data_url)
        self.all_outputs.append(sealed_output)

        # use first pkey
        # key = job_step.get_first_key(self.crypto)
        # pkey = key['public-key']
        # summary['tee-pkey'] = key['public-key']

        # 3. call terminusto generate forward message
        # enclave_hash = job_step.read_parser_hash(self.parser_url)
        forward_result = data_url + ".shukey.foward.json"
        # data_forward_json = job_step.forward_message(
        # self.crypto, data_key_file, pkey, "", forward_result)
        with open(forward_result, 'r') as of:
            data_forward_json = json.load(of)
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
        # allowance_json = job_step.generate_allowance(
        # self.crypto, param_hash, data_key_file, enclave_hash, pkey, data_hash, allowance_result)
        with open(allowance_result, 'r') as of:
            allowance_json = json.load(of)
        self.all_outputs.append(allowance_result)

        return input_obj, allowance_json

    def run(self):
        '''
        1. call terminus to generate key
        2. call terminus to generate request
        3. call terminus to generate allowances
        4. call fid_analyzer
        5. call terminus to decrypt
        '''

        summary = {}
        # 1. generate key
        key_file = self.name + ".key.json"
        shukey_json = job_step.gen_key(self.crypto, key_file)
        # self.all_outputs.append(key_file)

        # use first pkey
        key = get_first_key(self.crypto)
        pkey = key['public-key']
        summary['tee-pkey'] = key['public-key']
        enclave_hash = job_step.read_parser_hash(self.parser_url)

        # 2.0 call terminus to generate forward message
        param_key_forward_result = self.name + ".request.shukey.foward.json"
        rq_forward_json = job_step.forward_message(
            self.crypto, key_file, pkey, enclave_hash, param_key_forward_result)
        self.all_outputs.append(param_key_forward_result)

        # 2.1 call terminus to generate request
        param_output_url = self.name + "_param.json"
        param_json = job_step.generate_request(
            self.crypto, self.input, "hex", key_file, param_output_url, self.config)
        summary['analyzer-input'] = param_json["encrypted-input"]
        self.all_outputs.append(param_output_url)

        # 3.0 get param hash
        param_hash_output_url = self.name + "_param_hash.json"
        # param_hash = job_step.hash_256(
        # self.crypto, self.input, 'hex', param_hash_output_url, self.config)
        with open(param_hash_output_url) as f:
            param_hash = f.readlines()[0]
        self.all_outputs.append(param_hash_output_url)

        # 3.1 call terminus to generate allowances
        input_data = []
        allowances = []
        for data_url in self.data_urls:
            in_data, allowance_data = self.handle_input_data(
                data_url, param_hash)
            input_data.append(in_data)
            allowances.append(allowance_data)

        # 4. call fid_analyzer
        print("\n\n[{0}] 数据处理,初始化...".format(show_time()))
        parser_input_file = self.name + "parser_input.json"
        parser_output_file = self.name + "parser_output.json"
        result_json = job_step.fid_analyzer(shukey_json, rq_forward_json, enclave_hash, input_data,
                                            self.parser_url, pkey, {}, self.crypto, param_json, allowances, parser_input_file, parser_output_file)
        print("\n[{0}] 输出加密结果: {1}".format(
            show_time(), result_json["encrypted_result"]))

        summary['encrypted-result'] = result_json["encrypted_result"]
        summary["result-signature"] = result_json["result_signature"]
        summary["cost-signature"] = result_json["cost_signature"]
        with open(self.name + ".summary.json", "w") as of:
            json.dump(summary, of)
        self.all_outputs.append(parser_input_file)
        self.all_outputs.append(parser_output_file)

        # 6. call terminus to decrypt
        # encrypted_result = summary["encrypted-result"]
        # decrypted_result = self.name + ".result"
        # self.result = job_step.decrypt_result(
        # self.crypto, encrypted_result, key_file, decrypted_result)
        # self.all_outputs.append(decrypted_result)
        self.all_outputs.append("info.json")
        # self.all_outputs.append("{}.summary.json".format(self.name))
        job_step.remove_files(self.all_outputs)


def cbd_1():
    name = "CBD-demo1"
    crypto = "stdeth"
    data_company = os.path.join(common.example_dir, "./dataset/企业信息.csv")
    data_tax = os.path.join(common.example_dir, "./dataset/税收.csv")
    data = [data_company, data_tax]
    parser = os.path.join(common.example_lib, "company_info_parser.signed.so")
    plugin = {
        data_company: os.path.join(common.example_lib, "libcompany_reader.so"),
        data_tax: os.path.join(common.example_lib, "libtax_reader.so"),
    }

    cj = multistream_job(crypto, name, data, parser, plugin, '00aa', {})
    cj.run()


def cbd_join():
    name = "CBD-demo1"
    crypto = "stdeth"
    data_company = os.path.join(common.example_dir, "./dataset/企业信息.csv")
    data_tax = os.path.join(common.example_dir, "./dataset/税收.csv")
    data_company_label = os.path.join(
        common.example_dir, "./dataset/企业类型字典.csv")
    data_industry = os.path.join(common.example_dir, "./dataset/行业字典.csv")
    data_county = os.path.join(common.example_dir, "./dataset/街乡字典.csv")
    data = [data_company, data_tax,
            data_company_label, data_industry, data_county]
    parser = os.path.join(common.example_lib, "company_join_parser.signed.so")
    plugin = {
        data_company: os.path.join(common.example_lib, "libcompany_reader.so"),
        data_tax: os.path.join(common.example_lib, "libtax_reader.so"),
        data_company_label: os.path.join(common.example_lib, "libcompany_label_reader.so"),
        data_industry: os.path.join(common.example_lib, "libindustry_reader.so"),
        data_county: os.path.join(common.example_lib, "libcounty_reader.so"),
    }
    gp = generate_params(data_company_label, plugin[data_company_label],
                         data_industry, plugin[data_industry], data_county, plugin[data_county], 'params.json')

    cj = multistream_job(crypto, name, data, parser, plugin, gp['params'], {})
    cj.run()


if __name__ == "__main__":
    cbd_join()
