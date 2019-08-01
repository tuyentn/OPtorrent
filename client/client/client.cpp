
char  ip[36];

#include "client.h"

int main()
{

	tracker = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)vs_tracker, NULL, 0, &id);
	seeder = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)vs_seeder, NULL, 0, &id);
	dow_from_seeder = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)dow_seeder, NULL, 0, &id);
	
	
	/****************************/


	//lay ip va thong tin file metainfo

	strcpy(ip, getip());
	printf("\nip = %s\n", ip); 

	get_metainfo(); // mo luon socket trong nay  //Sửa lại phần nhập file torrent cần xử lí

	//gui thong diep den tracker
	send_request();
	/*******************************/


	// ti sua lai cai nay
	int recvbuflen = DEFAULT_BUFLEN;
	// nhan response
	char response[200];
	strcpy(response, "");
	recv(socket_1, response, recvbuflen, 0);
	printf("\nresponse = %s\n", response);
	system("pause");
	key * list_response,*head;
	head = get_response(response);
	list_response  = head;   // Dau cua list
	// kiem tra list
	while (strcmp(list_response->name,"") != 0) {
		printf("\nName : %s\tValue : %s", list_response->name, list_response->value);
		list_response++;
	}
	//Dc roi

	// nhan thong diep tu tracker dinh ky 
	//thead 1

	

	WaitForSingleObject(tracker, 10);



	// xu ly response va handshake voi seeder
	list_response = head;  // Tro ve dau cua list
	char seeder_ip[100];
	char seeder_port[100];
	strcpy(seeder_ip, find_value(list_response, "ip"));
	printf("\n\n\nseeder ip : %s\n", seeder_ip);
	strcpy(seeder_port, find_value(list_response, "port"));
	printf("\nseeder port : %s\n", seeder_port);
	//ĐẾn đoạn này là xong phần client vs tracker
	// dung socket moi dùng ip và soket trả về ở trên seeder_port và seeder_ip

	
	/*socket_2 = start_socket(seeder_ip, DEFAULT_PORT);
	char * hand_shake_to_seeder;
	hand_shake_to_seeder = get_handshake_to_seeder();
	iResult = send(socket_2, hand_shake_to_seeder, (int)strlen(hand_shake_to_seeder) + 1, 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(socket_2);
		WSACleanup();
		system("pause");
		return 1;
	}*/


	//char hand_shake_from_seeder[200];
	//recv(socket_2, hand_shake_from_seeder, recvbuflen, 0);
	//key * list_hand_shake;
	//list_hand_shake = decode(hand_shake_from_seeder);


	// Lay manh dau tien trong info_hash va bat dau yeu cau
	// info_hash lay o meta info xu  ly dau tien
	//char * info_hash;

	//do {
	//	char info_hash_temp[21];
	//	char file_name[10];
	//	
	//	static int i = 0;
	//	memcpy(info_hash_temp, info_hash + 20 * i, sizeof(char)* 20);
	//	i++;
	//	_itoa(i, file_name, 10);

	//	char * pw_msg;
	//	pw_msg = peer_wire_msg(info_hash_temp, "choked");

	//	// gui thong diep
	//	iResult = send(socket_2, pw_msg, recvbuflen, 0);
	//	if (iResult == SOCKET_ERROR) {
	//		printf("send failed with error: %d\n", WSAGetLastError());
	//		closesocket(socket_2);
	//		WSACleanup();
	//		system("pause");
	//		return 1;
	//	}

	//	DownloadFile(file_name, socket_2);
	//} while (1);

	
	/****************************************************/

	
	/*do {
		iResult = recv(socket_1, response, recvbuflen, 0);
		if (iResult > 0) {
			printf("%s\n", response);
		}
		else {
			continue;
		}

	} while (iResult > 0);

*/
	//                Nhan respone
	/*do {

		iResult = recv(socket_1, recvbuf, recvbuflen, 0);
		if (iResult > 0) {
			printf("Bytes received: %d\n", iResult);
			printf("%s", recvbuf);
		}
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed with error: %d\n", WSAGetLastError());

	} while (iResult > 0);
	printf("\n\t%s\n", response);
	key * list_response;
*/

	/********************************************/
	// Seeder


	// ti sua lai cai nay

	// nhan response
	// lam den day roi can kiem tra gia tri recv de dung viec nhan du lieu

	//// dung socket moi


	////Seeder
	int iResult;
	

	char file_name[] = "anh-11111.jpg";
	SOCKET socket_2;
	socket_2 = start_socket(seeder_ip, seeder_port);

	/*recv(socket_2, hand_shake_from_seeder, 1024, 0);*/
	//key * list_hand_shake;
	//list_hand_shake = decode(hand_shake_from_seeder);
	
	char hand_shake_to_seeder[1000];
	strcpy(hand_shake_to_seeder, get_handshake_to_seeder());
	
	iResult = send(socket_2, hand_shake_to_seeder, (int)strlen(hand_shake_to_seeder) + 1, 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(socket_2);
		WSACleanup();
		system("pause");
		return 1;
	}

	// tao 1 thread thu 2 trao doi thong diep voi seeder
	

	// nhan va request du lieu
	char hand_shake_from_seeder[200];

	recv(socket_2, hand_shake_from_seeder, 1024, 0);
	printf("\n response handshake =  %s\n", hand_shake_from_seeder);

	while (!hand_pwg(hand_shake_from_seeder, socket_2)) {
		iResult = recv(socket_2, hand_shake_from_seeder, 1024, 0);
		if (iResult < 0) {
			printf("\nPhien lam viec ket thuc do socket dong dot ngot.\n");
			Sleep(1000);
			return 0;
		}
		printf("\n response handshake =  %s\n", hand_shake_from_seeder);
	}

	// thread 2 trao doi pwg_msg

	WaitForSingleObject(seeder, 100);

	// thread 3 de download
	//WaitForSingleObject(dow_from_seeder, 100);

	DownloadFile(file_name, socket_2);
	// Noi manh lai voi nhau 
	//file_cat();

	printf("\nFile duoc tai thanh cong !\n");

	closesocket(socket_1);
	closesocket(socket_2);
	WSACleanup();
	
	
	/******************************************************/

	system("pause");
	return 0;
}