#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define MAX_ITEMS 100
#define MAX_TEXT 100

#define NOT_DONE 32
#define DONE 88

#define ADD 1
#define REMOVE 2
#define MODIFY 3
#define QUIT -1

#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))

typedef struct {

    char *text;
    size_t size;
    int done;

} Item;

void autoFlush(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

void getAndSetLine(){
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    gotoxy(0, w.ws_row-1);
}

Item* newItem(){
    Item *task = malloc(sizeof(Item));
    char *buffer = malloc(sizeof(char)*MAX_TEXT);
    autoFlush();
    getAndSetLine();
    fprintf(stdout, "\rTask text: ");
    fgets(buffer, MAX_TEXT, stdin);
    task->text = buffer;
    task->done = NOT_DONE;
    task->size = strnlen(buffer, MAX_TEXT);

    return task;
}

void addItem(Item **items){
    // scanf("%d", &index);
    Item* it = newItem();
    for(int i = 0; i<MAX_ITEMS; i++){
        if (items[i] == NULL){
            items[i] = it;
            break;
        }
    }
}

void setDone(Item **items){
    int index = 0;
    getAndSetLine();
    fprintf(stdout, "\rWhich item is done? ");
    scanf("%d", &index);
    autoFlush();


    if(index < MAX_ITEMS && items[index] != NULL)
        items[index]->done = DONE;

}


/*
    TESTING FUNCTION 
*/
int getStrValue(char *str){
    int value = 0;
    while(*str){
        value += *str;
        str++;
    }
    return value;
}



void dumpFile(Item **list){
    FILE *f = fopen("todo.list", "wb");
    int items = 0;

    for(int i = 0; i<MAX_ITEMS; i++)
        if(list[i] != NULL) items++;
                
    fwrite(&items, sizeof(int), 1, f);

    for(int i = 0; i<MAX_ITEMS; i++){
        if(list[i] != NULL){
            fwrite(&list[i]->size, sizeof(size_t), 1, f);
            fwrite(list[i]->text, sizeof(char), list[i]->size, f);
            fwrite(&list[i]->done, sizeof(int), 1, f);
        }
    }   
}

Item** newList(){
    Item **list = malloc(sizeof(Item)*MAX_ITEMS); 

    for(int i = 0; i<MAX_ITEMS; i++){
        list[i] = NULL;
    }
    return list;
}


/*
    Load "todo.list" if exist, else NULL
*/
Item** loadFile(){
    Item **list = newList();

    FILE *f = fopen("./todo.list", "rb");
    if(f == NULL)
        return NULL;
    int items = 0;
    fread(&items, sizeof(int), 1, f);
    /*
        ITEM READ ORDER
        ITEM {
            .SIZE
            .TEXT
            .DONE
        }
    */
    for(int index = 0; index<items; index++){
        Item *newItem = malloc(sizeof(Item));
        fread(&(newItem->size), sizeof(size_t), 1, f);

        newItem->text = malloc((sizeof(char)*newItem->size)+1);
        fread(newItem->text, sizeof(char), newItem->size, f);

        fread(&(newItem->done), sizeof(int), 1, f);
        list[index] = newItem;
    }
    return list;
}