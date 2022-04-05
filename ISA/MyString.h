/*
 *  Module for handling string
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *GetString() {
    char *str = NULL;
    char *temp_buffer = NULL;
    int length = 0;
    while(1) {
        char temp = getchar();
        if(temp == '\n') break;

        //error
        temp_buffer = (char *)malloc(sizeof(char) * (length + 1));
        for(int x = 0 ; x < length ; x++)
            temp_buffer[x] = str[x];
        free(str);

        temp_buffer[length] = temp;
        str = temp_buffer;
        length++;
    }

    temp_buffer = (char *)malloc(sizeof(char) * (length + 1));
    for(int x = 0 ; x < length ; x++)
        temp_buffer[x] = str[x];
    temp_buffer[length] = 0;
    free(str);

    return temp_buffer;
}

void DeleteCharFromStr(char *str, char ch) {
    int length = strlen(str);
    for(int x = 0 ; x <  length; x++) 
        if(str[x] == ch)
            for(int y = x ; y < length ; y++)
                str[y] = str[y + 1];
}