//UDP 发送广播信息
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

char buf[1024 * 1024] = { 0 }; //消息缓冲区
long send_cnt = 0;

int* thread_monitor(void* arg)
{
	pthread_t newTid;
	newTid = pthread_self();
	printf("tid:%ld\n", newTid);
	long last_send = send_cnt;
	for (;;) {
		sleep(1);
		printf("[total:%ld KB, speed: %ld KB/s]\n", send_cnt / 1000, (send_cnt - last_send) / 1000);
		last_send = send_cnt;
	}
}

int main(int argc, char* argv[])
{
	//判断命令行参数是否满足
	if (argc != 4) {
		printf("请传递广播ip，端口号, 是否循环发送\n");
		return -1;
	}

	//接收端口号并转换为int
	int port = atoi(argv[2]);
	int isloop = atoi(argv[3]);
	/*if (port < 1025 || port > 65535) //0~1024一般给系统使用，一共可以分配到65535
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

	// create thread_monitor
	pthread_t tid;
	int iret = pthread_create(&tid, NULL, (void*)thread_monitor, NULL);
	if (iret != 0) {
		printf("thread create failed!\n");
		exit(1);
	}

	printf("Please input broadcast msg:\n");
	scanf("%s", buf); //获取要发送的消息
	//4 发送数据
	while (1) {
		if (isloop == 0) {
			scanf("%s", buf); //获取要发送的消息
		}
		int ret = sendto(udp_socket_fd, buf, strlen(buf), 0, (struct sockaddr*)&bro_addr, sizeof(bro_addr));
		if (ret == -1) {
			printf("sendto error: %d: %s\n", ret, strerror(errno));
			continue;
		}
		send_cnt += ret;
		if (strcmp(buf, "exit") == 0) {
			break; //退出循环
		}
	}

	bzero(buf, sizeof(buf));
	//5.关闭网络通信
	close(udp_socket_fd);

	return 0;
}
