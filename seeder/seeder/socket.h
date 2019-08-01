
#include "config.h"
SOCKET start_socket() {

	FILE	*ptr;

	WSADATA wsaData;
	int iResult;

	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;

	struct addrinfo *result = NULL;
	struct addrinfo hints;

	int iSendResult;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		system("pause");
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;



	// Resolve the server address and port
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		system("pause");
		return 1;
	}

	// Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		system("pause");
		return 1;
	}

	// Setup the TCP listening socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		system("pause");
		return 1;
	}

	freeaddrinfo(result);
	printf("Socket opened. Listening peers....\n");
	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		system("pause");
		return 1;
	}

	// Accept a client socket
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		system("pause");
		return 1;
	}
	printf("%ld connected.\n", ListenSocket);
	return ClientSocket;
}
int receive_handshake(SOCKET ClientSocket){
	char recvbuf[1000];
	int iResult, recvbuflen = DEFAULT_BUFLEN;
	FILE *ptr;
		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
	ptr = fopen("handshake.txt", "w");
	fputs(recvbuf, ptr);
	fclose(ptr);
	return 1;
}
int handshake_response(SOCKET ClientSocket, char * meg){
	send(ClientSocket, meg, (int)strlen(meg) + 1, 0);
	if (strstr(meg, "2:idi2e") == NULL) {
		return 0;
	}
	return 1;
}
//int transfer(SOCKET ClientSocket,char *meg){
//	char meg[1000];
//	char recvbuf[1000];
//	int iResult, recvbuflen;
//	do {
//		printf("\n\t\t: ");
//		fflush(stdin); gets(meg);
//		if (strcmp(meg, "exit") == 0){ break; }
//		send(ClientSocket, meg, (int)strlen(meg) + 1, 0);
//		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
//		if (iResult > 0) {
//			printf("%s\n", recvbuf);
//		}
//		else {
//			continue;
//		}
//
//	} while (1);
//}

void SendFile(SOCKET Socket,char *filename){
	if (Socket == NULL){
		return;
	}

		
		// mo file de truyen di

		FILE* fp = fopen(filename, "rb");

		if (fp == NULL) {
			printf("Khong mo duoc file");
			Sleep(2000);
			return;
		}

		fseek(fp, 0, SEEK_END);

		// Do dai cua file 
		long FileSize = ftell(fp);
		char GotFileSize[1024];
		_itoa_s(FileSize, GotFileSize, 10);

		// Truyen do dai cua file 
		send(Socket, GotFileSize, 1024, 0);
		rewind(fp);


		long SizeCheck = 0;
		char* mfcc;


		// bat dau gui file
		mfcc = (char*)malloc(FileSize + 1);
		fread_s(mfcc, FileSize, sizeof(char), FileSize, fp);
		send(Socket, mfcc, FileSize, 0);

		fclose(fp);
		Sleep(500);
		free(mfcc);
	
	return;
}