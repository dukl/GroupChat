
#include <stdio.h>    
#include <winsock2.h>    
#include <iostream>
#include<string>

using namespace std;
#pragma comment(lib,"ws2_32.lib") 

#define ServerIP "127.0.0.1"
void MessageOut();
void MessageIn();
int Login();

sockaddr_in control; 
sockaddr_in data_Out;
sockaddr_in data_In;

SOCKET MageIn;

int main()
{
	WORD sockVersion = MAKEWORD(2,2);           //版本号
    WSADATA wsaData;    
    if(WSAStartup(sockVersion, &wsaData)!=0)    //加载成功 返回0
    {    
        return 0;    
    } 

	MageIn=socket(AF_INET,SOCK_DGRAM,0);

    control.sin_family = AF_INET;    
    control.sin_port = htons(1803);    
    control.sin_addr.S_un.S_addr = inet_addr(ServerIP);

    
    data_Out.sin_family = AF_INET;    
    data_Out.sin_port = htons(1318);    
    data_Out.sin_addr.S_un.S_addr = inet_addr(ServerIP);  

	data_In.sin_family = AF_INET;    
    data_In.sin_port = htons(1319);    
    data_In.sin_addr.S_un.S_addr = inet_addr(ServerIP);

	if(Login()<0)
		return 0;
	MessageOut();
	while(1){
//	HANDLE LogThread =CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Login, NULL, 0, NULL);
//	HANDLE SendThread =CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MessageOut, NULL, 0, NULL);
	HANDLE RecvThread =CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MessageIn, NULL, 0, NULL);
    }
	   
    WSACleanup(); 
	system("pause");
    return 0;    
}

void MessageOut(){
while(1){
    SOCKET SendSocket=socket(AF_INET,SOCK_STREAM,0);
	if(SendSocket == INVALID_SOCKET)  
    {  
        printf("invalid socket!");  
    //    break;  
    }  
	if(connect(SendSocket, (sockaddr *)&data_Out, sizeof(data_Out)) == SOCKET_ERROR)  
    {   
        printf("connect error !");  
        closesocket(SendSocket); 
     //   break;  
    }  
	string Message;
	cin>>Message;
	const char*sendData=Message.c_str();
	send(SendSocket, sendData, strlen(sendData), 0); 
	char revData[255];
	int ret=recv(SendSocket,revData,255,0);
	closesocket(SendSocket); 
	}
}

void MessageIn(){
	sockaddr_in addrServer;
	char response[20];
	int len = sizeof(SOCKADDR);
	int result = recvfrom(MageIn, response, 200, 0, (SOCKADDR*)&addrServer, &len);
	printf(response);
}

int Login(){
    SOCKET LoginSocket=socket(AF_INET,SOCK_STREAM,0);
	if(LoginSocket == INVALID_SOCKET){  
        printf("invalid socket!");  
        return -1;  
    }  
	if((connect(LoginSocket, (sockaddr *)&control, sizeof(control))) != SOCKET_ERROR){
		string name,tmp;
		cout<<"please input your account!"<<endl;
		cin>>name;
		tmp="login@"+name;
		send(LoginSocket,tmp.c_str(), strlen(tmp.c_str()), 0);  
	}else{
		printf("connect error !\n");  
        closesocket(LoginSocket); 
        return -1; 
	}
	char revData[255];
	int ret=recv(LoginSocket,revData,255,0);
	if(ret > 0)    
     {    
        revData[ret] = 0x00;    
        printf(revData); 
	    printf("\n");
     } 
	closesocket(LoginSocket);
	return 0;
}