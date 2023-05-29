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
        return {'public-key': '765f92fcba56939c6f7845bf7cae3c5a2f9490e201e1774330fde9f7cb9d65f8753b6cc7f9bf9aefd54688ba3a8005e2ac8509e80e3575989deb534686e892d4'}
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


class classic_job:
    def __init__(self, crypto, name, data_url, parser_url, plugin_url, input_param, config={}):
        self.crypto = crypto
        self.name = name
        self.data_url = data_url
        self.parser_url = parser_url
        self.plugin_url = plugin_url
        self.input = input_param
        self.all_outputs = list()
        self.config = config

    def run(self):
        '''
        1. call terminus to generate key
        2. call data provider to seal data
        3. call terminus to generate forward message
        4. call terminus to generate request
        5. call fid_analyzer
        6. call terminus to decrypt
        '''

        # 0. generate key
        data_key_file = self.name + ".data.key.json"
        data_shukey_json = job_step.gen_key(self.crypto, data_key_file)
        self.all_outputs.append(data_key_file)

        # 1. generate key
        key_file = self.name + ".key.json"
        shukey_json = job_step.gen_key(self.crypto, key_file)
        self.all_outputs.append(key_file)

        # 2. call data provider to seal data
        sealed_data_url = self.name + ".sealed"
        sealed_output = self.name + ".sealed.output"
        summary = {}
        summary['data-url'] = self.data_url
        summary['plugin-path'] = self.plugin_url
        summary['sealed-data-url'] = sealed_data_url
        summary['sealed-output'] = sealed_output

        r = job_step.seal_data(self.crypto, self.data_url, self.plugin_url,
                               sealed_data_url, sealed_output, data_key_file)
        data_hash = job_step.read_data_hash(sealed_output)
        summary['data-hash'] = data_hash
        self.all_outputs.append(sealed_data_url)
        self.all_outputs.append(sealed_output)

        # use first pkey
        key = job_step.get_first_key(self.crypto)
        pkey = key['public-key']
        summary['tee-pkey'] = key['public-key']

        # 3. call terminus to generate forward message
        forward_result = self.name + ".shukey.foward.json"
        data_forward_json = job_step.forward_message(
            self.crypto, data_key_file, pkey, "", forward_result)
        enclave_hash = job_step.read_parser_hash(self.parser_url)
        self.all_outputs.append(forward_result)

        # 4.0 call terminus to generate forward message
        param_key_forward_result = self.name + ".request.shukey.foward.json"
        rq_forward_json = job_step.forward_message(
            self.crypto, key_file, pkey, enclave_hash, param_key_forward_result)
        self.all_outputs.append(param_key_forward_result)

        # 4.1 call terminus to generate request
        param_output_url = self.name + "_param.json"
        param_json = job_step.generate_request(
            self.crypto, self.input, "hex", key_file, param_output_url, self.config)
        summary['analyzer-input'] = param_json["encrypted-input"]
        self.all_outputs.append(param_output_url)

        # 5. call fid_analyzer
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
        input_data = [input_obj]
        parser_input_file = self.name + "parser_input.json"
        parser_output_file = self.name + "parser_output.json"
        result_json = job_step.fid_analyzer(shukey_json, rq_forward_json, enclave_hash, input_data,
                                            self.parser_url, pkey, {}, self.crypto, param_json, [], parser_input_file, parser_output_file)

        summary['encrypted-result'] = result_json["encrypted_result"]
        summary["result-signature"] = result_json["result_signature"]
        summary["cost-signature"] = result_json["cost_signature"]
        with open(self.name + ".summary.json", "w") as of:
            json.dump(summary, of)
        self.all_outputs.append(parser_input_file)
        self.all_outputs.append(parser_output_file)
        self.all_outputs.append(self.name + ".summary.json")
        self.all_outputs.append("info.json")

        # 6. call terminus to decrypt
        encrypted_result = summary["encrypted-result"]
        decrypted_result = self.name + ".result"
        self.result = job_step.decrypt_result(
            self.crypto, encrypted_result, key_file, decrypted_result)
        self.all_outputs.append(decrypted_result)
        job_step.remove_files(self.all_outputs)


if __name__ == "__main__":
    name = "tax"
    crypto = "stdeth"
    data = os.path.join(common.example_dir, "./dataset/税收.csv")
    parser = os.path.join(common.example_lib, "tax_parser.signed.so")
    plugin = os.path.join(common.example_lib, "libtax_reader.so")

    cj = classic_job(crypto, name, data, parser, plugin, '00aa')
    cj.run()
    print("result is : ", cj.result)
