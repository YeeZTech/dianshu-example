#!/bin/bash
YPC_INSTALL_DIR=$1
if [ $# -ne 1  ]; then
  echo "input parameters should contain YPC_INSTALL_DIR!"
  exit
fi

# gen all plugins
cur_dir=`pwd`
example_dir=$cur_dir
