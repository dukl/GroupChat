/*
#include <stdio.h>    
#include <winsock2.h>    
#include <iostream>
#include<string>

using namespace std;
#pragma comment(lib,"ws2_32.lib") 
int main()
{
	WORD sockVersion = MAKEWORD(2,2);           //版本号
    WSADATA wsaData;    
    if(WSAStartup(sockVersion, &wsaData)!=0)    //加载成功 返回0
    {    
        return 0;    
    } 

	SOCKET UDPSocket=socket(AF_INET,SOCK_DGRAM,0);
	if(UDPSocket == INVALID_SOCKET)  
    {  
        printf("invalid socket!");  
        return 0;  
    }  
	sockaddr_in serAddr;  
    serAddr.sin_family = AF_INET;  
    serAddr.sin_port = htons(8888);  
    serAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); 
	int nAddrlen = sizeof(serAddr);
  
	string data;
	cin>>data;
	const char*sendData=data.c_str();
	sendto(UDPSocket, sendData, strlen(sendData), 0,(SOCKADDR*)&serAddr,sizeof(SOCKADDR));  

	char revData[256];
	while(true){
		int ret=recvfrom(UDPSocket,revData,sizeof(revData),0,(SOCKADDR*)&serAddr,&nAddrlen);
		if(ret > 0)    
        {    
            revData[ret] = 0x00;    
            printf(revData); 
			printf("\n");
        }
	}
	closesocket(UDPSocket);    
    WSACleanup();    
    return 0;    
}
*/