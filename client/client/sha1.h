
char * sha1(char * str){
	int i = 0, len;
	len = strlen(str);
	char encode[21];
	char *pstr, temp;
	strcpy(encode, "one piece one punch");
	pstr = str;
	while ((*pstr != '\0') && (i<20)){
		encode[i] = *pstr;
		i++; pstr++;
	}
	for (int j = 0; j < len; j++){
		for (i = 0; i < 20; i++){
			encode[i] = encode[i] + len;
		}
		for (i = 0; i < 20; i++){
			temp = encode[i];
			encode[i] = encode[19 - i];
			encode[19 - i] = temp;
		}
	}

	encode[20] = '\0';
	
	return encode;
}