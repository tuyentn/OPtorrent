#include "config.h"

void response_peer(char * send_response, SOCKET ClientSocket){
	int iSendResult,iResult;
	char sendbuf[100];
	strcpy(sendbuf, send_response);
	iResult = strlen(sendbuf) + 1;
	Sleep(1000);
	iSendResult = send(ClientSocket, sendbuf, iResult, 0);
	printf("\n\n   SENT RESPONSE.\n\n\n\n\n\t\t");

}