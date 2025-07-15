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
$ tar xjf libev_pc.tar.bz2 <br>
$ cd libev_pc/ <br>
$ CC=gcc ./configure --prefix=$PWD/tmp <br>
$ make -j 16 <br>
$ make install <br>
$ ls tmp/ <br>
include lib share <br>


2. 编译 jsonrpc 库 <br>
操作命令如下： <br>
$ sudo apt install libtool <br>
$ tar xjf jsonrpc-c_pc.tar.bz2 <br>
$ cd jsonrpc-c_pc/ <br>
$ autoreconf -i <br>
$ CC=gcc ./configure --prefix=$PWD/tmp <br>
$ make -j 16 <br>
$ make install <br>
$ ls tmp/ <br>
include lib <br>


3. 编译测试程序 <br>
操作命令如下： <br>
$ cd json-rpc_test/ <br>
$ make <br>
<br>
example:
<img width="754" height="266" alt="Screenshot 2025-07-15 212120" src="https://github.com/user-attachments/assets/f86634b5-210b-40a2-b717-fe5871855477" />

