#include "function.h"



//Kiem tra file chia se xem co info_hash tuong ung khong, neu k co thi DROP CONNECTION
char * check_info(char * info_hash){
	char c,name[100];
	char str[1000];
	int i = 0;

	//DOC co so du lieu name_info.txt vao str
	FILE *ptr ;
	ptr = fopen("name_info.txt", "r");
	while ((c = getc(ptr)) != EOF) {
		str[i] = c;
		i++;
	}
	str[i] = '\0';
	fclose(ptr);
	//Tim kiem info_hash
	char *p;
	p = strstr(str, info_hash);
	if (p == NULL){ 
		printf("Khong ton tai file, hoac sai info_hash"); 
		return "FILE NOT FOUND"; 
	}
	//Lay ten cua file
	p += 21;
	i = 0;
	while (*p != ' '){
		name[i] = *p;
		i++;
		p++;
	}
	name[i] = 0;
	return name;
}
//Data handle
char * handle_handshake() {
	FILE	*ptr2;
	ptr2 = fopen("handshake.txt", "r");
	char info_hash[24];
	char protocol [30];
	char str[500];
	char c;
	int i = 0;
	if (ptr2 == NULL) {
		printf("Khong mo duoc file ");
		system("pause");
		exit(1);
	}
	while ((c = getc(ptr2)) != EOF) {
		str[i] = c;
		i++;
	}
	str[i] = '\0';
	fclose(ptr2);
	char peerid[21];
	//Xu li thong diep handshake!!!
	key *listkey, *p;
	listkey = decode(str);  // Lay toan bo du lieu dua vao list key
	p = listkey;			//con tro p ve dau` list key
	strcpy(info_hash, find_value(p, "info_hash"));
	p = listkey;			//con tro p ve dau` list key
	strcpy(peerid, find_value(p, "peer_id"));
	p = listkey;			//con tro p ve dau` list key
	strcpy(protocol, find_value(p, "pstr"));
	printf("\tHandshake message handle complete.\n\n");
	char name[1000];
	//check_info(info_hash);
	return "hinh-anh-3.jpg";
}
//Message state
#define RESPON "d3:leni0001e2:idi1ee"
char * respose_handshake(){
	char unckocked[100];
	strcat(unckocked, "d");
	strcat(unckocked, bencode_str("len"));
	strcat(unckocked, bencode_num("0001"));
	strcat(unckocked, bencode_str("id"));
	strcat(unckocked, bencode_num("1"));
	strcat(unckocked, "e");
	strcat(unckocked, "\0");
	printf("OK");
	return unckocked;
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
		printf("\n\t1\n");
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
	printf("\n\t%s\n", pw_msg);
	printf("\nOK\n");
	return pw_msg;

}

char * hand_pwg_response() {
	char pwg_response[200];
	char c;

	// tra loi thong diep cua client chon cac option

	printf("Choose an option : \n\t\t1.Choked\n\t\t2.UnChoked\n\t\t3.Interested\n\t\t4.Uninterested");
	printf("\n\t\t Your Choice : "); fflush(stdin); scanf("%c", &c);
	switch (c) {
	case '1':strcpy(pwg_response, peer_wire_msg("choked")); break;
	case '2':strcpy(pwg_response, peer_wire_msg("unchoked")); break;
	case '3':strcpy(pwg_response, peer_wire_msg("interested")); break;
	case '4':strcpy(pwg_response, peer_wire_msg("uninterested")); break;
	default:
		printf("Nhap khong hop le. \n");
		break;
	}
	return pwg_response;
}