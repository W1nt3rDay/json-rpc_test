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

#include <stdio.h>
#include <jsonrpc-c.h>			//JSON-RPC 库头文件
#include "rpc.h"				//本地头文件, 包含端口宏定义等
#include "cJSON.h"
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>			//for inet_aton()
#include <errno.h>
#include <stdlib.h>				//for strtoul

// 全局变量：定义 JSON-RPC 服务对象
static struct jrpc_server my_server;


/* 
 * name: say_hello
 * func: 实现JSON-RPC中的 "sayHello"方法
 * para: 
 *     ctx    ：RPC 上下文（未使用）
 *     params ：JSON 对象，包含字段 "name"
 *     id     ：调用 ID（未使用）
 * retV: 构造一个cJSON字符串"Hello, name"
 */
cJSON * say_hello(jrpc_context * ctx, cJSON * params, cJSON *id) {
    char buf[100];
    cJSON *name = cJSON_GetObjectItem(params, "name");
    sprintf(buf, "Hello, %s", name->valuestring);
    return cJSON_CreateString(buf);
}

/*
 * 函数名称：add
 * 函数功能：实现 JSON-RPC 中的 "add" 方法，返回两个整数的和
 * 参数同上
 * 返回值：整数相加结果
 */
cJSON * add(jrpc_context * ctx, cJSON * params, cJSON *id) {
    char buf[100];
    cJSON * a = cJSON_GetArrayItem(params,0);
    cJSON * b = cJSON_GetArrayItem(params,1);
    return cJSON_CreateNumber(a->valueint + b->valueint);
}

/* 
 * name: RPC_Server_Init
 * func: 初始化并运行JSON-RPC服务器，注册远程过程调用函数
 * retV: 0表示正常退出
 */
int RPC_Server_Init(void)
{
	int err;
	
	//初始化JSON-RPC服务器，监听PORT端口
	err = jrpc_server_init(&my_server, PORT);
	if (err)
	{
		printf("jrpc_server_init err: %d\n",err);
	}
	
	//注册远程调用函数 say_hello -> "sayHello"
	//注册远程调用函数 add -> "add"
	jrpc_register_procedure(&my_server, say_hello, "sayHello", NULL);
	jrpc_register_procedure(&my_server, add, "add", NULL);
	
	
	jrpc_server_run(&my_server);  //启用JSON-RPC服务器（阻塞）
	jrpc_server_destroy(&my_server);
	
	return 0;
}


/*
 * 函数名称：RPC_Client_Init
 * 函数功能：建立客户端 socket，并连接到 RPC 服务器
 * 返回值：socket 描述符，失败返回 -1
 */
int RPC_Client_Init(void)
{
	int iSocketClient;
	struct sockaddr_in tSocketServerAddr;
	int iRet;
	
	iSocketClient = socket(AF_INET, SOCK_STREAM, 0);
	
	tSocketServerAddr.sin_family = AF_INET;
	tSocketServerAddr.sin_port   = htons(PORT);
	inet_aton("127.0.0.1", &tSocketServerAddr.sin_addr);
	memset(tSocketServerAddr.sin_zero, 0, 8);
	
	iRet = connect( iSocketClient,
					(const struct sockaddr *)&tSocketServerAddr,
					sizeof(struct sockaddr) );
	if (-1 == iRet)
	{
		printf("connect error!\n");
		return -1;
	}
	return iSocketClient;
}

/*
 * 函数名称：rpc_hello
 * 函数功能：向服务器发送 "sayHello" 的 JSON-RPC 请求，并解析响应
 * 参数：
 *     iSocketClient：客户端 socket
 *     name         ：发送的名字
 * 返回值：0 表示成功，-1 表示失败
 */
int rpc_hello(int iSocketClient, char *name)
{
    char buf[300];
    int iLen;
    sprintf(buf, "{\"method\": \"sayHello\"," \
                   "\"params\":"              \
                   "{\"name\": \"%s\"}, \"id\": \"2\" }", name);                    
    iLen = send(iSocketClient, buf, strlen(buf), 0);
    if (iLen ==  strlen(buf))
    {
		// 等待响应，跳过空行
        while (1) 
        {
            iLen = read(iSocketClient, buf, sizeof(buf));
            buf[iLen] = 0;
            if (iLen == 1 && (buf[0] == '\r' || buf[0] == '\n'))
                continue;
            else
                break;
        } 
        
		//处理返回值
        if (iLen > 0)
        {
            cJSON *root = cJSON_Parse(buf);
            cJSON *result = cJSON_GetObjectItem(root, "result");
            if (result)
            {
                printf("%s\n", result->valuestring);
                cJSON_Delete(root);
                return 0;
            }
            else
            {
                cJSON_Delete(root);
                return -1;
            }
        }
        else
        {
            printf("read rpc reply err : %d\n", iLen);
            return -1;
        }
    }
    else
    {
        printf("send rpc request err : %d, %s\n", iLen, strerror(errno));
        return -1;
    }
}

/*
 * 函数名称：rpc_add
 * 函数功能：向服务器发送 "add" 方法 RPC 请求，获取结果并赋值给 sum
 * 参数：
 *     iSocketClient：客户端 socket
 *     a, b         ：两个相加的整数
 *     sum          ：返回的和（输出参数）
 * 返回值：0 表示成功，-1 表示失败
 */
int rpc_add(int iSocketClient, int a, int b, int *sum)
{
	char buf[100];
	int iLen;
	// 构造 JSON-RPC 请求
	sprintf(buf, "{ \"method\": \"add\", \
					\"params\": [%d,%d], \
					\"id\": \"2\" }",
					a, b);
	iLen = send(iSocketClient, buf, strlen(buf), 0);
	if (iLen == strlen(buf))
	{
		while(1)
		{
			iLen = read(iSocketClient, buf, sizeof(buf));
			buf[iLen] = 0;
			if (iLen == 1 && (buf[0] == '\r' || buf[0] == '\n'))
				continue;
			else
				break;
		}
		
		if (iLen > 0)
		{
			cJSON* root = cJSON_Parse(buf);
			cJSON* result = cJSON_GetObjectItem(root, "result");
			*sum = result->valueint;
			cJSON_Delete(root);
			return 0;
		}
		else
		{
			printf("read rpc reply err : %d\n", iLen);
            return -1;
		}
	}
	else
	{
		printf("send rpc request err : %d, %s\n", iLen, strerror(errno));
        return -1;
	}
		
	
}

/* 打印用法函数 */
static void print_usage(char* exec)
{
	printf("Usage:\n");
	printf("%s <server>\n", exec);
	printf("%s add <num1> <num2>\n",exec);
	printf("%s hello <name>\n", exec);
}

/* 
 * 主函数
 * 依据输入参数决定运行服务器还是客户端逻辑
 */
int main(int argc, char** argv)
{
/********** 检查输入格式 *********/
	if (argc < 2)
	{
		print_usage(argv[0]);
		return -1;
	}
	
/* 以"server"模式运行 */
	if (argv[1][0] == 's' || argv[1][0] == 'S')
	{
		RPC_Server_Init();  //启动服务器
	}
	else  //其他的输入默认进行客户端操作
	{
		int sum;
		int fd = RPC_Client_Init();  //初始化连接
		if (fd < 0)
		{
			printf("RPC_Client_Init err :%d\n", fd);
			return -1;
		}
		
		  //客户端向服务器请求执行add操作
		if (argc == 4 && !strcmp(argv[1], "add"))
		{
			int a = (int)strtoul(argv[2], NULL, 0);
			int b = (int)strtoul(argv[3], NULL, 0);
			int err = rpc_add(fd, a, b, &sum);  //构造RPC数据包
			if (!err)
			{
				printf("sum = %d\n", sum);
			}
			else
			{
				printf("rpc err : %d\n", err);
			}
		}
		  //客户端向服务器请求执行sayHello操作
		else if (argc == 3 && !strcmp(argv[1], "hello"))
		{
			int err = rpc_hello(fd, argv[2]);
			if (err)
			{
				printf("rpc err : %d\n", err);
			}
		}
	}
	return 0;
}