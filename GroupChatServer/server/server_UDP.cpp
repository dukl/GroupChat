/*
#include <stdio.h>    
#include <winsock2.h>    
#include <iostream>
#include<string>

using namespace std;
#pragma comment(lib,"ws2_32.lib") 
int main()
{
	WORD sockVersion = MAKEWORD(2,2);           //�汾��
    WSADATA wsaData;    
    if(WSAStartup(sockVersion, &wsaData)!=0)    //���سɹ� ����0
    {    
        return 0;    
    } 

	SOCKET serverSocket=socket(AF_INET,SOCK_DGRAM,0);
	if(serverSocket == INVALID_SOCKET)
	{
		printf("invalid socket!");
		return 0;
	}

	sockaddr_in sin;    
    sin.sin_family = AF_INET;    
    sin.sin_port = htons(8888);    
    sin.sin_addr.S_un.S_addr = INADDR_ANY;     
    if(bind(serverSocket, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)    //��socket��һ��ʵ�ʶ˿ڵ�Ӧ�ó����(��������ϲ�ݽ�����ʱ�����Ƕ˿ںţ�
    {    
        printf("bind error !");    
    }    

	sockaddr_in remoteAddr;

	int nAddrlen = sizeof(remoteAddr);
	char revData[255];

	while(true)
	{
		int ret=recvfrom(serverSocket,revData,255,0,(SOCKADDR*)&remoteAddr,&nAddrlen);
		if(ret > 0)    
        {    
            revData[ret] = 0x00;    
            printf(revData); 
			printf("\n");
			sendto(serverSocket, "hello",255,0,(SOCKADDR*)&remoteAddr,sizeof(SOCKADDR));  
        }


	}

	closesocket(serverSocket);    
    WSACleanup();    
    return 0;    
}
*/