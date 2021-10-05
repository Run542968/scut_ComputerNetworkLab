#include<stdio.h>
#include<WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable : 4996)
using namespace std;
int main() {
	//1.����汾
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (HIBYTE(wsaData.wVersion) != 2 ||
		LOBYTE(wsaData.wVersion) != 2) {
		printf("����汾ʧ��!\n");
		return -1;
	}
	printf("����汾�ɹ�!\n");

	//2.����socket
	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == INVALID_SOCKET) {
		printf("����socketʧ��!\n");
		return -1;
	}
	printf("����socket�ɹ�!\n");

	//3.����Э���ַ��-����ip�Լ��˿�
	SOCKADDR_IN addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//����IP��ַ
	addr.sin_port = htons(9999);//�˿ں�

	//4.���ӷ�����
	int r = connect(serverSocket, (sockaddr*)&addr, sizeof addr);
	if (r == INVALID_SOCKET) {
		printf("���ӷ�����ʧ��!\n");
		return -2;
	}
	printf("���ӷ������ɹ�!\n");

#if 0	//5.�����������Ϣͨ��

	char buff[1024];
	while (1) {
		printf("�����룺");
		scanf("%s", buff);
		send(serverSocket, buff, strlen(buff), NULL);
	}
#endif

	//5.�ļ�����ͨ��
	char fileName[256] = { 0 };
	printf("�������ļ�����");
	scanf("%s", fileName);
	   //5.1�����ļ���
	r = send(serverSocket, fileName, strlen(fileName), NULL);
	if (r > 0) {
		printf("�ļ������ͳɹ�!\n");
	}
	   //5.2�����ļ���С
	int size = 0;
	r = recv(serverSocket, (char*)&size, 4, NULL);
	if (r > 0) {
		printf("���յ��ļ���С��%d�ֽ�\n", size);
	}

	   //5.3�ȴ���һ���ļ�����
	FILE* fp = fopen(fileName, "wb");

	   //5.4ѭ���Ľ����ļ�����
	char buff[1024];
	int count = 0;//ͳ���ļ������˶��٣��ж��Ƿ�������
	while (1) {
		memset(buff, 0, 1024);
		r = recv(serverSocket, buff, 1024, NULL);
		if (r > 0) {
			count += r;
			fwrite(buff, 1, r, fp);//�ѽ��ܵ�������д���ļ�
			if (count >= size) {
				printf("�ļ��������!\n");
				break;
			}
		}
	}

	   //5.4�ر��ļ�
	   fclose(fp);

	   while (1);//���ֳ������У����ڲ���ر�
	   return 0;
}