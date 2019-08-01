


#ifndef _TOOLS_H_
#define _TOOLS_H_

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#define FILENAME "piece1.txt"
#define PEER_ID "-OP0001-seederid0001"

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


#define DEFAULT_PORT "27431"
#define DEFAULT_BUFLEN 512


#pragma comment (lib, "Ws2_32.lib")

HANDLE client;
HANDLE up_file;



//#include <stdio.h>  //Provides the core input and output capabilities of the C language.
//#include <stdlib.h> //For performing a variety of operations, including conversion, pseudo-random numbers, memory allocation, process control, environment, signalling, searching, and sorting.
//#define _WIN32_WINNT 0x0500 //constant for console resizing (redifinition)
//#include <time.h> //For converting between various time and date formats.
//#include <winsock2.h>   // contains functions for socket programmming
//#define DEFAULT_PORT 5000
//#define STRLEN 256
//#define NUM_CLIENTS 10
//#define MAX_SERVICES 10
//#define NUM_MSG 4  //number of fields in protocol
//
//
//
////keys define
//#define F1    59   //Client 1
//#define F2    60 //Client 2
//#define F3    61  //Client 3
//#define ESC    37  //Escape key
//#define ALT_A     30


// function

void main_menu();
void start_server();

#endif