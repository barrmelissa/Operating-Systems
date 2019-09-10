#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>


void printRooms(char* array_of_names[10], int chosen_names[7]){
int i=0;
FILE *fp;


int j=0;
	for(j=0; j<7; j++){
		//strcpy(rooms_dir, array_of_names[chosen_names[i]])
		//I want to open, print, name, and type all in one, can I?
		fp = fopen(array_of_names[chosen_names[i]], "w");
		fprintf(fp, "ROOM NAME: %s\n", array_of_names[chosen_names[i]]);
		if(i==0){
			fprintf(fp, "ROOM TYPE: START_ROOM\n");
		}
		else if(i==6){
			fprintf(fp, "ROOM TYPE: END_ROOM\n");
		}
		else{
			fprintf(fp, "ROOM TYPE: MID_ROOM\n");
		}
		//for(j=1; j<getConnections(i); j++){
		//	fprintf(fp, "ROOM CONNECTION # %d", j)
		//}
		i++;
}	
}

// while (isGraphFull() == 0)
// {
// 	addRandomConnection();

// }()
/*
char* getRoom(char* array_of_names, int chosen_names, int room1){

	int roomConnected;

	roomConnected = array_of_names[chosen_names[room1]];
	
}

int getConnections(int numofConnections[7]){
int i=0;
int count;
	for(i=0; i<7; i++){
		if(numofConnections[i] > 0){
			count++;
		}
	}
	return count;

}
*/
int isGraphFull(int outboundConnections[7][7])
{
int i=0;
int j=0;
	for(i=0; i<7; i++)
		for(j=0; j<7; j++)
			if(outboundConnections[i][j] >=3 && outboundConnections[i][j] <= 6){
				return 1;			}
			else{
				return 0;
			}
}

void addRandomConnection(int numofConnections[7], int outboundConnections[7][7]){

int room1;
int room2;

while(true)
{
	room1 = getRandomRoom();

	if(canAddConnectionFrom(room1) == 1)
		break;
}

do{

	room2 = getRandomRoom();
}
while(canAddConnectionFrom(numofConnections, room2) == 0 || isSameRoom(room1, room2) == 1 || connectionAlreadyExists(outboundConnections, room1, room2) == 1);

	connectRoom(numofConnections, outboundConnections, room1, room2);
	connectRoom(numofConnections, outboundConnections, room2, room1);

}

int getRandomRoom(){

	return rand() % 7;

}

int canAddConnectionFrom(int numofConnections[7], int room1){

	if(numofConnections[room1] < 6){
		return 1;
	}
	else
		return 0;
}


int connectionAlreadyExists(int outboundConnections[7][7], int room1, int room2){

	if(outboundConnections[room1][room2] > 0){
		return 1;
	}
	else	
		return 0;

}



void connectRoom(int numofConnections[7], int outboundConnections[7][7], int room1, int room2){

		outboundConnections[room1][room2] = 1;
		outboundConnections[room2][room1] = 1;

		numofConnections[room1]++;
		numofConnections[room2]++;
}

int isSameRoom(int room1, int room2){

	if(room1 == room2){
		return 1;
	}
	else
		return 0;
}




int main(){

//*****MAKING THE DIRECTORY*********************
//Code from piazza
char rooms_dir[] = "barrm.rooms.";
char pid_buffer[20];
pid_t processID = getpid();
sprintf(pid_buffer, "%d", processID);
strcat(rooms_dir, pid_buffer);
//strcat(rooms_dir, "/");
int result = mkdir(rooms_dir, 0755);
chdir(rooms_dir);
  
printf("Result of mkdir(): %d\n", result);
        if(result){
        printf("%s\n", "Failed to create dir");
        return 1;
        }
//**********************************************
//*****PICKING 7 RANDOM ROOMS FROM 10 HARDCODED *********************
//only need for print function
srand(time(NULL));	


char* array_of_names [10] = {"room1", "room2", "room3", "room4", "room5", "room6", "room7", "room8", "room9", "room10"};
int chosen_room[10]={0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
int outboundConnections[7][7];
int numofConnections[7];

int actual_rooms = 7;
int i=0;
int new_rooms;
int chosen_names[actual_rooms];
//int chosen_roomtypes[7];


for(i=0; i<actual_rooms; i++){

	new_rooms = rand() % 10;
	if (chosen_room[new_rooms] == -1){
		i--;
		continue;
	}
	chosen_names[i] = new_rooms;
	//printf("%d\n", chosen_names[i]);
	chosen_room[new_rooms] = -1;
}

//**********************************************

printRooms(array_of_names, chosen_names);

while (isGraphFull(outboundConnections == 0))
{
	addRandomConnection(numofConnections, outboundConnections);

}

return 0;
}
