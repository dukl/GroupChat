/*
#include <iostream>  
#include <fstream>  
#include <WinSock2.h>  
#pragma comment(lib,"WS2_32.lib")  
using namespace std;  
  
#define PORT 21     //FTP端口  
#define IP_ADDR "192.168.1.101"   //主机地址  
  
int getPortNum(char* buf);  
bool executeFTPCmd(SOCKET controlSocket, char* buf, int len, int stateCode);  
int getStateCode(char* buf);  
  
int main()  
{  
    WSADATA dat;  
    SOCKET controlSocket, dataSocket;  
    SOCKADDR_IN serverAddr;  
    int dataPort, ret, stateCode;  
    char buf[100]={0}, sendBuf[1024]={0};  
      
    //初始化，很重要  
    if (WSAStartup(MAKEWORD(2,2),&dat)!=0)  //Windows Sockets Asynchronous启动  
    {  
        cout<<"Init Falied: "<<GetLastError()<<endl;  
        system("pause");  
        return -1;  
    }  
  
    //创建Socket  
    controlSocket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);  
    if(controlSocket==INVALID_SOCKET)  
    {  
        cout<<"Creating Control Socket Failed: "<<GetLastError()<<endl;  
        system("pause");  
        return -1;  
    }  
    //构建服务器访问参数结构体  
    serverAddr.sin_family=AF_INET;  
    serverAddr.sin_addr.S_un.S_addr=inet_addr(IP_ADDR); //地址  
    serverAddr.sin_port=htons(PORT);            //端口  
    memset(serverAddr.sin_zero,0,sizeof(serverAddr.sin_zero));  
    //连接  
    ret=connect(controlSocket,(struct sockaddr*)&serverAddr,sizeof(serverAddr));  
    if(ret==SOCKET_ERROR)  
    {  
        cout<<"Control Socket connecting Failed: "<<GetLastError()<<endl;  
        system("pause");  
        return -1;  
    }  
    cout<<"Control Socket connecting is success."<<endl;  
    //接收返回状态信息  
    recv(controlSocket,buf,100,0);  
    cout<<buf;                                                    //220  
    //根据返回信息提取状态码，进行判断  
    if(getStateCode(buf) != 220)  
    {  
        cout<<"Error: Control Socket connecting Failed"<<endl;  
        system("pause");  
        exit(-1);  
    }  
    //用户名  
    memset(buf,0,100);  
    sprintf(buf,"USER %s\r\n","xxxx");  
    executeFTPCmd(controlSocket, buf, 100, 331);                //331  
    //密码  
    memset(buf,0,100);  
    sprintf(buf,"PASS %s\r\n","xxxx");  
    executeFTPCmd(controlSocket, buf, 100, 230);            //230  
    //=======================================  
    //切换到被动模式  
    memset(buf,0,100);  
    sprintf(buf,"PASV\r\n");  
    executeFTPCmd(controlSocket, buf, 100, 227);                //227  
    //返回的信息格式为---h1,h2,h3,h4,p1,p2  
    //其中h1,h2,h3,h4为服务器的地址，p1*256+p2为数据端口  
    dataPort=getPortNum(buf);  
    //客户端数据传输socket  
    dataSocket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);  
    serverAddr.sin_port=htons(dataPort);    //更改连接参数中的port值  
    ret=connect(dataSocket,(struct sockaddr*)&serverAddr,sizeof(serverAddr));  
    if(ret==SOCKET_ERROR)  
    {  
        cout<<"Data Socket connecting Failed: "<<GetLastError()<<endl;  
        system("pause");  
        return -1;  
    }  
    cout<<"Data Socket connecting is success."<<endl;  
    //更改当前目录  
    memset(buf,0,100);  
    sprintf(buf,"CWD %s\r\n","/ftpxxxx/web/monitor");   //250  
    executeFTPCmd(controlSocket, buf, 100, 250);  
    //上传文件  
    memset(buf,0,100);  
    sprintf(buf,"STOR %s\r\n","1.jpg");  
    executeFTPCmd(controlSocket, buf, 100, 125);                        //125  
  
    FILE* f=fopen("1.jpg","rb");  
    if(f==NULL)  
    {  
        cout<<"The file pointer is NULL!"<<endl;  
        cout<<"Error: "<<__FILE__<<" "<<__LINE__<<endl;  
        exit(-1);  
    }  
    while( !feof(f) )  
    {  
        fread(sendBuf, 1, 1024, f);  
        send(dataSocket, sendBuf, 1024, 0);  
    }  
    fclose(f);  
      
    //释放资源  
    closesocket(dataSocket);  
    closesocket(controlSocket);  
    WSACleanup();  
    system("pause");  
    return 0;  
}  
  
//从返回信息“227 Entering Passive Mode (182,18,8,37,10,25).”中  
//获取数据端口  
int getPortNum(char* buf)  
{  
    int num1=0,num2=0;  
  
    char* p=buf;  
    int cnt=0;  
    while( 1 )  
    {  
        if(cnt == 4 && (*p) != ',')  
        {  
            num1 = 10*num1+(*p)-'0';  
        }  
        if(cnt == 5)  
        {  
            num2 = 10*num2+(*p)-'0';  
        }  
        if((*p) == ',')  
        {  
            cnt++;  
        }  
        p++;  
        if((*p) == ')')  
        {  
            break;  
        }  
    }  
    cout<<"The data port number is "<<num1*256+num2<<endl;  
    return num1*256+num2;  
}  
//通过控制socket执行FTP命令  
bool executeFTPCmd(SOCKET controlSocket, char* buf, int len, int stateCode)  
{  
    send(controlSocket, buf, len, 0);  
    memset(buf, 0, len);  
    recv(controlSocket, buf, 100, 0);  
    cout<<buf;  
    if(getStateCode(buf) == stateCode)  
    {  
        return true;  
    }  
    else  
    {  
        cout<<"The StateCode is Error!"<<endl;  
        return false;  
    }  
}  
//从返回信息中获取状态码  
int getStateCode(char* buf)  
{  
    int num=0;  
    char* p=buf;  
    while(p != NULL)  
    {  
        num=10*num+(*p)-'0';  
        p++;  
        if(*p==' ')  
        {  
            break;  
        }  
    }  
  
    return num;  
}  
*/