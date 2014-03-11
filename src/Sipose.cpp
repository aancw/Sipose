/*
	
	Author : aancw < cacaddv[at]gmail[dot]com >
	Created on : 11 Mar 2014
	Tested OS : Windows 7 x64,Linux debian Wheezy(7.0 - 7.4),Slackware 14.1,Fedora
	Compiler : g++/gcc (Debian 4.7.2-5) 4.7.2
	Version : v2.1

	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

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
#include <ctime>

using namespace std;

void CONSOLE_Print(string message)
{
	cout << message <<endl;
	
	// Logging cout to a file
	ofstream myfile;
	myfile.open ("Sipose.log",ios::app);
	if( !myfile.fail( ) )
	{
		myfile << message <<endl;
		myfile.close();
	}
	
}

void DEBUG_Print(string message)
{
	// It's just cout 
	cout << message <<endl;
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
string dateTime()
{
	//----------------------------
	// Untuk check waktu expired dan delete admin berkala by aan
	
	//-------------------
	//current date/time based on current system
	time_t now	= time(0);
	tm *ltm		= localtime(&now);
	
	//-------------------
   // Pisahkan satu persatu date dan time untuk mempermudah manipulasi data
   // Localtime sudah dirubah menjadi WIB atau GMT + 7 
   int Hari		= ltm->tm_mday;
   int Bulan	= 1 + ltm->tm_mon;
   int Tahun	= 1900 + ltm->tm_year;
   int Jam		= ltm->tm_hour;
   int Menit	= ltm->tm_min ;
   int Detik	= ltm->tm_sec;
   
	//---------------
	// You Need to convert from Const Char* To String :D
	string HariStr	= UTIL_ToString(Hari);
	string BulanStr = UTIL_ToString(Bulan);
	string TahunStr	= UTIL_ToString(Tahun);
	string JamStr	= UTIL_ToString(Jam);
	string MenitStr = UTIL_ToString(Menit);
	string DetikStr	= UTIL_ToString(Detik);

	string Waktu = "["+HariStr+"/"+BulanStr+"/"+TahunStr+" "+JamStr+":"+MenitStr+":"+DetikStr+"]  ";
	return Waktu;
}
int scanPortWithSingle(char* alamat1,int SinglePort)
{
	//-------------------
	// Inisialisasi Winsock
	#ifdef _WIN32
		WSADATA wsaData;
		int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
		if (iResult != NO_ERROR)
		{
			CONSOLE_Print( dateTime() + "[ERROR] " + "WSAStartup failed with error : "+UTIL_ToString(iResult));
			return 1;
		}
	#endif

	//--------------------
	// Membuat Socket untuk koneksi ke server
	int Socket;
	Socket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
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
	hostent* alamat2;
	alamat2 = gethostbyname(alamat1);
	sockaddr_in SockAddr;
	SockAddr.sin_family=AF_INET;
	SockAddr.sin_addr.s_addr = *((unsigned long*)alamat2->h_addr); 
	SockAddr.sin_port = htons(SinglePort);

	if (connect(Socket, (sockaddr*)(&SockAddr), sizeof(SockAddr)) == 0)
	{
		CONSOLE_Print(dateTime() + "[PROCCESS] " + UTIL_ToString(SinglePort)+"\tOpen");
	}
	else
	CONSOLE_Print(dateTime() + "[PROCCESS] " + UTIL_ToString(SinglePort)+"\tClosed");

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
int scanPortWithRange(int Start,int End,char* alamat1)
{
	//-------------------
	// Inisialisasi Winsock
	#ifdef _WIN32
		WSADATA wsaData;
		int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
		if (iResult != NO_ERROR)
		{
			CONSOLE_Print( dateTime() + "[ERROR] " + "WSAStartup failed with error : "+UTIL_ToString(iResult));
			return 1;
		}
	#endif

	for (Start; Start<=End; Start++)
	{
	
		//--------------------
		// Membuat Socket untuk koneksi ke server
		int Socket;
		Socket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
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
		hostent* alamat2;
		alamat2 = gethostbyname(alamat1);
		sockaddr_in SockAddr;
		SockAddr.sin_family=AF_INET;
		SockAddr.sin_addr.s_addr = *((unsigned long*)alamat2->h_addr); 
		SockAddr.sin_port = htons(Start);

		if (connect(Socket, (sockaddr*)(&SockAddr), sizeof(SockAddr)) == 0)
		{
			//CONSOLE_Print(UTIL_ToString(Start)+"\tclose :");
			CONSOLE_Print(dateTime() + "[PROCCESS] " + UTIL_ToString(Start)+"\tOpen");
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
}
int main (){
	//---------------------
	// Deklarasi Variabel author,version,appName
	string author	= "aancw";
	string appName	= "Sipose";
	string version	= "v2.1";

	//---------------------
	// Variable untuk Ip Address dan port yang akan dikoneksikan ke server
	int	Start,End,Pilihan,SinglePort;
	char alamat1[20];
	u_long iMode=0;
	
	CONSOLE_Print(dateTime() + "[INFO] " + "Starting Sipose Application");
	CONSOLE_Print(dateTime() + "[INFO] " +  appName+" "+version+" by "+author);
	DEBUG_Print("\nPort Scanning Mode ");
	DEBUG_Print("1. Single Port Scanning Mode");
	DEBUG_Print("2. Range Port Scanning Mode");
	DEBUG_Print("\nPlease Select what you want : ");
	cin>>Pilihan;

	if( Pilihan == 1 )
	{
		DEBUG_Print("Enter Valid Host Address(ex: localhost or 127.0.0.1) : ");
		cin>>alamat1;
		DEBUG_Print("Enter the ports you want to scan : ");
		cin>>SinglePort;
	}
	else if (Pilihan == 2)
	{
		
		DEBUG_Print("Input Valid Host Address(ex: localhost or 127.0.0.1) : ");
		cin>>alamat1;
		DEBUG_Print("Enter Port Range ");
		DEBUG_Print("Start : ");
		cin>>Start;
		DEBUG_Print("End : \n");
		cin>>End;
		CONSOLE_Print(dateTime() + "[INFO] Scanning speed depends on the speed of the internet and the amount of range that you enter!");

		#ifdef _WIN32
		system("CLS");//clear the screen
		#elif __linux__
		printf("\033[H\033[J"); // Linux Clear Screen
		#endif
	}
	else
		CONSOLE_Print("Untuk saat ini hanya ada dua pilihan Mode Scanning ");


	CONSOLE_Print(dateTime() + "[INFO] Please be patient if you use Range Port Scanning Mode ");
	CONSOLE_Print(dateTime() + "[PROCCESS] " + "Begin Port Scanning on "+string(alamat1)+"\n");
	CONSOLE_Print(dateTime() + "[PROCCESS] "  + "Port\tStatus");
	
	if( Pilihan == 1)
	{
		
		scanPortWithSingle(alamat1,SinglePort);
	}
	else if( Pilihan == 2)
	{
		// Scanning Proccess for Range Scanning Mode
		scanPortWithRange(Start,End,alamat1);
	}

	CONSOLE_Print(dateTime() + "[INFO] " + "End Port Scanning on "+string(alamat1)+"\n");

#ifdef _WIN32
	system("pause");
#endif
	return 0;
}
