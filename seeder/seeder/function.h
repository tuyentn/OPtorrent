
#include "sha1.h"
#include "socket.h"
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

//Gui thong diep bat tay
char * send_handshake(){
	char response[1000];
	char *p;
	p = response;
	strcat(response, "d");  //Begin a dictionary bencode
	strcat(response, bencode_str("ptrslen"));
	strcat(response, bencode_num("9"));
	strcat(response, bencode_str("pstr"));
	strcat(response, bencode_str("one piece"));
	strcat(response, bencode_str("info_hash"));
	strcat(response, bencode_str(sha1(FILENAME)));
	strcat(response, bencode_str("peer_id"));
	strcat(response, bencode_str(PEER_ID));
	strcat(response, "e");//End dictionary
	strcat(response, "\0");
	return response;
}

//Ham lay dia chi IP
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
	return ip;
}



//Tao file torrent dua tren thong tin nhap vao
//Example: make_torrent("test2.txt", "192.168.1.33", "22/4/2016", "This is a fake comment", "2-alt team", FILENAME);
int make_torrent(char* nametorrent, char*announce, char * creationdate, char *comment, char * createdby, char *filename){
	//strcat(nametorrent, ".txt");  //Them dinh dang .txt
	FILE *f;
	f = fopen(nametorrent, "w");
	fputc('d', f);

	//Gan cac name key va cac value tuong ung
	char temp[11][100];
	char sha1_hash[21];
	strcpy(sha1_hash, sha1(filename));
	strcpy(temp[0], bencode_str("announce"));
	strcpy(temp[1], bencode_str(announce));
	strcpy(temp[2], bencode_str("creation date"));
	strcpy(temp[3], bencode_str(creationdate));
	strcpy(temp[4], bencode_str("comment"));
	strcpy(temp[5], bencode_str(comment));
	strcpy(temp[6], bencode_str("created by"));
	strcpy(temp[7], bencode_str(createdby));
	strcpy(temp[8], bencode_str("info_hash"));
	strcpy(temp[9], bencode_str(sha1_hash));
	strcpy(temp[10], "");

	// ghi vao file
	for (int i = 0; i < 10; i++){ fputs(temp[i], f); }
	fputc('e', f);
	fclose(f);
	return 1;
}

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
//Khoi tao csdl name_file
int init_name_file(){
	FILE *ptr;
	ptr = fopen("name_info.txt", "w");
	char info[21];
	strcpy(info, sha1(FILENAME));
	fputs(info, ptr);
	fputc(' ', ptr);
	fputs(FILENAME, ptr);
	fputc('\n', ptr);
	fclose(ptr);
	return 1;
}