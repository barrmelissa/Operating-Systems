#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(const char *msg) { perror(msg); exit(0); } // Error function used for reporting issues

int main(int argc, char *argv[])
{
	//code from lecture
	int socketFD, portNumber, charsWritten, charsRead;
	struct sockaddr_in serverAddress;
	struct hostent* serverHostInfo;
	//buffers set to 180000 to hold big files
	char buffer[180000];
	char buffer1[180000];
	char buffer2[180000];
    
	if (argc < 3) { fprintf(stderr,"USAGE: %s hostname port\n", argv[0]); exit(0); } // Check usage & args

	// Set up the server address struct
	memset((char*)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
	portNumber = atoi(argv[3]); // Get the port number, convert to an integer from a string
	serverAddress.sin_family = AF_INET; // Create a network-capable socket
	serverAddress.sin_port = htons(portNumber); // Store the port number
	serverHostInfo = gethostbyname("localhost"); // Convert the machine name into a special form of address
	if (serverHostInfo == NULL) { fprintf(stderr, "CLIENT: ERROR, no such host\n"); exit(0); }
	memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length); // Copy in the address

	memset(buffer, '\0', sizeof(buffer)); // Clear out the buffer array
	//file for the message
	FILE *file1;
	file1 = fopen(argv[1], "r"); //read the file
	fgets(buffer2, sizeof(buffer2) - 1, file1); // Get input from the user, trunc to buffer - 1 chars, leaving \0
	buffer2[strcspn(buffer2, "\n")] = '\0'; // Remove the trailing \n that fgets adds
	fclose(file1);
	
	//file for the key
	FILE *keyFile;
	keyFile = fopen(argv[2], "r"); //read the file
	fgets(buffer1, sizeof(buffer1) - 1, keyFile); // Get input from the user, trunc to buffer - 1 chars, leaving \0
	buffer1[strcspn(buffer1, "\n")] = '\0'; // Remove the trailing \n that fgets adds
	fclose(keyFile);

	//creating a char array of the alphabet + the space charcter to compare the message with
	char chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";
	//comparing the char array with the buffer[0] to see if it is in the char array
	int test = strchr(chars, buffer2[0]) - chars;
	//if it is not part of the array throw error
	if(test <= -1 || test > 26){
			fprintf(stderr, "ERROR: Bad characters\n");
			exit(1);
	}
	//if the key is shorter than the message throw error
	if(strlen(buffer2) > strlen(buffer1)){
		fprintf(stderr, "ERROR: Key too short\n");
		exit(1);

	}

	// Set up the socket
	socketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
	if (socketFD < 0) error("CLIENT: ERROR opening socket");
	
	// Connect to server
	if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) // Connect socket to address
		error("CLIENT: ERROR connecting");

	//putting the datastring and keystring together into a format that
	//contains delimators to be used when tokenizing
	strcat(buffer, "e"); //shows that this file is to be encoded only
	strcat(buffer, "#"); //message starts after #
	strcat(buffer, buffer2); //messge
	strcat(buffer, "@"); //end of message
	strcat(buffer, buffer1); //key
	strcat(buffer, "$"); //end of key
	strcat(buffer, "?"); //end of file


	charsWritten = send(socketFD, buffer, strlen(buffer), 0); // Write to the server
	if (charsWritten < 0) error("CLIENT: ERROR writing to socket");
	if (charsWritten < strlen(buffer)) printf("CLIENT: WARNING: Not all data written to socket!\n");
	memset(buffer, '\0', sizeof(buffer)); // Clear out the buffer again for reuse
	
	char completeMessage[180000], readBuffer[1000];
	memset(completeMessage, '\0', sizeof(completeMessage)); // Clear the buffer
	while (strstr(completeMessage, "@") == NULL) // As long as we haven't found the terminal...
	{
		memset(readBuffer, '\0', sizeof(readBuffer)); // Clear the buffer
		charsRead = recv(socketFD, readBuffer, sizeof(readBuffer) - 1, 0); // Get the next chunk
		strcat(completeMessage, readBuffer); // Add that chunk to what we have so far
		if (charsRead == -1) { printf("charsRead == -1\n"); break; } // Check for errors
		if (charsRead == 0) { break; }
	}

	int terminalLocation = strstr(completeMessage, "@") - completeMessage; // Where is the terminal
	//check for errors
	if(charsRead == 0){
		exit(0);
	}
	//check for errors
	if(charsRead == -1){
		exit(0);
	}
	completeMessage[terminalLocation] = '\0'; // End the string early to wipe out the terminal
		
	if (charsRead < 0) error("CLIENT: ERROR reading from socket");
	if(completeMessage[0] == 'd'){
		fprintf(stderr, "CLIENT ERROR: otp_dec cannot use otp_enc_d\n");
		exit(1);
	}

	//print total recieved message
	printf("%s\n", completeMessage);

	close(socketFD); // Close the socket
	return 0;
	
}

