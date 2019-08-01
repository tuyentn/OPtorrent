

#include "socket.h"
#include "sha1.h"
#include "interface.h"

//Lay dia chi IP
char *getip(){
	WSAData wsaData;
	if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0) {
		return "255";
	}
	char ac[80];
	char ip[50];
	if (gethostname(ac, sizeof(ac)) == SOCKET_ERROR) {
		WSAGetLastError();
		return "error";
	}

	struct hostent *phe = gethostbyname(ac);
	if (phe == 0) {
		return "error";
	}
	for (int i = 0; phe->h_addr_list[i] != 0; ++i) {
		struct in_addr addr;
		memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
		if (i == 0) strcpy(ip, inet_ntoa(addr));
	}
	
	WSACleanup();
	printf("%s", ip);
	system("cls");
	return ip;
}

//bencode
char * bencode_str(char *str){
	char temp[100];
	int i = 0;
	while (str[i] != NULL) i++;
	char count[5];
	_itoa(i, count, 10);
	strcpy(temp, count);
	strcat(temp, ":");
	strcat(temp, str);
	strcat(temp, "\0");
	return temp;
}
char * bencode_num(char *str){
	char tempx[100];
	strcpy(tempx, "i");
	strcat(tempx, str);
	strcat(tempx, "e");
	strcat(tempx, "\0");
	return tempx;
}


//decode cua bencoding

//                               DECODE of bencoding
//For example use:
//	FILE *ptr;
//	ptr = fopen("test2.torrent", "r");
//	char buff[1000];
//	char c;
//	int j = 0;
//	while ((c = getc(ptr)) != EOF) {
//		buff[j] = c;
//		j++;
//	}
//	key *x;
//	x = decode(buff);
//	char value[100];
//	strcpy(value, find_value(x, "created by"));
//	printf("%s", value);
//	while (strcmp(x->name, "")){
//	printf("Name:%s\nValue:%s\n", x->name, x->value);
//	x++;
//	}
typedef struct {
	char name[30];
	char value[100];
} key;
key *decode(char *str){  //Dau vao la 1 thu vien "d.....e"
	key list[20];
	char *p;
	p = str;
	p++;
	int j;
	int i = 0;
	for (i = 0; ((*p != NULL) && (*p != 'e')); i++){
		int k = 0;
		char lena[4];
		while ((*p != ':') && (*p != NULL) && (*p != 'e')){            //Lay do dai cua NAME
			lena[k] = *p;
			p++; k++;
		}
		lena[k] = 0;
		//doi do dai sang kieu int
		p++;
		int len = atoi(lena);
		//lay name
		for (j = 0; j < len; j++){
			list[i].name[j] = *p;
			p++;
		}
		list[i].name[j] = 0;

		//   GET VALUE if is string
		if (*p != 'i'){
			k = 0;
			char lenc[4];
			while ((*p != ':') && (*p != NULL) && (*p != 'e')){            //Lay do dai cua VALUE
				lenc[k] = *p;
				p++; k++;
			}
			lenc[k] = 0;
			p++;
			len = atoi(lenc); //doi thanh int

			//lay gia tri
			for (j = 0; j < len; j++){
				list[i].value[j] = *p;     //lay value vao list[i]
				p++;
			}
		}
		//GET VALUE if is integer
		else {
			p++;//bo qua chu cai 'i'
			j = 0;
			while (*p != 'e'){ //chua den 'e'
				list[i].value[j] = *p;
				j++;
				p++;
			}
			p++;  //Bo qua chu cai e
		}
		list[i].value[j] = 0;
	}
	strcpy(list[i].name, "");
	strcpy(list[i].value, "");
	return list;
}
char *find_value(key *input, char *look_name){
	int i = 0;
	key *list = input;
	while (i<20){
		if (strcmp(list->name, look_name) == 0)  //Neu ma tim thay look_name o trong list->name
			return list->value;
		i++;
		list++;
	}
	return "Khong tim thay name";
}
key *decode_list(char *str){  //Dau vao la 1 list "l.....e"
	key list[20];
	char *p;
	p = str;
	p++; p++;           //Bo qua chu 'l' va chu 'd'
	int j;
	int i = 0;
	for (i = 0; ((*p != NULL) && (*p != 'e')); i++){
		int k = 0;
		char lena[4];
		while ((*p != ':') && (*p != NULL) && (*p != 'e')){            //Lay do dai cua NAME
			lena[k] = *p;
			p++; k++;
		}
		lena[k] = 0;
		//doi do dai sang kieu int
		p++;
		int len = atoi(lena);
		//lay name
		for (j = 0; j < len; j++){
			list[i].name[j] = *p;
			p++;
		}
		list[i].name[j] = 0;

		//   GET VALUE if is string
		if (*p != 'i'){
			k = 0;
			char lenc[4];
			while ((*p != ':') && (*p != NULL) && (*p != 'e')){            //Lay do dai cua VALUE
				lenc[k] = *p;
				p++; k++;
			}
			lenc[k] = 0;
			p++;
			len = atoi(lenc); //doi thanh int

			//lay gia tri
			for (j = 0; j < len; j++){
				list[i].value[j] = *p;     //lay value vao list[i]
				p++;
			}
		}
		//GET VALUE if is integer
		else {
			p++;//bo qua chu cai 'i'
			j = 0;
			while (*p != 'e'){ //chua den 'e'
				list[i].value[j] = *p;
				j++;
				p++;
			}
			p++;  //Bo qua chu cai e
		}
		list[i].value[j] = 0;
	}
	strcpy(list[i].name, "");
	strcpy(list[i].value, "");
	return list;
}
//Khi dung phai khai bao
//key *list;
//list = decode(meg);
//char value[100];
//strcpy(value, find_value(x, name_key));
char tracker_ip[50];

key * get_metainfo(){
	FILE * f;

	// list tra ve
	key * result;


	char tor_name[80];
	char c;
	char info[1000];
	int i = 0;
	char info_hash[100];
	char tracker_ip[50];
	
	printf("\nNhap file torrent can xu ly : ");
	fflush(stdin); gets(tor_name);



	f = fopen(tor_name, "r");
	if (f == NULL) {
		printf("Khong mo duoc file");
		Sleep(2000);
		exit(1);
	}
	

	while ((c = getc(f)) != EOF) {
		info[i] = c;
		i++;
	}
	info[i] = 0;

	// lay info_hash va ip tracker

	result = decode(info);
	printf("test");
	strcpy(info_hash, find_value(result, "info_hash"));
	strcpy(tracker_ip, find_value(result, "announce"));
	printf("\n%s", tracker_ip);
	socket_1 = start_socket(tracker_ip, DEFAULT_PORT);
	/*while (strcmp(result->name,"")) {
		printf("\nname : %s\tvalue : %s\n", result->name, result->value);
		result++;
	
	}*/

	return result;

}

char * msg_request() {
	char request[1000];
	strcpy(request, "");
	strcat(request, "d");  //Begin a dictionary bencode
	strcat(request, bencode_str("info_hash"));
	char sha[30];
	strcpy(sha, sha1(FILENAME));

	strcat(request, bencode_str(sha));
	strcat(request, bencode_str("peer_id"));
	strcat(request, bencode_str(PEER_ID));
	strcat(request, bencode_str("port"));
	strcat(request, bencode_num(DEFAULT_PORT));
	strcat(request, bencode_str("downloaded"));
	strcat(request, bencode_num("9876"));
	strcat(request, bencode_str("left"));
	strcat(request, bencode_num("1234"));
	strcat(request, bencode_str("ip"));
	strcat(request, bencode_str(getip()));
	strcat(request, bencode_str("event"));

	strcat(request, bencode_str("started"));

	
	//strcat(request, bencode_str(sha1(FILENAME)));
	
	strcat(request, "e");//End dictionary
	strcat(request, "\0");
	return request;
	//d8:peer_id6:ABCXYZ4:porti9989e10:downloadedi9876e4:lefti1234e2:ip12:192.168.1.905:event7:startede
}

void send_request() {
	
	char request[200];
	char response[200];
	int iResult;
	strcpy(request, msg_request());
	printf("\n\tSending request ");
	for (int i = 0; i < 5; i++){
		Sleep(500);
		printf(". ");
	}
	printf("\n");
	iResult = send(socket_1, request, (int)strlen(request) + 1, 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(socket_1);
		WSACleanup();
		system("pause");
		return;
	}
	printf("\n\tSending Successful !!!\n");
	return;
}

key * get_response(char * response) {
	key *list;
	
	
	list = decode(response);
	
	return list;
}


char * get_handshake_to_seeder(){
	char handshake[1000];
	strcpy(handshake, "");
	strcat(handshake, "d");  //Begin a dictionary bencode
	strcat(handshake, bencode_str("ptrslen"));
	strcat(handshake, bencode_num("9"));
	strcat(handshake, bencode_str("pstr"));
	strcat(handshake, bencode_str("one piece"));
	strcat(handshake, bencode_str("info_hash"));
	char sha[30];
	strcpy(sha, sha1(FILENAME));
	
	strcat(handshake, bencode_str(sha));
	
	strcat(handshake, bencode_str("peer_id"));
	strcat(handshake, bencode_str(PEER_ID));
	strcat(handshake, "e");//End dictionary
	strcat(handshake, "\0");
	//printf("%s", handshake);
	return handshake;
}


char * peer_wire_msg(char * option) {
	char pw_msg[1000];
	strcpy(pw_msg, "");
	strcat(pw_msg, "d");  //Begin a dictionary bencode

	if (strcmp(option, "choked") == 0) {
		strcat(pw_msg, bencode_str("len"));
		strcat(pw_msg, bencode_num("0001"));
		strcat(pw_msg, bencode_str("id"));
		strcat(pw_msg, bencode_num("0"));
	}
	else if (strcmp(option, "unchoked") == 0){
		strcat(pw_msg, bencode_str("len"));
		strcat(pw_msg, bencode_num("0001"));
		strcat(pw_msg, bencode_str("id"));
		strcat(pw_msg, bencode_num("1"));
	}
	else if (strcmp(option, "interested") == 0) {
		strcat(pw_msg, bencode_str("len"));
		strcat(pw_msg, bencode_num("0001"));
		strcat(pw_msg, bencode_str("id"));
		strcat(pw_msg, bencode_num("2"));
	}
	else if (strcmp(option, "not interested") == 0) {
		strcat(pw_msg, bencode_str("len"));
		strcat(pw_msg, bencode_num("0001"));
		strcat(pw_msg, bencode_str("id"));
		strcat(pw_msg, bencode_num("3"));
	}
	else if (strcmp(option, "request") == 0) {
		strcat(pw_msg, bencode_str("len"));
		strcat(pw_msg, bencode_num("0013"));
		strcat(pw_msg, bencode_str("id"));
		strcat(pw_msg, bencode_num("6"));
		strcat(pw_msg, bencode_str("index"));
		strcat(pw_msg, bencode_num("0"));
		strcat(pw_msg, bencode_str("begin"));
		strcat(pw_msg, bencode_num("0"));


	}
	else if (strcmp(option, "cancel") == 0) {
		strcat(pw_msg, bencode_str("len"));
		strcat(pw_msg, bencode_num("0003"));
		strcat(pw_msg, bencode_str("id"));
		strcat(pw_msg, bencode_num("8"));
	}
	else printf("\n\tSai option message!\n");

	

	//strcat(pw_msg, bencode_str(sha1(FILENAME)));
	
	strcat(pw_msg, "e");//End dictionary
	strcat(pw_msg, "\0");
	printf("\n%s", pw_msg);
	return pw_msg;
	
}

int hand_pwg(char * hand_shake_from_seeder, SOCKET socket) {
	key * list_hand_shake;
	char id_pwg[10];
	list_hand_shake = decode(hand_shake_from_seeder);

	// tim id
	strcpy(id_pwg, find_value(list_hand_shake, "id"));
	int id = atoi(id_pwg);
	char request_pwg_response[100];
	switch (id) {
	case 0: printf("\nChoked Message"); break;
	case 1: printf("\nUnchoked Message"); break;
	case 2:
		printf("\nInterested Message.\n");
		strcpy(request_pwg_response, peer_wire_msg("request"));
		break;

	case 3:
		printf("\nUninterested Message.");

		break;
	case 4: printf("\nHave Message"); break;
	case 5: printf("\nBitfield Message"); break;
	case 6: printf("\nRequest Message"); break;
	case 7: printf("\nPiece Message"); break;
	case 8: printf("\nCancel Message."); break;
	default:
		printf("ID khong hop le ");
		break;
	}
	if (id == 2) {
		int iSendResult = send(socket, request_pwg_response, (int)strlen(request_pwg_response) + 1, 0);
		if (iSendResult < 0){
			printf("\n\tKhong send duoc \n");
		}
		return 1;
	}
	else {
		return 0;
	}
}

void DownloadFile(char* file_name, SOCKET Socket){
	if (Socket == NULL){
		return;
	}


		char GotFileSize[1024];
		// Nhan size cua goi tin
		recv(Socket, GotFileSize, 1024, 0);
		long FileSize = atoi(GotFileSize);
		long SizeCheck = 0;

		// Tao file de viet 
		FILE *fp = fopen(file_name, "wb");
		char* mfcc;


		// bat dau nhan file
		mfcc = (char*)malloc(FileSize + 1);
		while (SizeCheck < FileSize){
			int Received = recv(Socket, mfcc, FileSize, 0);
			int Written = fwrite(mfcc, sizeof(char), Received, fp);
			SizeCheck += Written;

		}
		fclose(fp);
		Sleep(500);
		free(mfcc);
	
}

void file_cat(){
	FILE * pFile, * main_file;
	char file_name[20];
	char main_file_name[] = "mle elamasd a sdoqw sad ioz c";
	char c;
	

	//ten file chinh can tai ve
	main_file = fopen(main_file_name, "wb");


	//cac manh
	int i = 1;
	do {
		_itoa(i, file_name, 10);
		i++;

		// mo cac manh theo thu tu 1 2 3
		pFile = fopen(file_name, "rb");
		if (pFile == NULL) {
			// het thoat vong lap
			printf("Het roi");
			break;
		}
		else {

			// noi cac manh vao file chinh
			while ((c = getc(pFile)) != EOF) {
				fputc(c, main_file);
			}
		}
		fclose(pFile);
	} while (1);
	fclose(main_file);
}

void vs_tracker(){
	while (1) {
		char msg_tracker[200];
		int kq;
		kq = recv(socket_1, msg_tracker, sizeof(msg_tracker), 0);
		if (kq < 0) {
			printf("Ket noi voi tracker ket thuc vi loi lien ket\n");
			break;
		}
	}

}
void vs_seeder() {
	while (1) {
		char pwg_msg[200];
		int kq;
		kq = recv(socket_2, pwg_msg, sizeof(pwg_msg), 0);
		if (kq < 0) {
			break;
		}
		hand_pwg(pwg_msg, socket_2);
	}
}
void dow_seeder() {
	DownloadFile(file_name, socket_2);

}