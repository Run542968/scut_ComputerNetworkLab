#include<stdio.h>
#include<WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
using namespace std;
#pragma warning(disable : 4996)//��������4996����
int main() {
	//1.����汾
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (HIBYTE(wsaData.wVersion) != 2 ||
		LOBYTE(wsaData.wVersion) != 2){
		printf("����汾ʧ��!\n");
		return -1;
}
	printf("����汾�ɹ�!\n");

	//2.����socket
	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket==INVALID_SOCKET) {
		printf("����socketʧ��!\n");
		return -1;
	}
	printf("����socket�ɹ�!\n");

	//3.����Э���ַ��-����ip�Լ��˿�
	SOCKADDR_IN addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//����IP��ַ
	addr.sin_port = htons(9999);//�˿ں�

	//4.��
	//                         ��ַ���ַ          ����
	int r = bind(serverSocket, (sockaddr*)&addr, sizeof addr);
	if (r == INVALID_SOCKET) {
		printf("��ʧ��!\n");
		return -1;
	}
	printf("�󶨳ɹ�!\n");
	
	//5.����
	r = listen(serverSocket, 10);//��������10
	if (r == INVALID_SOCKET) {
		printf("����ʧ��!\n");
		return -1;
	}
	printf("�����ɹ�!\n");

	//6.���ܿͻ�������
	SOCKET clientSocket = accept(serverSocket,0,0);//�᷵��һ��client��socket����
	if (clientSocket == SOCKET_ERROR){
		printf("�ͻ��˳���!\n");
		return -2;
		}
	printf("�ͻ������ӷ������ɹ�!\n");

	
#if 0	//7.����������Ϣͨ��
	char buff[1024];
	while (1) {//����ͨ��
		memset(buff, 0, 1024);
		r = recv(clientSocket, buff, 1023, NULL);//�ӿͻ��˽�������
		if (r > 0) {//���յ��˾ʹ�ӡ������һ��
			printf(">>%s\n", buff);
		}
	}
#endif

	//7.�ļ�����ͨ��
	char fileName[256] = { 0 };
	r = recv(clientSocket, fileName, 255, NULL);//�����ļ���
	if (r > 0) {
		printf("�����ļ�����%s\n", fileName);
	}

	   //7.1�򿪷����ļ������ļ�
	FILE* fp = fopen(fileName,"rb");//rb����ʹ�ö�byte�ķ�ʽ���ļ�

	   //7.2��ȡ�ļ���С
	int size;
	fseek(fp, 0, SEEK_END);//���ļ�����ָ�붨λ���ļ�ĩβ
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);//��ԭָ�뵽�ļ�ͷ

	   //7.3�����������ļ���С
	r = send(clientSocket, (char*)&size, 4, NULL);
	if (r > 0) {
		printf("�ļ���С���ͳɹ�!");
	}

	   //7.4ѭ���Ķ�ȡ�ļ����ݲ�����
	char buff[1024];
	while (1) {
		memset(buff, 0, 1024);
		r = fread(buff, 1, 1024, fp);
		if (r > 0) {//�����ȡ������
			send(clientSocket, buff, r, NULL);
		}
		else {
			break;
		}
	}
	
	   //7.5�ر��ļ�
	fclose(fp);

	while (1);//���ֳ������У����ڲ���ر�
	return 0;
}