


// declare library variable function
#ifndef _TOOLS_H_
#define _TOOLS_H_

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif


#define _CRT_SECURE_NO_DEPRECATE

#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#define FILENAME "piece1.txt"
#define PEER_ID "-OP0001-clientid0001"

#include <Windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> //Contains functions used to classify characters by their types or to convert between upper and lower case in a way that is independent of the used character set
#include <windows.h> //defines a very large number of Windows specific functions that can be used in C.
#include <string.h> //For manipulating several kinds of strings.
#include <process.h>
#include <conio.h>  //for doing console input output
#include <thread>


#define DEFAULT_PORT "27001"
#define DEFAULT_BUFLEN 512


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

int iResult;
char info_hash[100];
char file_name[] = "anh-1.jpg";
SOCKET socket_1;
SOCKET socket_2;
SOCKET socket_3;
HANDLE tracker;
HANDLE seeder;
HANDLE dow_from_seeder;

DWORD id;

//char *  tracker_ip;
//char * ip_client;

#endif

