#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "funcs.c"

#define option_size 10

void printList(Item **self) {

    printf("TODO LIST\n\n");
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (self[i] != NULL)
            printf("%d [%c] %s", i, self[i]->done, self[i]->text);
    }
}

void upperCase(char *s){
    while (*s) {
        *s = toupper((unsigned char) *s);
        s++;
    }

}

int menu() {
    
    char option[option_size] = {0};

    do {
        

        /*
            Set the cursor to the last line and "clean" it
        */
        getAndSetLine();
        fprintf(stdout, "\r                                ");
        getAndSetLine();


        scanf("%9s", option);
        upperCase(option);

        if (!strncmp(option, "ADD", option_size))
            return ADD;
        if (!strncmp(option, "REMOVE", option_size))
            return REMOVE;
        if (!strncmp(option, "MODIFY",  option_size))
            return MODIFY;
        if (!strncmp(option, "DONE", option_size))
            return DONE;

    } while (strncmp(option, "QUIT", option_size));
    return QUIT;
}

void removeItem(Item **self){
    
    int element;
    scanf("%d", &element);
    
    if(self[element] != NULL){
        free(self[element]->text);
        free(self[element]);
    }
    self[element] = NULL;
}

int main() {

    Item **items = loadFile();
    if(items == NULL)
        items = newList();

    int UP = 1;

    while (UP){
        system("clear");
        printList(items);
        switch (menu()) {
            case ADD:
                addItem(items);
                break;

            case REMOVE:
                removeItem(items);
                break;

            case DONE:
                setDone(items);
                break;

            case QUIT:
                dumpFile(items);
                UP = 0;
                break;
            case MODIFY:{
                Item *task = malloc(sizeof(Item));
                task->text = malloc(sizeof(char)*MAX_TEXT);
                strcpy(task->text, "HACER EL MODIFY\n");
                task->done = NOT_DONE;
                task->size = strnlen(task->text, MAX_TEXT);
                for(int i = 0; i<MAX_ITEMS; i++){
                    if (items[i] == NULL){
                        items[i] = task;
                        break;
                    }
                }
            }break;
            default:
                break;
        }
    }
    return EXIT_SUCCESS;
}
