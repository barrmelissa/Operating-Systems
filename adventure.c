#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <dirent.h>
#include <time.h>
#include <pthread.h>

pthread_mutex_t MUTEX;

//struct to hold all name variables, type variables, intConnections, and connections
struct Room{

char name[256];
char type[256];
int intConnections;
char connections[6][9];
//struct Room* connections[7];

};

//Function to find the newest directory
//code from 2.4 lecture readings
void newestDir(char *newestDirName){

int newestDirTime = -1; // Modified timestamp of newest subdir examined
  char targetDirPrefix[32] = "barrm.rooms."; // Prefix we're looking for
  
  DIR* dirToCheck; // Holds the directory we're starting in
  struct dirent *fileInDir; // Holds the current subdir of the starting dir
  struct stat dirAttributes; // Holds information we've gained about subdir

  dirToCheck = opendir("."); // Open up the directory this program was run in

  if (dirToCheck > 0) // Make sure the current directory could be opened
  {
    while ((fileInDir = readdir(dirToCheck)) != NULL) // Check each entry in dir
    {
      if (strstr(fileInDir->d_name, targetDirPrefix) != NULL) // If entry has prefix
      {
        stat(fileInDir->d_name, &dirAttributes); // Get attributes of the entry

        if ((int)dirAttributes.st_mtime > newestDirTime) // If this time is bigger
        {
          newestDirTime = (int)dirAttributes.st_mtime;
          memset(newestDirName, '\0', sizeof(newestDirName));
          strcpy(newestDirName, fileInDir->d_name);
        }
      }
    }
  }

  closedir(dirToCheck); // Close the directory we opened

  //printf("Newest entry found is: %s\n", newestDirName);


}

//function to read in all contents and set the names and types
void readInContents(char *newestDirName, char *newestRoomName, char *contents, char *startRoomVar, char *endRoomVar, struct Room *newroom[], int *iforStartroom, int *iforEndroom){
    //Modified from the 2.4 readings code
   // char buffer[256] = {'\0'};
   int i=0;
    DIR* recentRoom; //creates DIR
    recentRoom = opendir(newestDirName); //opens the dir we are looking in
    chdir(newestDirName); //changes into the dir we are looking in
    struct dirent *roomInDir; //Current room being read
    char targetRoomPrefix[32] = "room"; // Prefix we're looking for
    while((roomInDir = readdir(recentRoom)) != NULL) //checking each file in dir
    { 
        if(strstr(roomInDir->d_name, targetRoomPrefix)) //if the entry has the prefix
        {
            //gets each room in the directory
            memset(newestRoomName, '\0', sizeof(newestRoomName)); //fills a block of memory
            memset(contents, '\0', sizeof(contents)); //fills a block of memory
            strcpy(newestRoomName, roomInDir->d_name); //fills a block of memory
            //printf("Room found is: %s\n", newestRoomName);
            FILE *file1 = fopen(newestRoomName, "r"); //initializes the file and opens it to read in
            int size = 0;
            fseek(file1, 0L, SEEK_END); //moves the file pointer to the end
            size = ftell(file1); //sets the size
            fseek(file1, 0L, SEEK_SET); //moves it back to the begainning
    
            fread(contents, sizeof(char), size, file1); //read in the contents into the contents variable
            contents[size] = '\0';
            fclose(file1); //closes the file after we are done with it
            //printf("Contents of file are: %s\n", contents);

            //sets the token and reads in until it hits a new line character
            char *token = strtok(contents, "\n");

            //sets the char variables
            char first_var[50], second_var[50], third_var[50];
            while(token != NULL){
                
                //sscanf reads formatting input from the token and reads in 3 char*
                sscanf(token, "%s %s %s", first_var, second_var, third_var);
                //checks if the second var is equivelent to type
                if(strcmp(second_var, "TYPE:") == 0){
                    //checks if the third var is equivelent to startroom
                    if(strcmp(third_var, "START_ROOM") == 0){
                        //copys the name at newroom[i] into the startroomvar
                        strcpy(startRoomVar, newroom[i]->name);
                        *iforStartroom = i;
                        //*iforStartroom = 0;

                    }
                    //checks if the third var is equivelent to the endroom
                    else if(strcmp(third_var, "END_ROOM") == 0){
                        strcpy(endRoomVar, newroom[i]->name);
                        *iforEndroom = i;
                    }
                    //if it is not the startroom or endroom then it copys the third var into the type at i
                    strcpy(newroom[i]->type, third_var);
                }
                //checks if the secondvar is equivelent to name
                else if(strcmp(second_var, "NAME:") == 0){
                    strcpy(newroom[i]->name, third_var);
                }
                token = strtok(NULL, "\n");
            }
              //printf("newroom type is: %s\n", newroom[i]->type);
              //printf("newroom name is: %s\n", newroom[i]->name);
              i++;

           
        }


    }
    //changes out of the directory
    chdir("..");
    closedir(recentRoom); //closes the directory 

}
//use it 49 time for each file and 7 times
//while loop
//if newroom[i]->name === roomName
//  return newroom[i]

//Function to read in the connections and set them
void readInConnections(char *newestDirName, char *newestRoomName, char *contents, struct Room *newroom[], int numofConnections){
    int i=0;
    int j=0;
    //From 2.4 readings
    DIR* recentRoom; //creats DIR
    recentRoom = opendir(newestDirName); //opens the dir we are looking at
    chdir(newestDirName); //changes into the dir we are looking at
    //for(i=0; i<7; i++){
    struct dirent *roomInDir; //Current room being read
    char targetRoomPrefix[32] = "room"; // Prefix we're looking for
    while((roomInDir = readdir(recentRoom)) != NULL) //checking each file in dir
    { 
        if(strstr(roomInDir->d_name, targetRoomPrefix)) //if the entry has the prefix
        {
            //gets each room in the directory
            memset(newestRoomName, '\0', sizeof(newestRoomName)); //fills a block of memeory
            memset(contents, '\0', sizeof(contents)); //fills a block of memeory
            strcpy(newestRoomName, roomInDir->d_name); //fils a block of memeory
            //printf("Room found is: %s\n", newestRoomName);
            FILE *file1 = fopen(newestRoomName, "r"); //initializes the file and opens it to read in 
            int size = 0;
            fseek(file1, 0L, SEEK_END); //moves the file pointer to the end
            size = ftell(file1); //sets the size
            fseek(file1, 0L, SEEK_SET); //moves it back to the begainning

            fread(contents, sizeof(char), size, file1); //read in the contents into the contents variable
            contents[size] = '\0';
            fclose(file1); //closes the file after we are done with it
            //printf("Contents of file are: %s\n", contents);

            //sets the token and reads in until it hits a new line character
            char *token = strtok(contents, "\n");
            //sets the char variables
            char first_var[50], second_var[50], third_int[50], fourth_char[50], fifth_var[50];
          //  printf("Inside connectionsA\n");
            numofConnections = 0;
            j=0;
            while(token != NULL){
                    //printf("Inside connectionsB\n");
                //sscanf reads formatting input from the token and reads in 5 char*
                sscanf(token, "%s %s %d %c %s", first_var, second_var, third_int, fourth_char, fifth_var);
                //checks if the secondvar is equivelent to connection
                if(strcmp(second_var, "CONNECTION") == 0){
                        numofConnections++;

                        //goes into if statement if the connections are not NULL
                        if(newroom[i]->connections[j] != NULL){
                            //copys the fifthvar into the connections
                            strncpy(newroom[i]->connections[j], fifth_var, 9);
                            //break;
                           j++;

                         }
                    //printf("Connection inside: %s\n", newroom[i]->connections[j-1]);

                }
                token = strtok(NULL, "\n");
        //check token after

            }
                //sets the number of connections
                newroom[i]->intConnections = numofConnections;
                //printf("Num of connections is: %d\n", newroom[i]->intConnections);

                i++;
            }
           

    }
    //changes out of the directory
    chdir("..");
    closedir(recentRoom); //closes the directory after we are done with it 
    
}

//Function to get the time and print it to a file using a mutex
void gettime(){
    FILE *fp; //initalizes the file
    char *filecontents = NULL; //sets the filecontents to NULL
    size_t length; //sets the length
    pthread_mutex_lock(&MUTEX); //locks the mutex
    fp = fopen("currentTime.txt", "r"); //opens the file with read only 
    getline(&filecontents, &length, fp); //uses getline and passes in the length, contents, and the file
    fclose(fp); //closes the file
    pthread_mutex_unlock(&MUTEX); //unlocks the mutex
    printf("%s\n\n", filecontents); //prints the contents to the screen
    free(filecontents); //frees the contents


}
//Function to set the time
static void *setTime(void * arg){
    FILE *fp; //initalizes the file

    time_t rawtime;
    struct tm *info;
    char buffer[80]; //sets the buffer
    time( &rawtime);
    info = localtime( &rawtime ); //passes by reference the rawtime into function localtime and sets that to info
    strftime(buffer, 80, "%l:%M%p, %A, %B %d, %Y", info); //prints the exact format and order of the time
    //printf("<%s>\n", buffer);
    pthread_mutex_lock(&MUTEX); //locks the mutex
    fp = fopen("currentTime.txt", "w+"); //opens the file and writes to it
    //perror("");
    fprintf(fp, "%s\n", buffer); //prints the time to the file
    fclose(fp); //closes the file
    pthread_mutex_unlock(&MUTEX); //unlocks the mutex
}

//Function to print the game
void printGame(struct Room *newroom[], char *newestRoomName, char *startRoomVar, char *endRoomVar, char *totalpath, int totalsteps, int numofConnections, char *userInput, int *iforStartroom, int *iforEndroom, char *indexhelper){
    //sets the indexhelper to the startroom
    indexhelper = newroom[*iforStartroom]->name;
    //printf("**********PRINTING GAME**********\n");
    int i=0;
    int j=0;
    int x=0;
   //int tempVar = *iforStartroom;
   do{
       x=0;
       //only goes into this code if the user did not enter time
       if(strcmp(userInput, "time") != 0 ){
        //prints the current location and name
        printf("CURRENT LOCATION: %s\n", newroom[*iforStartroom]->name);
        //prints the possible connections
        printf("POSSIBLE CONNECTIONS: ");
        //loops through the number of connections
        for(j=0; j<newroom[*iforStartroom]->intConnections; j++){ 
            //checks that the connection is not NULL 
            if(newroom[*iforStartroom]->connections[j] != NULL){
                //checks that the connection is less than j
                if(j<newroom[*iforStartroom]->intConnections -1){
                    //print the connections
                    printf("%s, ", newroom[*iforStartroom]->connections[j]);
                }
                else{
                    printf("%s.\n", newroom[*iforStartroom]->connections[j]);

                }

            }
        }
       }
        printf("WHERE TO? >");
        //take in the user input
        scanf("%s", userInput);
        printf("\n");
        //if the user enters time then go into this if statement
        if(strcmp(userInput, "time") == 0){
            pthread_t timethread; //initalizes the time thread
            pthread_create(&timethread, NULL, &setTime, NULL); //creates the time thread
            pthread_join(timethread, NULL); //joins the time thread
             gettime(); //calls the function gettime
             x = 1;
        }
        //sets a tempVar to the index
        int tempVar = *iforStartroom;

        //for loop for the number of connections
        for(j=0; j<newroom[tempVar]->intConnections; j++){
            //if the user input is equivelent to the one of the connections for the room
            if(strcmp(userInput, newroom[tempVar]->connections[j]) == 0){
               // printf("i is: %d\n", *iforStartroom);
                int p = 0;
                for(p=0; p<7; p++){
                    //if the name at p is equivelent to one of the connections for the room
                    if(strcmp(newroom[p]->name, newroom[tempVar]->connections[j]) == 0){
                        *iforStartroom = p;
                        x = 1;
                        break;
                    }
                }   
                totalsteps++;
            }
        
            
        }
        //if the user enters back input it prints this statement
        if(x != 1){
            printf("HUH? I DON'T UNDERSTAND THAT ROOM. TRY AGAIN.\n\n");
        }

        //if the indexhelper is not equal to the name at i
        if(strcmp(indexhelper, newroom[*iforStartroom]->name) != 0){
            //if the startroomvar is not equal to the name a i
            if(strcmp(startRoomVar, newroom[*iforStartroom]->name) != 0){
            //cat the name at i to the total path
            strcat(totalpath, newroom[*iforStartroom]->name);
            strcat(totalpath, "\n");
            //set the index helper to the newroom name at i
            indexhelper = newroom[*iforStartroom]->name;
            }
            //if the startroomVar is equal to the name for the startroom
            if(strcmp(startRoomVar, newroom[*iforStartroom]->name) == 0){
                //if the userinput is time
                if(strcmp(userInput, "time") == 0){
                    int y = 0;
                }
                else{
                    //if the user input is equivelent to the startroomvar
                    if(strcmp(userInput, startRoomVar) == 0){
                        //cat the name at i to the total path
                        strcat(totalpath, newroom[*iforStartroom]->name);
                        strcat(totalpath, "\n");
                        //set the index helper to the newroom name at i
                        indexhelper = newroom[*iforStartroom]->name;
                    }
                    int u = 0;
                }
            }
        }
        
    }while(*iforStartroom != *iforEndroom); //loops while the startroom is not equal to the endroom
    printf("YOU HAVE FOUND THE END ROOM. CONGRATULATIONS! \n");
    printf("YOU TOOK %d STEPS. YOUR PATH TO VICTORY WAS: \n", totalsteps);
    printf("%s", totalpath);

    

}


int main()
{    
    pthread_mutex_init(&MUTEX, NULL); //initalizes the mutex

    //initalizing variables
    int iforStartroom = 0;
    int iforEndroom = 0;
    int i=0;
    //using malloc for memory for the struct
    struct Room* newroom[7] = {NULL};
    for (i=0; i<7; i++){
         newroom[i] = malloc(sizeof(struct Room));
    }
    //initalizing char arrays
    char newestDirName[256]; // Holds the name of the newest dir that contains prefix
    char newestRoomName[256];
    char contents[256];
    char startRoomVar[256];
    char endRoomVar[256];
    memset(startRoomVar, '\0', sizeof(startRoomVar)); //fills a block of memory
    memset(endRoomVar, '\0', sizeof(endRoomVar)); //fills a block of memory
    //char currentRoom[256];
    //initalizing more variables
    char totalpath[256] = {'\0'};
    int totalsteps = 0;
    int numofConnections = 0;
    char userInput[256];
    char indexhelper[256];

    memset(newestDirName, '\0', sizeof(newestDirName)); //fills a block of memory
    
    newestDir(newestDirName);
  
    //function call for readInContents
    readInContents(newestDirName, newestRoomName, contents, startRoomVar, endRoomVar, newroom, &iforStartroom, &iforEndroom);
   // for(i=0; i <7; i++){
        //printf("name: %s\ntype: %s\n", newroom[i]->name, newroom[i]->type);
   // }
    //printf("Start room is: %s\n", startRoomVar);
    //printf("End room is: %s\n", endRoomVar);

    int j=0;
    //function call for readInConnections
    readInConnections(newestDirName, newestRoomName, contents, newroom, numofConnections);
    //printf("\nNow looking at connections outside the loop\n");
//    for(i=0; i<7; i++){
//         for(j=0; j<7; j++){
//         printf("Connection outside: %s\n", newroom[i]->connections[j]);
//         }
//    }
    //strcpy(newestRoomName, startRoomVar);
    //printf("**************IFORSTARTROOM************* %d\n", iforStartroom);
    //printf("**************IFORENDROOM************* %d\n", iforEndroom);
    //function call to print game
    printGame(newroom, newestRoomName, startRoomVar, endRoomVar, totalpath, totalsteps, numofConnections, userInput, &iforStartroom, &iforEndroom, indexhelper);

    pthread_mutex_destroy(&MUTEX); //destroy mutex

  return 0;

}
