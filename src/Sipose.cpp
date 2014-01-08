/*
author : aancw
Created on : 06 Jan 2014
OS : Windows 7 x64,Linux debian Wheezy
Compiler : g++ (Debian 4.7.2-5) 4.7.2
Version : v1.3

*/

#ifdef __linux__
    // linux Include File
	#include <arpa/inet.h>
	#include <assert.h>
	#include <errno.h>
	#include <netinet/in.h>
	#include <iostream>
	#include <signal.h>
	#include <stdlib.h>
	#include <stdio.h>
	#include <string.h>
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <sys/wait.h>
	#include <netdb.h>
	#include <unistd.h>
	#include <string>
#elif _WIN32
    // windows Include File
	#include <winsock2.h>
	#include <windows.h>
	#include <string>
	#include <iostream>
	#include <conio.h>
	#include <stdio.h>
	#include <stdlib.h>
	#pragma comment(lib,"ws2_32.lib")
#else 
    cerr << "OS not supported" << endl;
#endif

// Include File untuk semua OS
#include <sstream> /* untuk string stream*/
#include <fstream> /* untuk fopen,fwrite*/
using namespace std;

void CONSOLE_Print(string message)
{
	cout << message<<endl;
	
	// Logging cout to a file
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
	// It's just cout 
	cout<<message<<endl;
}

string UTIL_ToString( unsigned int i )
{
	// Convert unsigned integer to String :D
	string result;
	stringstream SS;
	SS << i;
	SS >> result;
	return result;
}
int main (){
	//---------------------
	// Deklarasi Variabel author,version,appName
	string author = "aancw";
	string appName = "Sipose";
	string version = "v1.3";

	//---------------------
	// Variable untuk Ip Address dan port yang akan dikoneksikan ke server
	struct hostent *alamat2;
	int Start,End;
	char alamat1[20];
	u_long iMode=0;
	
	//-------------------
	// Inisialisasi Winsock
	#ifdef _WIN32
		WSADATA wsaData;
		int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
		if (iResult != NO_ERROR)
		{
			CONSOLE_Print( "WSAStartup failed with error : "+UTIL_ToString(iResult));
			return 1;
		}
	#endif

	CONSOLE_Print(appName+" "+version+" by "+author);
	DEBUG_Print("Input Valid Host Address(ex: localhost or 127.0.0.1) : ");
	cin>>alamat1;
	DEBUG_Print("Enter Port Range...");
	DEBUG_Print("Start : ");
	cin>>Start;
	DEBUG_Print("End : ");
	cin>>End;
#ifdef _WIN32
	system("CLS");//clear the screen
#elif __linux__
	printf("\033[H\033[J");
#endif
	CONSOLE_Print("Begin Port Scanning : "+string(alamat1)+"...\n");
	alamat2 = gethostbyname(alamat1);

	CONSOLE_Print("Port\tStatus");

	for (Start; Start<=End; Start++)
	{
	
		//--------------------
		// Membuat Socket untuk koneksi ke server
		int Socket;
		Socket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		#ifdef _WIN32
				if(Socket == INVALID_SOCKET)
				{
					CONSOLE_Print("Socket function failed with error : "+WSAGetLastError());
					WSACleanup();
					return 1;
				}
		#endif

		//-------------------
		// Untuk Sockaddr seperti ip address dan port yang akan terkoneksi
		sockaddr_in SockAddr;
		SockAddr.sin_family=AF_INET;
		SockAddr.sin_addr.s_addr = *((unsigned long*)alamat2->h_addr); 
		SockAddr.sin_port = htons(Start);

		if (connect(Socket, (sockaddr*)(&SockAddr), sizeof(SockAddr)) == 0)
		{
			//CONSOLE_Print(UTIL_ToString(Start)+"\tclose :");
			CONSOLE_Print(UTIL_ToString(Start)+"\tOpen");
		}

		#ifdef _WIN32
			iResult = closesocket(Socket);
			if(iResult == SOCKET_ERROR)
			{
				CONSOLE_Print("closesocket function failed with error"+WSAGetLastError());
				WSACleanup();
				return 1;
			}
		#elif __linux__
			close(Socket);
		#endif
	}
	CONSOLE_Print("End Port Scanning...\n");

	
	return 0;
}
