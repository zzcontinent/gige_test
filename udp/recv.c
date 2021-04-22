
//UDP 接收广播信息
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
	//判断命令行参数是否满足
	if (argc != 2) {
		printf("请传递一个端口号\n");
		return -1;
	}

	//将接收端口号并转换为int
	int port = atoi(argv[1]);
	/*
	   if( port<1025 || port>65535 )//0~1024一般给系统使用，一共可以分配到65535
	   {
	   printf("端口号范围应为1025~65535");
	   return -1;
	   }
	   */

	// 1.创建udp通信socket
	int udp_socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (udp_socket_fd < 0) {
		perror("creat socket fail\n");
		return -1;
	}

	//2.设置UDP的地址并绑定
	struct sockaddr_in local_addr = { 0 };
	local_addr.sin_family = AF_INET;	     //使用IPv4协议
	local_addr.sin_port = htons(port);	     //网络通信都使用大端格式
	local_addr.sin_addr.s_addr = INADDR_ANY; //inet_addr("0.0.0.0");;//Linux下，需要设为0.0.0.0才能接收到广播

	int ret = bind(udp_socket_fd, (struct sockaddr*)&local_addr, sizeof(local_addr));
	if (ret < 0) {
		perror("bind fail:");
		close(udp_socket_fd);
		return -1;
	} else {
		printf("recv ready!!!\n");
	}

	struct sockaddr_in src_addr = { 0 }; //用来存放对方(信息的发送方)的IP地址信息
	int len = sizeof(src_addr);		 //地址信息的大小
	char buf[80000] = { 0 };		 //消息缓冲区

	//3 循环接收客户发送过来的数据
	long total_kb = 0;
	while (1) {
		ret = recvfrom(udp_socket_fd, buf, sizeof(buf), 0, (struct sockaddr*)&src_addr, &len);
		if (ret == -1) {
			break;
		}
		total_kb += strlen(buf) / 1000;
		printf("\r[ip: %s, port: %d, len: %ld, total: %ld KB]", inet_ntoa(src_addr.sin_addr), ntohs(src_addr.sin_port), strlen(buf), total_kb); //打印消息发送方的ip与端口号
		if (strcmp(buf, "exit") == 0) {
			break; //收到"exit"退出循环
		}
		memset(buf, 0, sizeof(buf)); //清空存留消息
	}

	//4 关闭通信socket
	close(udp_socket_fd);
	return 0;
}
