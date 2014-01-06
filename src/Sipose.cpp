/*
author : aancw
Version : v1.2
OS : Windows 7 x64
*/
#include <winsock2.h>
#include <windows.h>
#include <string>
#include <iostream>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream> /* string stream*/
#include <fstream> /* untuk fopen,fwrite*/
#pragma comment(lib,"ws2_32.lib")

using namespace std;

void CONSOLE_Print(string message)
{
	cout << message<<endl;
	
	//Logging
	ofstream myfile;
	myfile.open ("Logging.txt",ios::app);
	if( !myfile.fail( ) )
	{
		myfile << message<<endl;
		myfile.close();
	}
	
}

void DEBUG_Print(string message)
{
	cout<<message<<endl;
}

string UTIL_ToString( unsigned int i )
{
	string result;
	stringstream SS;
	SS << i;
	SS >> result;
	return result;
}
int main (){

	WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
	{
		CONSOLE_Print( "WSAStartup failed.\n");
    }

	SOCKET Socket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

	//membuat pointer hostents
	struct hostent *alamat2;
	int Start,End;
	char alamat1[20];
	u_long iMode=1;
	int iResult;

	CONSOLE_Print("Sipose v1.2 by aancw\n");
	DEBUG_Print("Input Valid Host Address(ex: www.google.com or 127.0.0.1) : ");
	cin>>alamat1;
	DEBUG_Print("Enter Port Range...");
	DEBUG_Print("Start : ");
	cin>>Start;
	DEBUG_Print("End : ");
	cin>>End;
	system("CLS");//clear the screen
	CONSOLE_Print("Begin Port Scanning : "+string(alamat1)+"...\n");
	alamat2 = gethostbyname(alamat1);
	SOCKADDR_IN SockAddr;
	SockAddr.sin_family=AF_INET;
	SockAddr.sin_addr.s_addr = *((unsigned long*)alamat2->h_addr); 
	
	CONSOLE_Print("Port\tStatus");

	for (; Start<=End; Start++)
	{
		SockAddr.sin_port = htons(Start);
		if (connect(Socket, (LPSOCKADDR)&SockAddr, sizeof(SOCKADDR)) == 0)
		{
		 CONSOLE_Print(UTIL_ToString(Start)+"\tOpen");
		}else{
		 CONSOLE_Print(UTIL_ToString(Start)+"\tClosed");
		}
	}

	CONSOLE_Print("End Port Scanning...\n");
	 // If iMode!=0, non-blocking mode is enabled.
	// Mempercepat Port Scanning ketika port closed
	iResult = ioctlsocket(Socket,FIONBIO,&iMode);
	
	if (iResult != NO_ERROR)
	printf("ioctlsocket failed with error: %ld\n", iResult);

	closesocket(Socket);
    WSACleanup();
	system("pause");
	return 0;
}
