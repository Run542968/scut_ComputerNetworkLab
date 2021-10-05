#include<stdio.h>
#include<WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable : 4996)
using namespace std;
int main() {
	//1.请求版本
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (HIBYTE(wsaData.wVersion) != 2 ||
		LOBYTE(wsaData.wVersion) != 2) {
		printf("请求版本失败!\n");
		return -1;
	}
	printf("请求版本成功!\n");

	//2.创建socket
	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == INVALID_SOCKET) {
		printf("创建socket失败!\n");
		return -1;
	}
	printf("创建socket成功!\n");

	//3.创建协议地址族-区分ip以及端口
	SOCKADDR_IN addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//本地IP地址
	addr.sin_port = htons(9999);//端口号

	//4.连接服务器
	int r = connect(serverSocket, (sockaddr*)&addr, sizeof addr);
	if (r == INVALID_SOCKET) {
		printf("连接服务器失败!\n");
		return -2;
	}
	printf("连接服务器成功!\n");

#if 0	//5.传输简单文字信息通信

	char buff[1024];
	while (1) {
		printf("请输入：");
		scanf("%s", buff);
		send(serverSocket, buff, strlen(buff), NULL);
	}
#endif

	//5.文件传输通信
	char fileName[256] = { 0 };
	printf("请输入文件名：");
	scanf("%s", fileName);
	   //5.1发送文件名
	r = send(serverSocket, fileName, strlen(fileName), NULL);
	if (r > 0) {
		printf("文件名发送成功!\n");
	}
	   //5.2接收文件大小
	int size = 0;
	r = recv(serverSocket, (char*)&size, 4, NULL);
	if (r > 0) {
		printf("接收到文件大小：%d字节\n", size);
	}

	   //5.3先创建一个文件并打开
	FILE* fp = fopen(fileName, "wb");

	   //5.4循环的接收文件内容
	char buff[1024];
	int count = 0;//统计文件结束了多少，判断是否接收完毕
	while (1) {
		memset(buff, 0, 1024);
		r = recv(serverSocket, buff, 1024, NULL);
		if (r > 0) {
			count += r;
			fwrite(buff, 1, r, fp);//把接受到的数据写入文件
			if (count >= size) {
				printf("文件接收完毕!\n");
				break;
			}
		}
	}

	   //5.4关闭文件
	   fclose(fp);

	   while (1);//保持程序运行，窗口不会关闭
	   return 0;
}