/*
#include <iostream>  
#include <fstream>  
#include <WinSock2.h>  
#pragma comment(lib,"WS2_32.lib")  
using namespace std;  
  
#define PORT 21     //FTP�˿�  
#define IP_ADDR "192.168.1.101"   //������ַ  
  
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
      
    //��ʼ��������Ҫ  
    if (WSAStartup(MAKEWORD(2,2),&dat)!=0)  //Windows Sockets Asynchronous����  
    {  
        cout<<"Init Falied: "<<GetLastError()<<endl;  
        system("pause");  
        return -1;  
    }  
  
    //����Socket  
    controlSocket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);  
    if(controlSocket==INVALID_SOCKET)  
    {  
        cout<<"Creating Control Socket Failed: "<<GetLastError()<<endl;  
        system("pause");  
        return -1;  
    }  
    //�������������ʲ����ṹ��  
    serverAddr.sin_family=AF_INET;  
    serverAddr.sin_addr.S_un.S_addr=inet_addr(IP_ADDR); //��ַ  
    serverAddr.sin_port=htons(PORT);            //�˿�  
    memset(serverAddr.sin_zero,0,sizeof(serverAddr.sin_zero));  
    //����  
    ret=connect(controlSocket,(struct sockaddr*)&serverAddr,sizeof(serverAddr));  
    if(ret==SOCKET_ERROR)  
    {  
        cout<<"Control Socket connecting Failed: "<<GetLastError()<<endl;  
        system("pause");  
        return -1;  
    }  
    cout<<"Control Socket connecting is success."<<endl;  
    //���շ���״̬��Ϣ  
    recv(controlSocket,buf,100,0);  
    cout<<buf;                                                    //220  
    //���ݷ�����Ϣ��ȡ״̬�룬�����ж�  
    if(getStateCode(buf) != 220)  
    {  
        cout<<"Error: Control Socket connecting Failed"<<endl;  
        system("pause");  
        exit(-1);  
    }  
    //�û���  
    memset(buf,0,100);  
    sprintf(buf,"USER %s\r\n","xxxx");  
    executeFTPCmd(controlSocket, buf, 100, 331);                //331  
    //����  
    memset(buf,0,100);  
    sprintf(buf,"PASS %s\r\n","xxxx");  
    executeFTPCmd(controlSocket, buf, 100, 230);            //230  
    //=======================================  
    //�л�������ģʽ  
    memset(buf,0,100);  
    sprintf(buf,"PASV\r\n");  
    executeFTPCmd(controlSocket, buf, 100, 227);                //227  
    //���ص���Ϣ��ʽΪ---h1,h2,h3,h4,p1,p2  
    //����h1,h2,h3,h4Ϊ�������ĵ�ַ��p1*256+p2Ϊ���ݶ˿�  
    dataPort=getPortNum(buf);  
    //�ͻ������ݴ���socket  
    dataSocket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);  
    serverAddr.sin_port=htons(dataPort);    //�������Ӳ����е�portֵ  
    ret=connect(dataSocket,(struct sockaddr*)&serverAddr,sizeof(serverAddr));  
    if(ret==SOCKET_ERROR)  
    {  
        cout<<"Data Socket connecting Failed: "<<GetLastError()<<endl;  
        system("pause");  
        return -1;  
    }  
    cout<<"Data Socket connecting is success."<<endl;  
    //���ĵ�ǰĿ¼  
    memset(buf,0,100);  
    sprintf(buf,"CWD %s\r\n","/ftpxxxx/web/monitor");   //250  
    executeFTPCmd(controlSocket, buf, 100, 250);  
    //�ϴ��ļ�  
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
      
    //�ͷ���Դ  
    closesocket(dataSocket);  
    closesocket(controlSocket);  
    WSACleanup();  
    system("pause");  
    return 0;  
}  
  
//�ӷ�����Ϣ��227 Entering Passive Mode (182,18,8,37,10,25).����  
//��ȡ���ݶ˿�  
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
//ͨ������socketִ��FTP����  
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
//�ӷ�����Ϣ�л�ȡ״̬��  
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