
#include <stdio.h>    
#include <winsock2.h>    
#include <iostream>
#include<string>
#include <vector>
using namespace std;
#pragma comment(lib,"ws2_32.lib") 

void LogAccept();
void MessageIn();
void MessageOut();

struct Info{
	string name;
	int port;
	string ip;
};
vector <struct Info> Infos;
vector <string> data;
SOCKET ControlSocket;
SOCKET DataSocketOut;
SOCKET DataSocketIn;
int flag=0;
int main()
{
	WORD sockVersion = MAKEWORD(2,2);           //版本号
    WSADATA wsaData;    
    if(WSAStartup(sockVersion, &wsaData)!=0)    //加载成功 返回0
    {    
        return 0;    
    } 
	ControlSocket=socket(AF_INET,SOCK_STREAM,0);
	DataSocketOut=socket(AF_INET,SOCK_DGRAM,0);
	DataSocketIn=socket(AF_INET,SOCK_STREAM,0);

	if(ControlSocket == INVALID_SOCKET)
	{
		printf("invalid socket!");
		return 0;
	}
	if(DataSocketOut == INVALID_SOCKET)
	{
		printf("invalid socket!");
		return 0;
	}
	if(DataSocketIn == INVALID_SOCKET)
	{
		printf("invalid socket!");
		return 0;
	}

	sockaddr_in control;    
    control.sin_family = AF_INET;    
    control.sin_port = htons(1803);    
    control.sin_addr.S_un.S_addr = INADDR_ANY;     
    if(bind(ControlSocket, (LPSOCKADDR)&control, sizeof(control)) == SOCKET_ERROR)    
    {    
        printf("bind error !");    
    }    
	if(listen(ControlSocket, 5) == SOCKET_ERROR)    
    {    
        printf("listen error !");    
        return 0;    
    }  

	sockaddr_in dataIn;    
    dataIn.sin_family = AF_INET;    
    dataIn.sin_port = htons(1318);    
    dataIn.sin_addr.S_un.S_addr = INADDR_ANY;     
    if(bind(DataSocketIn, (LPSOCKADDR)&dataIn, sizeof(dataIn)) == SOCKET_ERROR)    
    {    
        printf("bind error !");    
    }    
	if(listen(DataSocketIn, 5) == SOCKET_ERROR)    
    {    
        printf("listen error !");    
        return 0;    
    }

	sockaddr_in dataOut;    
    dataOut.sin_family = AF_INET;    
    dataOut.sin_port = htons(1319);    
    dataOut.sin_addr.S_un.S_addr = INADDR_ANY;     
    if(bind(DataSocketOut, (LPSOCKADDR)&dataOut, sizeof(dataOut)) == SOCKET_ERROR)    
    {    
        printf("bind error !");    
    } 
	/*
	if(listen(DataSocketOut, 5) == SOCKET_ERROR)    
    {    
        printf("listen error !");    
        return 0;    
    }
	*/
	
while(1){
	HANDLE ThreadLogin =CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)LogAccept, NULL, 0, NULL);
	HANDLE ThreadMessageIn =CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MessageIn, NULL, 0, NULL);
	HANDLE ThreadMessageOut =CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MessageOut, NULL, 0, NULL);
}

//	closesocket(serverSocket);    
    WSACleanup();    
    return 0;    
}
void LogAccept(){
	SOCKET clientSocket;
	sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);
	char revData[255];

	while(true)
	{
//		printf("等待连接...\n");    
        clientSocket = accept(ControlSocket, (SOCKADDR *)&remoteAddr, &nAddrlen);    
        if(clientSocket == INVALID_SOCKET)    
        {    
            printf("accept error !");    
            continue;    
        }    
        printf("接受到一个连接：%s \r\n", inet_ntoa(remoteAddr.sin_addr));    

		int ret=recv(clientSocket,revData,255,0);
		if(ret > 0)    
        {    
            revData[ret] = 0x00; 
			string s(&revData[0],&revData[strlen(revData)]);
			int pos=s.find_first_of("@");
			string login=s.substr(0,pos);
			string name=s.substr(pos+1,s.length()-pos);
			int pos2=name.find_first_of("@");
			string port=name.substr(0,pos2);
			string trueName=name.substr(pos2+1,name.length()-pos2);
//			cout<<s<<endl;
//			cout<<login<<endl;
//            cout<<port<<endl;
//			cout<<trueName<<endl;
			struct Info temp;
			if(login=="login"){
				temp.ip=inet_ntoa(remoteAddr.sin_addr);
			    sscanf(port.c_str(),"%d",&temp.port);
			    cout<<temp.port;
			    temp.name=name;
                send(clientSocket, "welcome!", strlen("welcome!"), 0); 
				Infos.push_back(temp);
			}
        }   

	}
}
void MessageIn(){
	SOCKET clientSocket;
	sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);
	char revData[255];

	while(true)
	{
//		printf("等待连接...\n");    
        clientSocket = accept(DataSocketIn, (SOCKADDR *)&remoteAddr, &nAddrlen);    
        if(clientSocket == INVALID_SOCKET)    
        {    
            printf("accept error !");    
            continue;    
        }    
//        printf("：%s \r\n", inet_ntoa(remoteAddr.sin_addr));    

		int ret=recv(clientSocket,revData,255,0);
		if(ret > 0)    
        {    
            revData[ret] = 0x00; 
			string s(&revData[0],&revData[strlen(revData)]);
			data.push_back(s);
			flag=1;
			send(clientSocket, "ok!", strlen("ok!"), 0); 
            printf(revData); 
			printf("\n");
        }   

	}
}
void MessageOut(){
	while(1){
	if(flag){
		for(int i=0;i<Infos.size();i++)
		{
			sockaddr_in temp;    
            temp.sin_family = AF_INET;    
			temp.sin_port = htons(Infos[i].port);    
			temp.sin_addr.S_un.S_addr = inet_addr((Infos[i].ip).c_str());  
			if(data.size()==0){
				cout<<"wrong!"<<endl;
			}
			cout<<data[data.size()-1]<<endl;
			string s=data[data.size()-1];

	        sendto(DataSocketOut, s.c_str(), strlen(s.c_str()) + 1, 0, (SOCKADDR*)&temp, sizeof(SOCKADDR));
		}
		data.pop_back();
		flag=0;
	}
	}
}