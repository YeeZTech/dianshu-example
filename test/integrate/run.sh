#!/bin/bash
YPC_INSTALL_DIR=$1
if [ $# -ne 1  ]; then
  echo "input parameters should contain YPC_INSTALL_DIR!"
  exit
fi

cur_dir=`pwd`
example_dir=$cur_dir/../..
plugin_dir=$example_dir/toolkit/plugins
schema_dir=$plugin_dir/schema
export YPC_INSTALL_DIR=$YPC_INSTALL_DIR
export LD_LIBRARY_PATH=$YPC_INSTALL_DIR/lib:$LD_LIBRARY_PATH
integrate_dir=$example_dir/test/integrate

# 0. install js libs
cd $integrate_dir/js && npm install

# 1. gen all plugins
for entry in "$schema_dir"/*
do
  filename=$(basename -- "$entry")
  filename=(${filename//./ })
  echo ${filename[0]}
  echo $entry
  rm -rf $plugin_dir/${filename[0]}
  cd $plugin_dir && python3 $plugin_dir/plugen.py --name ${filename[0]} --target-dir $plugin_dir --config $entry --ypc-install-dir $YPC_INSTALL_DIR
done


# 2. run all integrates
cd $integrate_dir && python3 $integrate_dir/test_gdp.py --ypc-install-dir $YPC_INSTALL_DIR
