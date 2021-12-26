//Project 1 - Counting the Number of Words
//Sicheng Fei




#include <stdio.h> 
#include <stdlib.h>
#define MAX_LEN 30

int main(int argc, char* argv[]) {

    //string has 1 null byte in the end, so total length should +1
    char max[MAX_LEN + 1];

    //argv[0]: ./a.out
    //argv[1]: path to the file
    //"r": read
    FILE* file = fopen(argv[1], "r");

    //exit if file is null
    if (file == NULL)
        return(-1);

    int count;

    //scan file for strings until the end
    //return 1 everytime it finds a string
    while(fscanf(file, "%s", max) == 1) {
        count++;
    }

    //close file
    fclose(file);

    //need "%i" when printing int
    printf("%i", count);

    printf(" total words");

    //new line, needed to make next user command to go to next line
    printf("\n");

    return(0);
}
