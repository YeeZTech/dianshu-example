import re
import common

with open('t_summary_org_info.sql', 'r') as f:
    lines = f.readlines()
# print(lines)

ll = list()
for idx, line in enumerate(lines):
    l = [m.start() for m in re.finditer('\'', line)]
    l = l[-8:]
    i = 0
    params = list()
    while i < len(l):
        param = line[l[i] + 1:l[i + 1]]
        output = "decrypt.output"
        common.execute_cmd(
            "$HOME/ypc_0.5.1/bin/yterminus --decrypt --use-privatekey-file CBD-mysql.key.json --use-param {} --output {}".format(param, output))
        with open(output, 'r') as f:
            decrypted_param = f.read()
        params.append(decrypted_param)
        i += 2
    ll.append(line[:l[0]])
    i = 0
    while i < len(l):
        ll[idx] += '\''
        ll[idx] += params[int(i / 2)]
        ll[idx] += '\''
        ll[idx] += ','
        i += 2
    ll[idx] = ll[idx][:-1]
    ll[idx] += ')'
for ele in ll:
    print(ele)
