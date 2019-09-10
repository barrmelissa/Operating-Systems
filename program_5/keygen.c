
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

//taking command line arguments
int main(int argc, char *argv[]){

//setting the seed for randomizing
srand(time(NULL));

    //checking if there are no command line arguments
    if(argc <= 1){
        fprintf(stderr, "There were no command line arguments entered.\n", "string format", 46);
        exit(1);
    }
    
    //char * to hold the key
    char *newKeyFile;
    int size;
    //taking the user input and setting that as the size
    size = atoi(argv[1]);

    //mallocing for the size of the key the user wants
    newKeyFile = (char*)malloc(size+1);
    if(size > 0){
        memset(newKeyFile, '\0', sizeof(newKeyFile));
    }

    int i = 0;
    //for loop to randomize for the amount of characters the user wants
    for (i = 0; i<size; i++){
        int first_letter;
        //randomizes 27 letters starting at 65 for capital letters
        first_letter = rand() % 27 + 65;
        //if it is 91 we will treat it as a space and set it to the correct ascii value 32
        if(first_letter == 91){
            newKeyFile[i] = 32;
        }
        //otherwise we add it to the array
        else{
            newKeyFile[i] = first_letter;
        }

    }
    //adding new line character in ascii
    newKeyFile[i] = 10;

    //printing out the final key
    printf("%s", newKeyFile);
    free(newKeyFile);
    return 0;

}

