#include "function.h"

typedef struct{
	char peer_id[21];
	char port[8];
	char ip[36];
}list_of_peer;
list_of_peer list_peer[20];
int amount_peer = 0;
//get all peer contain info_hash of metainfo file
int get_peer(char * info_hash){
	
	char temp[32], str[1000];
	char *p, c;
	FILE *ptr;
	int j, i = 0;
	ptr = fopen("torrentlist.txt", "r");
	if (ptr == NULL) {
		printf("Khong mo duoc file ");
		system("pause");
		exit(1);
	}
	while ((c = getc(ptr)) != EOF) {
		str[i] = c;
		i++;
	}
	
	
	str[i] = NULL;
	fclose(ptr);
	strcpy(temp, "");
	strcat(temp, info_hash);
	p = strstr(str, temp);
	if (*p == NULL){ printf("Khong tim thay list_peer Kiem tra lai swarm va info_hash\n"); return 0; }
	p += 20;
	p++;	
	i = 0;
	j = 0;
	while (*p != '}'){
		if (*p != ';'){
			list_peer[i].peer_id[j] = *p;
			j++;
			p++;
		}
		else{
			p++;
			list_peer[i].peer_id[j] = '\0';
			i++;
			j = 0;
		}
	}
	strcpy(str, "\0");
	amount_peer = i;
	
	//Get port and ip of peer
	//for (i = 0; i < amount_peer; i++){
		i = 0;
		j = 0;
		char ids[21];
		char ports[8];
		char ips[36];
		strcpy(ids, list_peer[i].peer_id);
		ptr = fopen("swarm.txt", "r");
		if (ptr == NULL) {
			printf("Khong mo duoc file ");
			system("pause");
			exit(1);
		}
		while ((c = getc(ptr)) != EOF) {
			str[j] = c;
			j++;
		}
		str[j] = NULL;
		fclose(ptr);
		//Look up peer by id

		if (strstr(str, ids) == NULL){
			printf("Loi k tim thay peer_id, kiem tra lai CSDL!\n");
			//continue;
		}
		
		p = strstr(str, ids);
		while (*p != '{') p++;
		p++;
		j = 0;
		while (*p != ';'){
			ports[j] = *p;
			p++;
			j++;
		}
		p++;
		ports[j] = '\0';
		j = 0;
		while (*p != '}'){
			ips[j] = *p;
			p++;
			j++;
		}
		ips[j] = '\0';
		strcpy(list_peer[i].port, ports);
		strcpy(list_peer[i].ip, ips);
	//}
	printf("\n\tGet all peer complete.\n\n");
	return 1;
}
//Send response
char * send_response(){
	char response[1000];
	char *p;
	p = response;
	strcpy(response, "");
	//strcpy(response, "l"); //Begin a list bencode
	for (int i = 0; i < 1; i++){         // i < amount peer
		strcat(response, "d");  //Begin a dictionary bencode
		strcat(response, bencode_str("peer id"));
		strcat(response, bencode_str(list_peer[i].peer_id));
		strcat(response, bencode_str("ip"));
		strcat(response, bencode_str(list_peer[i].ip));
		strcat(response, bencode_str("port"));
		strcat(response, bencode_num(list_peer[i].port));
		strcat(response, "e");//End dictionary
	}
	//strcat(response, "e");
	strcat(response, "\0");
	//printf("\nResponse = %s", response);
	return response;
}
//Update swarm with the new peer
void check_swarm(char * peer_id, char * port, char * ip){
	FILE *ptr1;
	char c;
	char str[500];
	int i = 0;
	//Doc file swarm vao str
	ptr1 = fopen("swarm.txt", "r");
	if (ptr1 == NULL) {
		printf("Khong mo duoc file ");
		system("pause");
		exit(1);
	}
	while ((c = getc(ptr1)) != EOF) {
		str[i] = c;
		i++;
	}
	str[i] = '\0';
	fclose(ptr1);
	//Up date
	ptr1 = fopen("swarm.txt", "a");
	if (strstr(str, ip) != NULL){
		fclose(ptr1);
		return;
	}
	else{
		fprintf(ptr1, "%s{%s;%s}\n", peer_id, port, ip);
	}
	fclose(ptr1);
	printf("\tUpdate swarm complete.\n\n");
}
//Data handle
char * handle_request() {
	FILE	*ptr2;
	ptr2 = fopen("request.txt", "r");
	char info_hash[24];
	char peer_id[21];
	char port[8];
	char ip[36];
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

	

	//Xu li thong diep request!!!
	key *listkey,*p;
	listkey = decode(str);  // Lay toan bo du lieu dua vao list key
	p = listkey;			//con tro p ve dau` list key
	strcpy(info_hash,find_value(p, "info_hash"));
	p = listkey;			//con tro p ve dau` list key
	strcpy(peer_id, find_value(p, "peer_id"));
	p = listkey;			//con tro p ve dau` list key
	strcpy(port, find_value(p, "port"));
	p = listkey;			//con tro p ve dau` list key
	strcpy(ip, find_value(p, "ip"));
	printf("\tMessage handle complete.\n\n");
	check_swarm(peer_id, port, ip);
	
	get_peer(info_hash);
	
	return send_response();
}
