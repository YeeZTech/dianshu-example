#!/usr/bin/python3
import os
import sys
import subprocess
import json
import project

current_file = os.path.abspath(__file__)
integrate_dir = os.path.dirname(current_file)
example_dir = os.path.dirname(integrate_dir).replace(" ", "\ ")
example_bin = os.path.join(example_dir, "./bin")
example_lib = os.path.join(example_dir, "./lib")
data_dir = os.path.join(example_dir, "./dataset")

ypc_lib = os.path.join(project.ypc_lib_dir(), "./")
ypc_bin = os.path.join(ypc_lib, "../bin")

kmgr_enclave = {
    'stdeth': os.path.join(ypc_lib, "keymgr.signed.so"),
    'gmssl': os.path.join(ypc_lib, "keymgr_gmssl.signed.so"),
}


def execute_cmd(cmd):
    cmd = 'node ' + cmd
    print("execute_cmd: {}".format(cmd))
    p = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
    p.wait()
    if p.returncode != 0:
        raise RuntimeError('Failed to execute cmd {}'.format(cmd))

    return p.stdout.read().decode('utf-8')


def fid_terminus(**kwargs):
    cmd = os.path.join(integrate_dir, "./js/simjs.js")
    for k, v in kwargs.items():
        cmd = cmd + " --{} {}".format(k, v)
    output = execute_cmd(cmd)
    return [cmd, output]


def fid_data_provider(**kwargs):
    cmd = os.path.join(current_dir, "./js/simdataprovider.js")
    for k, v in kwargs.items():
        cmd = cmd + " --{} {}".format(k, v)
    output = execute_cmd(cmd)
    return [cmd, output]
