//UDP 发送广播信息
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

int main(int argc, char* argv[])
{
	//判断命令行参数是否满足
	if (argc != 3) {
		printf("请传递广播ip，端口号\n");
		return -1;
	}

	//接收端口号并转换为int
	int port = atoi(argv[2]);
	/*
	if (port < 1025 || port > 65535) //0~1024一般给系统使用，一共可以分配到65535
	{
		printf("端口号范围应为1025~65535");
		return -1;
	}
	*/

	//1.创建UDP  socket
	int udp_socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (udp_socket_fd == -1) {
		printf("create socket failed ! error message :%s\n", strerror(errno));
		return -1;
	}

	//2.开启发送广播数据功能
	int on = 1; //开启
	int ret = setsockopt(udp_socket_fd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));
	if (ret < 0) {
		perror("setsockopt fail\n");
		return -1;
	}

	//3.设置当前网段的广播地址
	struct sockaddr_in bro_addr = { 0 };
	bro_addr.sin_family = AF_INET;
	bro_addr.sin_port = htons(port);
	bro_addr.sin_addr.s_addr = inet_addr(argv[1]); //设置为广播地址

	char * buf = malloc(1024000); //消息缓冲区

	//4 发送数据
	//printf("Please input broadcast msg:");
	scanf("%s", buf); //获取要发送的消息
	printf("send:%d\n",strlen(buf));
	while(1)
	{
		sendto(udp_socket_fd, buf, strlen(buf), 0, (struct sockaddr*)&bro_addr, sizeof(bro_addr));
	}
	bzero(buf, sizeof(buf));
	//5.关闭网络通信
	close(udp_socket_fd);

	return 0;
}
