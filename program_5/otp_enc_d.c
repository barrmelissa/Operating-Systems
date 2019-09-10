#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdbool.h>

//function that does the actual encoding
char *strencode(char* dataString, char* keyString){

	//creating a char array of the alphabet + the space charcter to compare the message with
    char chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";
    //mallocing memory for char * that will hold the encryted string
    char * ret = malloc(strlen(dataString)+1);
    int i = 0;
	int j = 0;
	int k = 0;

	//for loop that goes through the datastring until it hits the null terminator
    for(i=2; dataString[i] != '\0'; i++){
	
		//encrypting
		//comparing the char array with the datastring[i]
		int di = strchr(chars, dataString[i]) - chars;
		//comparing the char array with the keystring[i]
        int ki = strchr(chars, keyString[j]) - chars;
        //add the datastring i to the keystring i and mod 27
		ret[j] = chars[(di + ki) % strlen(chars)];
		j++;

    }
	//adding an '@' so the recieving loop knows when to stop
	ret[j] = '@';
	j = j + 1;
	//adding the null terminator on
    ret[j] = '\0';
	//returning the now encrypted string
    return ret;


}

void error(const char *msg) { perror(msg); exit(1); } // Error function used for reporting issues

//function to tokenize string and ensure it is the right one to be encoded
char* encode(char buffer[], char encryptedString[180000]){

	char *parsedString;
	//serperators that have been strcat onto the string before being sent
	char seperators[] = "@$";
	int i = 0;
	//checking that the file sent is supposed to be encoded, if it was sent with a 'd' it will throw an error
	if(buffer[i] == 'e'){

        char *token = strtok(buffer, seperators);
		//tokenizes the string, takes everything until it hits the first seperator which is '@'
		char *dataString = token;
		token = strtok(NULL, seperators);
		//tokenizes the string, takes everything until it hits the second seperator which is '$'
		char *keyString = token;		

		//calls the function to encode now that the data and key are seperated
		char *temp = strencode(dataString, keyString);
		//copies the encoded string back into the char * and returns it
		strcpy(encryptedString, temp);
		free(temp);
		return encryptedString;

	}
	else{
		//throws error if it is not supposed to be encoded
		fprintf(stderr, "ERROR: otp_dec cannot use otp_enc_d\n");
		//adds the 'e' to the begainning of the string so that when it is sent back it knows that it is incorrect and will terminate
		encryptedString[0] = 'e';
		encryptedString[1] = '\0';
		return encryptedString;

	}

}

//function to verfiy that the connection is coming from the correct program
bool verifyConnection(char buffer[]){

	//verifing that it's from otp_enc
	if(strcmp(buffer, "otp_enc") == 0){
		return true;
	}
	else{
		return false;
	}

}

//function to recieve and process the child
void process_child(char buffer[], int charsRead, int establishedConnectionFD, char encryptedString[180000]){

	//making a buffer size of 180000 to have enough space to hold the large files + the large keys that are strcat together
	memset(buffer, '\0', 180000);	
	char readBuffer[1000];
	memset(buffer, '\0', sizeof(buffer)); // Clear the buffer
	while (strstr(buffer, "?") == NULL) // As long as we haven't found the terminal...
	{
		memset(readBuffer, '\0', sizeof(readBuffer)); // Clear the buffer
		charsRead = recv(establishedConnectionFD, readBuffer, sizeof(readBuffer) - 1, 0); // Get the next chunk
		strcat(buffer, readBuffer); // Add that chunk to what we have so far
		// printf("PARENT: Message received from child: \"%s\", total: \"%s\"\n", readBuffer, completeMessage);
		if (charsRead == -1) { printf("charsRead == -1\n"); break; } // Check for errors
		if (charsRead == 0) { printf("charsRead == 0\n"); break; }

	}
	int terminalLocation = strstr(buffer, "?") - buffer; // Where is the terminal
	if(charsRead == 0){
		exit(0);
	}
	buffer[terminalLocation] = '\0'; // End the string early to wipe out the terminal
	
	//calls the verifyConnnection function to make sure that it is a verified connection
	int verified = verifyConnection(buffer);
	if(verified == 1){
		error("Not verified connection");
		exit(1);
	}

	//error checking
	if (charsRead < 0){
		error("ERROR reading from socket");
	}

	//calls the encode function
	encode(buffer, encryptedString);
	charsRead = send(establishedConnectionFD, encryptedString, strlen(encryptedString), 0); // Send success back

	if (charsRead < 0) error("ERROR writing to socket");
	close(establishedConnectionFD); // Close the existing socket which is connected to the client

}

//taking command line arguments
int main(int argc, char *argv[])
{
	//creating a size of 180000 to hold large files
	char encryptedString[180000];

	//code from lecture
	int listenSocketFD, establishedConnectionFD, portNumber, charsRead;
	socklen_t sizeOfClientInfo;
	char buffer[180000];
	struct sockaddr_in serverAddress, clientAddress;

	if (argc < 2) { fprintf(stderr,"USAGE: %s port\n", argv[0]); exit(1); } // Check usage & args

	// Set up the address struct for this process (the server)
	memset((char *)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
	portNumber = atoi(argv[1]); // Get the port number, convert to an integer from a string
	serverAddress.sin_family = AF_INET; // Create a network-capable socket
	serverAddress.sin_port = htons(portNumber); // Store the port number
	serverAddress.sin_addr.s_addr = INADDR_ANY; // Any address is allowed for connection to this process

	// Set up the socket
	listenSocketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
	if (listenSocketFD < 0) error("ERROR opening socket");

	// Enable the socket to begin listening
	if (bind(listenSocketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) // Connect socket to port
		error("ERROR on binding");
	listen(listenSocketFD, 5); // Flip the socket on - it can now receive up to 5 connections

	//while loop that accepts the message and forks a child process
	while(1){
		sizeOfClientInfo = sizeof(clientAddress); // Get the size of the address for the client that will connect
		establishedConnectionFD = accept(listenSocketFD, (struct sockaddr *)&clientAddress, &sizeOfClientInfo); // Accept
		if (establishedConnectionFD < 0){
			error("ERROR1 on accept");
		}
		//if accepted then fork
		int pid = fork();

		if(pid == -1){
			error("ERROR forking");
			exit(1);
		}
		//child process
		else if(pid == 0){

			//calls the function to process the child
			process_child(buffer, charsRead, establishedConnectionFD, encryptedString);
			//closes the connection
			close(establishedConnectionFD);
			exit(0);

		}
		//parent process
		else{

			//closes the connection
			close(establishedConnectionFD);

		}

	}

		close(listenSocketFD); // Close the listening socket
	
		return 0; 

}

