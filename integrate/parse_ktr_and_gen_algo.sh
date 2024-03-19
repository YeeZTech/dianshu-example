cd ../example/CBD-mysql/py && python3 xml_parser.py
export LD_LIBRARY_PATH=$HOME/ypc_0.5.1/lib:$LD_LIBRARY_PATH
cd $HOME/dianshu-example/build
make clean && make -j
