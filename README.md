# json-rpc_test
/**
 * @file rpc.c
 * @brief 基于 JSON-RPC 的 C 客户端/服务器通信示例
 *
 * 支持两个远程过程调用：
 * - sayHello(name): 返回 Hello + name 的字符串
 * - add(a, b): 返回两个整数的和
 *
 * 使用 jsonrpc-c 和 cJSON 库，支持客户端/服务端通信。
 */

Usage:
在 Ubuntu 中做实验。先编译 libev 库，再编译 jsonrpc 库，最后编译、执行测试
程序 json-rpc_test。
假设这 3 个文件放在同一个目录下：
$ ls
jsonrpc-c_pc.tar.bz2 json-rpc_test libev_pc.tar.bz2
<br>
1. 编译 libev 库
操作命令如下：
$ tar xjf libev_pc.tar.bz2
$ cd libev_pc/
$ CC=gcc ./configure --prefix=$PWD/tmp
$ make -j 16
$ make install
$ ls tmp/
include lib share
$ sudo cp -rf tmp/include/* /usr/include/
$ sudo cp -rfd tmp/lib/* /usr/lib

2. 编译 jsonrpc 库
操作命令如下：
$ sudo apt install libtool
$ tar xjf jsonrpc-c_pc.tar.bz2
$ cd jsonrpc-c_pc/
$ autoreconf -i
$ CC=gcc ./configure --prefix=$PWD/tmp
$ make -j 16
$ make install
$ ls tmp/
include lib
$ sudo cp -rf tmp/include/* /usr/include/
$ sudo cp -rfd tmp/lib/* /usr/lib

3. 编译测试程序
操作命令如下：
$ cd json-rpc_test/
$ make

example:
<img width="754" height="266" alt="Screenshot 2025-07-15 212120" src="https://github.com/user-attachments/assets/f86634b5-210b-40a2-b717-fe5871855477" />

