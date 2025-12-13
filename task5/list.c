#include "list.h"
#include<string.h>
#include "input.h"

static void initList(list *lst){
    lst->size = 0;
    lst->capacity = 0;
    lst->data = NULL;
}
static void initBuffer(buffer *buf){
    buf->size = 0;
    buf->capacity = 0;
    buf->data = NULL;
}

static void clearList(list *lst){
    if (lst->data == NULL) 
        return;
    for (int i = 0; i < lst->size; i++)
        free(lst->data[i]);
    free(lst->data);
    lst->size = 0;
    lst->capacity = 0;
    lst = NULL;
}

static void clearBuffer(buffer *buf){
    buf->size = 0;
    buf->capacity = 0;
    buf->data = NULL;
}

static void addSymbol(buffer *buf, int c){
    if(buf->size >= buf->capacity)
        buf->data = realloc(buf->data, buf->capacity += ADD_CAPACITY);

    buf->data[buf->size++] = c;
}

static void shrinkBuffer(buffer *buf){
    buf->capacity = buf->size + 1;
    buf->data = realloc(buf->data, buf->capacity);
    buf->data[buf->size++] = '\0';
}

static void shrinkList(list *lst){
    if(lst->data != NULL){
        lst->data = realloc(lst->data, lst->size * sizeof(lst->data));
        lst->capacity = lst->size;
    }
}

static void addWord(list *lst, buffer *buf){
        shrinkBuffer(buf);

        if (lst->size >= lst->capacity)
            lst->data = realloc(lst->data, (lst->capacity += ADD_CAPACITY) * sizeof(lst->data));
        
        lst->data[lst->size++] = buf->data;
}

static void printSize(list *lst){
    printf("Size: %d\n", lst->size);
}

static void printList(list *lst){
    if (lst->data == NULL)
        return;
    printf("[");
    for (int i = 0; i < lst->size-1; i++)
        printf("%s, ", lst->data[i]);
}
