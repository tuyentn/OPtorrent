
#include "handle.h"
int main(){
	SOCKET skc = start_socket();			 //Khoi tao socket , skc = clientsocket
	SOCKET socket_3;
	receive_handshake(skc);		//Nhan thong diep handshake
	printf("\nBat tay thanh cong ");
	char namefile[100];							//Ten cua file
	strcpy(namefile,handle_handshake());
	printf("\nBat tay thanh cong\n %s\n", namefile);
	char pwg_response[200];

	// tra loi thong diep cua client chon cac option
	do {
		strcpy(pwg_response, hand_pwg_response());
		//(skc, pwg_response);
		printf("\nTra loi xong\n");
	} while (!handshake_response(skc, pwg_response));
	printf("\nTra loi xong\nSan Sang trao doi du lieu.\n");

	// bat dau nhan request tu client
	char pwg_request[1024];
	int iResult;
	iResult = recv(skc, pwg_request, sizeof(pwg_request), 0);
	if (iResult < 0) {
		printf("\n\tkhong nhan duoc ");
	}
	printf("\n%s\n", pwg_request);

	key * list_hand_shake;
	char id_pwg[10];
	char id_request[10];
	list_hand_shake = decode(pwg_request);
	strcpy(id_pwg, find_value(list_hand_shake, "id"));
	if (strcmp(id_pwg, "6") == 0) {
		strcpy(id_request, find_value(list_hand_shake, "index"));
	}
	/*int id_req = atoi(id_request);
	switch (id_req) {
	case 0: strcpy(namefile, "test1.txt");
	case 1: strcpy(namefile, "test2.txt");
	}*/


	
	SendFile(skc, namefile);
	


	closesocket(skc);
	WSACleanup();

	system("pause");
	return 0;
}
