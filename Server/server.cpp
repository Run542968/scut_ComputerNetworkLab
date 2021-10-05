#include<stdio.h>
#include<WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
using namespace std;
#pragma warning(disable : 4996)//搞掉讨厌的4996报错
int main() {
	//1.请求版本
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (HIBYTE(wsaData.wVersion) != 2 ||
		LOBYTE(wsaData.wVersion) != 2){
		printf("请求版本失败!\n");
		return -1;
}
	printf("请求版本成功!\n");

	//2.创建socket
	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket==INVALID_SOCKET) {
		printf("创建socket失败!\n");
		return -1;
	}
	printf("创建socket成功!\n");

	//3.创建协议地址族-区分ip以及端口
	SOCKADDR_IN addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//本地IP地址
	addr.sin_port = htons(9999);//端口号

	//4.绑定
	//                         地址族地址          长度
	int r = bind(serverSocket, (sockaddr*)&addr, sizeof addr);
	if (r == INVALID_SOCKET) {
		printf("绑定失败!\n");
		return -1;
	}
	printf("绑定成功!\n");
	
	//5.监听
	r = listen(serverSocket, 10);//监听数量10
	if (r == INVALID_SOCKET) {
		printf("监听失败!\n");
		return -1;
	}
	printf("监听成功!\n");

	//6.接受客户端连接
	SOCKET clientSocket = accept(serverSocket,0,0);//会返回一个client的socket符号
	if (clientSocket == SOCKET_ERROR){
		printf("客户端出错!\n");
		return -2;
		}
	printf("客户端连接服务器成功!\n");

	
#if 0	//7.传输文字信息通信
	char buff[1024];
	while (1) {//持续通信
		memset(buff, 0, 1024);
		r = recv(clientSocket, buff, 1023, NULL);//从客户端接收数据
		if (r > 0) {//接收到了就打印出来看一看
			printf(">>%s\n", buff);
		}
	}
#endif

	//7.文件传输通信
	char fileName[256] = { 0 };
	r = recv(clientSocket, fileName, 255, NULL);//接收文件名
	if (r > 0) {
		printf("接收文件名：%s\n", fileName);
	}

	   //7.1打开符合文件名的文件
	FILE* fp = fopen(fileName,"rb");//rb代表使用读byte的方式读文件

	   //7.2获取文件大小
	int size;
	fseek(fp, 0, SEEK_END);//把文件内容指针定位到文件末尾
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);//还原指针到文件头

	   //7.3服务器发送文件大小
	r = send(clientSocket, (char*)&size, 4, NULL);
	if (r > 0) {
		printf("文件大小发送成功!");
	}

	   //7.4循环的读取文件内容并发送
	char buff[1024];
	while (1) {
		memset(buff, 0, 1024);
		r = fread(buff, 1, 1024, fp);
		if (r > 0) {//如果读取到内容
			send(clientSocket, buff, r, NULL);
		}
		else {
			break;
		}
	}
	
	   //7.5关闭文件
	fclose(fp);

	while (1);//保持程序运行，窗口不会关闭
	return 0;
}