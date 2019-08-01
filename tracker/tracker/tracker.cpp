

// Tracker

#include "tracker.h"


int main()
{

	SOCKET sk = start_socket();
	char response[1000];
	strcpy(response, handle_request());
	//printf("\n\t%s\n", response); thong diep tra loi request
	response_peer(response, sk);
	
	


	system("pause");
	return 0;

}


