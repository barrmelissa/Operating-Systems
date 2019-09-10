#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char *encode(char* dataString, char* keyString){

    char chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";
    char * ret = malloc(strlen(dataString)+1);
    int i = 0;
    for(i=0; dataString[i] != '\0'; i++){

        int di = strchr(chars, dataString[i]) - chars;
        int ki = strchr(chars, keyString[i]) - chars;
        ret[i] = chars[(di + ki) % strlen(chars)];
    }
    ret[i] = '\0';
    return ret;


}

char *decode(char* dataString, char* keyString){

    char chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";
    char * ret = malloc(strlen(dataString)+1);
    int i = 0;
    for(i=0; dataString[i] != '\0'; i++){

        int di = strchr(chars, dataString[i]) - chars;
        int ki = strchr(chars, keyString[i]) - chars;
        ret[i] = chars[(di - ki + strlen(chars)) % strlen(chars)];
    }
    ret[i] = '\0';
    return ret;


}


int main(int argc, char* argv[]) {

    char *temp = encode(argv[1], argv[2]);
    printf("%s\n", temp);
    printf("%s\n", decode(temp, argv[2]));



    return 0;
}


// if((strlen(dataString)-2) <= strlen(keyString)){
// 		int i = 0;
// 		int j = 0;
// 		int dataInt, keyInt, addInt, divInt, add2Int, add3Int, div2Int;
// 		for(i = 2; i<strlen(dataString); i++){
		
// 			if(dataString[i] == ' '){
// 				dataInt = 26;
// 				if(keyString[j] == ' '){
// 					keyInt = 26;
// 					addInt = dataInt - keyInt;
// 					// divInt = addInt % 27;
//                     add3Int = divInt + 27;
//                     div2Int = add3Int % 27;
// 					add2Int = div2Int + 65;
// 					if(add2Int == 91){
// 						encryptedString[j] = ' ';

// 					}
// 					else{
// 						encryptedString[j] = add2Int;
// 					}				}
// 				else{
// 					keyInt = keyString[j] - 65;
// 					addInt = dataInt - keyInt;
// 					// divInt = addInt % 27;
//                     add3Int = divInt + 27;
//                     div2Int = add3Int % 27;
// 					add2Int = div2Int + 65;
// 					if(add2Int == 91){
// 						encryptedString[j] = ' ';

// 					}
// 					else{
// 						encryptedString[j] = add2Int;
// 					}				}

// 			}
// 			else{
// 			dataInt = dataString[i] - 65;
			
// 				if(keyString[j] == ' '){
// 					keyInt = 26;
// 					addInt = dataInt - keyInt;
// 					// divInt = addInt % 27;
//                     add3Int = divInt + 27;
//                     div2Int = add3Int % 27;
// 					add2Int = div2Int + 65;
// 					if(add2Int == 91){
// 						encryptedString[j] = ' ';

// 					}
// 					else{
// 						encryptedString[j] = add2Int;
// 					}
// 				}
// 				else{
// 					keyInt = keyString[j] - 65;
// 					addInt = dataInt - keyInt;
// 					// divInt = addInt % 27;
//                     add3Int = divInt + 27;
//                     div2Int = add3Int % 27;
// 					add2Int = div2Int + 65;
// 					if(add2Int == 91){
// 						encryptedString[j] = ' ';

// 					}
// 					else{
// 						encryptedString[j] = add2Int;
// 					}				}

// 			}
// 			j++;

// 		}

