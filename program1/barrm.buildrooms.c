#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>

//Function to print the rooms out after they have been created
void printRooms(char* array_of_names[10], int chosen_names[7], int numofConnections[7], int outboundConnections[7][7]){
int i=0;
//creating a file pointer
FILE *fp;

int j=0;
int y=0;
	//for loop to open 7 files and put in the contents
	for(i=0; i<7; i++){
		//opens a file and names it the name at the first index of the 7 names I have randomly chosen
		fp = fopen(array_of_names[chosen_names[i]], "w");
		//prints to the file the room name at i
		fprintf(fp, "ROOM NAME: %s\n", array_of_names[chosen_names[i]]);
		//for loop to print the number of connections each room has
		for(y=0; y < numofConnections[i]; y++){
			y=0;
			//for loop for each connection
				for(j=0; j<7; j++){
					//prints out the number of room connections
					if(outboundConnections[i][j] == 1){
					fprintf(fp, "ROOM CONNECTION %d: ", y+1);
					//prints out the name of each room connection from the list of names chosen
					fprintf(fp, "%s\n", array_of_names[chosen_names[j]]);
					y++;
			}
			}
		}
		//if else loop to set the first room as the start room, the last room as the end room, and everything else as the mid room
		if(i==0){
			fprintf(fp, "ROOM TYPE: START_ROOM\n");
		}
		else if(i==6){
			fprintf(fp, "ROOM TYPE: END_ROOM\n");
		}
		else{
			fprintf(fp, "ROOM TYPE: MID_ROOM\n");
		}

	}	
}

//Function to determine if the graph is full
int isGraphFull(int numofConnections[7]){
int i=0;
int goback = 1;
	for(i=0; i<7; i++){
		//if the number of connections at i are less than 3 then the graph is not full and it sets goback to 0, otherwise returns goback
		if(numofConnections[i] < 3){
			goback = 0;
		}
	}
	return goback;
}

//Function to connect the rooms if the predetermined conditions are satisfied
void connectRoom(int outboundConnections[7][7], int numofConnections[7], int room1, int room2){
		//sets the outbound connections two dimensional matrix to i at room1,room2 to show that they are connected
		outboundConnections[room1][room2] = 1;
		outboundConnections[room2][room1] = 1;
		//increments the count of connections for room1 and room2
		numofConnections[room1]++;
		numofConnections[room2]++;
}

//Function to add a connection
void addRandomConnection(int numofConnections[7], int outboundConnections[7][7]){

//initalizes two rooms
int room1;
int room2;

while(true)
{
	//calls getRandomRoom function and sets the returned int to room1
	room1 = getRandomRoom();

	//calls canAddConnectionfrom to see if it returns true
	if(canAddConnectionFrom(numofConnections, room1) == 1)
		break;
}
int c = 0;

//does the following code while it satisfies at least one of the conditions in the while loop
do{
	c++;
	//calls getRandomRoom function and sets the returned int to room2
	room2 = getRandomRoom();

}while(canAddConnectionFrom(numofConnections, room2) == 0 || isSameRoom(room1, room2) == 1 || connectionAlreadyExists(outboundConnections, room1, room2) == 1);

	//calls connectRoom function if all predetermined conditions are satisfied
	connectRoom(outboundConnections, numofConnections, room1, room2);

		// int z = 0;
		// int y = 0;
		// for(z = 0; z < 7; z++){
		// 	for(y = 0; y < 7; y++){
		// 		printf("%d ", outboundConnections[z][y]);
		// 	}
		// 	printf("\n");
		// }

}

//function to return a random int between 0 and 6
int getRandomRoom(){

	return rand() % 7;

}

//function that checks if you can add a connection
int canAddConnectionFrom(int numofConnections[7], int room1){

	//checks if the number of connections at room 1 are less than 6
	if(numofConnections[room1] < 6){
		return 1;
	}
	else
		return 0;
}

//function that checks if a connection already exists between two rooms
int connectionAlreadyExists(int outboundConnections[7][7], int room1, int room2){
	//if it is 1 at room1 and room2 then a connection exists
	if(outboundConnections[room1][room2] == 1){
		return 1;
	}
	else{	
		return 0;
	}
}


//function that checks if it is the same room
int isSameRoom(int room1, int room2){
	//if room1 is equivelent to room2 then return 1
	if(room1 == room2){
		return 1;
	}
	else
		return 0;
}


int main(){

//*****MAKING THE DIRECTORY*********************
//Code from piazza
char rooms_dir[] = "barrm.rooms."; //sets rooms_dir to the prefix for all the rooms
char pid_buffer[20]; //sets a buffer
pid_t processID = getpid(); //gets the pid
sprintf(pid_buffer, "%d", processID); //puts the processID into the buffer
strcat(rooms_dir, pid_buffer); //cats the buffer onto the end of the rooms_dir prefix
//strcat(rooms_dir, "/");
mkdir(rooms_dir, 0755); //makes the directory with the correct permissions
chdir(rooms_dir); //changesn into the directory that was made

//**********************************************
//*****PICKING 7 RANDOM ROOMS FROM 10 HARDCODED *********************
//setting the seed
srand(time(NULL));	

//creating a hardcoded array of room names
char* array_of_names [10] = {"roomA", "roomB", "roomC", "roomD", "roomE", "roomF", "roomG", "roomH", "roomI", "roomJ"};
//an array of indices that will be used for each room
int chosen_room[10]={0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
//a two dimensional matrix that holds a 0 or 1 for the connections
int outboundConnections[7][7] = { {0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0} };
//an array to hold the number of connections for each room
int numofConnections[7] = {0, 0, 0, 0, 0, 0, 0};

//initalizing variables
int actual_rooms = 7;
int i=0;
int new_rooms;
int chosen_names[actual_rooms];
//int chosen_roomtypes[7];

//for loop to randomly pick 7 rooms out of the 10 hardcoded
for(i=0; i<actual_rooms; i++){
	//gets a random number between 0 and 9
	new_rooms = rand() % 10;
	//if it randomizes a duplicate room then it will be set to -1 
	if (chosen_room[new_rooms] == -1){
		i--;
		continue;
	}
	//sets the chosen room into the i spot in chosen_names
	chosen_names[i] = new_rooms;
	//printf("%d\n", chosen_names[i]);
	//sets the chosen room to -1
	chosen_room[new_rooms] = -1;
}

//**********************************************

//while loop to loop until the graph is full
while (isGraphFull(numofConnections) == 0)
{
	//function call to add a connection if the graph is not full
	addRandomConnection(numofConnections, outboundConnections);

}
printRooms(array_of_names, chosen_names, numofConnections, outboundConnections);

// int z=0;
// for(z=0; z<7; z++){
// 	printf("Room %d: connections: %d\n", z, numofConnections[z]);
// }
return 0;
}
