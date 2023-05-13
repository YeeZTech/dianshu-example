import json
import os
import common
from datetime import datetime


def show_time():
    return datetime.now().strftime('%Y/%m/%d %H:%M:%S.%f')


class job_step:
    def gen_key(crypto, shukey_file):
        param = {
            "crypto": crypto,
            "gen-key": "",
            "gen-key": "",
            "no-password": "",
            "output": shukey_file
        }
        common.fid_terminus(**param)
        with open(shukey_file, 'r') as of:
            return json.load(of)

    def seal_data(crypto, data_url, plugin_url, sealed_data_url, sealed_output, data_key_file):
        param = {
            "crypto": crypto,
            "data-url": data_url,
            "plugin-path": plugin_url,
            "sealed-data-url": sealed_data_url,
            "output": sealed_output,
            "use-publickey-file": data_key_file
        }
        return common.fid_data_provider(**param)

    def get_first_key(crypto):
        keys = common.fid_keymgr_list(crypto)
        if len(keys) == 0:
            common.fid_keymgr_create("test", crypto)
        keys = common.fid_keymgr_list(crypto)
        pkey = ''
        private_key = ''
        for k, v in keys.items():
            pkey = v
            private_key = common.get_keymgr_private_key(k, crypto)
            break
        return {'public-key': pkey, "private-key": private_key}

    def read_data_hash(fp):
        with open(fp) as f:
            lines = f.readlines()
            for l in lines:
                if l.startswith("data_id"):
                    ks = l.split("=")
                    return ks[1].strip()

    def read_parser_hash(parser_url):
        param = {
            "enclave": parser_url,
            "output": "info.json"
        }
        r = common.fid_dump(**param)
        with open("info.json") as f:
            data = json.load(f)
            return data["enclave-hash"]

    def forward_message(crypto, shukey_file, dian_pkey, enclave_hash, forward_result):
        param = {
            "crypto": crypto,
            "forward": "",
            "use-privatekey-file": shukey_file,
            "tee-pubkey": dian_pkey,
            "output": forward_result
        }
        if enclave_hash:
            param.update({"use-enclave-hash": enclave_hash})
        common.fid_terminus(**param)
        with open(forward_result, 'r') as of:
            return json.load(of)

    def generate_request(crypto, input_param, param_format, shukey_file, param_output_url, config):
        param = {
            "crypto": crypto,
            "request": "",
            "use-param": input_param,
            "param-format": param_format,
            "use-publickey-file": shukey_file,
            "output": param_output_url
        }
        r = str()
        r = common.fid_terminus(**param)
        with open(param_output_url) as of:
            return json.load(of)

    def fid_analyzer(shukey_json, rq_forward_json, enclave_hash, input_data, parser_url, dian_pkey, model, crypto, param_json, allowances, parser_input_file, parser_output_file):
        parser_input = {
            "shu_info": {
                "shu_pkey": shukey_json["public-key"],
                "encrypted_shu_skey": rq_forward_json["encrypted_skey"],
                "shu_forward_signature": rq_forward_json["forward_sig"],
                "enclave_hash": enclave_hash
            },
            "input_data": input_data,
            "parser_path": parser_url,
            "keymgr_path": common.kmgr_enclave[crypto],
            "parser_enclave_hash": enclave_hash,
            "dian_pkey": dian_pkey,
            "model": model,
            "param": {
                "crypto": crypto,
                "param_data": param_json["encrypted-input"],
                "public-key": shukey_json["public-key"],
            }
        }
        if allowances:
            parser_input['param']['allowances'] = allowances
        with open(parser_input_file, "w") as of:
            json.dump(parser_input, of)
        param = {
            "input": parser_input_file,
            "output": parser_output_file
        }
        r = common.fid_analyzer(**param)
        try:
            with open(parser_output_file) as of:
                return json.load(of)
        except Exception as e:
            # result is not json format
            with open(parser_output_file) as of:
                return of.readlines()

    def decrypt_result(crypto, encrypted_result, shukey_file, decrypted_result):
        param = {
            "crypto": crypto,
            "decrypt": "",
            "use-param": encrypted_result,
            "use-privatekey-file": shukey_file,
            "output": decrypted_result
        }
        r = common.fid_terminus(**param)
        with open(decrypted_result) as f:
            return f.readlines()

    def decrypt_result_key(crypto, encrypted_result, shukey_file, decrypted_result):
        param = {
            "crypto": crypto,
            "decrypt": "",
            "use-param": encrypted_result,
            "use-privatekey-file": shukey_file,
            "output": decrypted_result
        }
        r = common.fid_terminus(**param)
        with open(decrypted_result, 'rb') as f:
            key = bytearray(f.read())
            return ''.join(format(x, '02x') for x in key)
            # return f.readlines()

    def decrypt_result_with_hex(crypto, encrypted_result, shukey, decrypted_result):
        param = {
            "crypto": crypto,
            "decrypt": "",
            "use-param": encrypted_result,
            "use-privatekey-hex": shukey,
            "output": decrypted_result
        }
        r = common.fid_terminus(**param)
        with open(decrypted_result) as f:
            return f.readlines()

    def encrypt_message(crypto, shukey_file, msg, output):
        param = {
            "crypto": crypto,
            "encrypt": "",
            "use-publickey-file": shukey_file,
            "output": output,
            "use-param": msg
        }
        r = common.fid_terminus(**param)
        with open(output, 'r') as of:
            return of.readlines()[0]

    def sign(crypto, data, param_format, private_key_file, output_url):
        param = {
            "crypto": crypto,
            "sign": "",
            "use-param": data,
            "param-format": param_format,
            "output": output_url,
            "use-privatekey-file": private_key_file
        }
        r = common.fid_terminus(**param)
        with open(output_url, 'r') as of:
            return json.load(of)

    def hash_256(crypto, data, param_format, param_hash_output_url, config):
        param = {
            "crypto": crypto,
            "sha3": "",
            "use-param": data,
            "param-format": param_format,
            "output": param_hash_output_url
        }
        r = str()
        r = common.fid_terminus(**param)
        with open(param_hash_output_url) as f:
            return f.readlines()[0]

    def generate_allowance(crypto, param_hash, shukey_file, enclave_hash, dian_pkey, data_hash, allowance_output):
        param = {
            "crypto": crypto,
            "allowance": "",
            "use-param": param_hash,
            "use-privatekey-file": shukey_file,
            "use-enclave-hash": enclave_hash,
            "tee-pubkey": dian_pkey,
            "dhash": data_hash,
            "output": allowance_output
        }
        r = common.fid_terminus(**param)
        with open(allowance_output, 'r') as of:
            return json.load(of)

    def remove_files(file_list):
        [common.execute_cmd('rm -rf {}'.format(f)) for f in file_list]


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
        key = job_step.get_first_key(self.crypto)
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


if __name__ == "__main__":
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