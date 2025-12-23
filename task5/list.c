#include "list.h"
#include<string.h>
#include "input.h"
#include<limits.h>
#include <linux/limits.h>

char *varHOME;
char varSHELL[PATH_MAX];
char *varUSER;
char varEUID[20];

void getEnv(){
    varHOME = getenv("HOME");
    ssize_t len = readlink("/proc/self/exe", varSHELL, sizeof(varSHELL) - 1);
    varSHELL[len] = '\0';
    varUSER = getlogin();
    sprintf(varEUID, "%d", getuid());
}

void initList(list *lst){
    lst->size = 0;
    lst->capacity = 0;
    lst->data = NULL;
}
void initBuffer(buffer *buf){
    buf->size = 0;
    buf->capacity = 0;
    buf->data = NULL;
}

void clearList(list *lst){
    if (lst->data == NULL) 
        return;
    for (int i = 0; i < lst->size; i++)
        free(lst->data[i]);
    free(lst->data);
    lst->size = 0;
    lst->capacity = 0;
    lst = NULL;
}

void clearBuffer(buffer *buf){
    buf->size = 0;
    buf->capacity = 0;
    buf->data = NULL;
}

void addSymbol(buffer *buf, int c){
    if(buf->size >= buf->capacity)
        buf->data = realloc(buf->data, buf->capacity += ADD_CAPACITY);

    buf->data[buf->size++] = c;
}

void shrinkBuffer(buffer *buf){
    buf->capacity = buf->size + 1;
    buf->data = realloc(buf->data, buf->capacity);
    buf->data[buf->size++] = '\0';
}

void shrinkList(list *lst){
    if(lst->data != NULL){
        lst->data = realloc(lst->data, lst->size * sizeof(lst->data));
        lst->capacity = lst->size;
    }
}

void addWord(list *lst, buffer *buf){
        shrinkBuffer(buf);

        if (lst->size >= lst->capacity)
            lst->data = realloc(lst->data, (lst->capacity += ADD_CAPACITY) * sizeof(lst->data));
        
        lst->data[lst->size++] = buf->data;
}

void printSize(list *lst){
    printf("Size: %d\n", lst->size);
}

void printList(list *lst){
    if (lst->data == NULL)
        return;
    printf("[");
    for (int i = 0; i < lst->size-1; i++)
        printf("%s, ", lst->data[i]);
    printf("%s]\n", lst->data[lst->size - 1]);
}

void insertVariables(list *lst){
    if (lst->data == NULL)
        return;

    list temp;
    buffer buf;
    initBuffer(&buf);
    initList(&temp);
    getEnv();


    for (int i = 0; i < lst->size; i++){
        clearBuffer(&buf);
        if((lst->data[i][0] != '$')){
            for (int j = 0; j < (int)strlen(lst->data[i]); j++){
                addSymbol(&buf, lst->data[i][j]);
            }
            addWord(&temp, &buf);
        }
        else if(!strcmp(lst->data[i], "$HOME")){
            for(int j = 0; j < (int)strlen(varHOME); j++){
                addSymbol(&buf, varHOME[j]);
            }
            addWord(&temp, &buf);
        }
        else if(!strcmp(lst->data[i], "$SHELL")){
            for(int j = 0; j < (int)strlen(varSHELL); j++){
                addSymbol(&buf, varSHELL[j]);
            }
            addWord(&temp, &buf);
        } 
        else if(!strcmp(lst->data[i], "$USER")){
            for(int j = 0; j < (int)strlen(varUSER); j++){
                addSymbol(&buf, varUSER[j]);
            }
            addWord(&temp, &buf);
        }
        else if (!strcmp(lst->data[i], "$EUID")){
            for(int j = 0; j < (int)strlen(varEUID); j++){
                addSymbol(&buf, varEUID[j]);
            }
            addWord(&temp, &buf);
        }
    }

    shrinkList(&temp);
    clearList(lst);
    *lst = temp;
    clearBuffer(&buf);
}