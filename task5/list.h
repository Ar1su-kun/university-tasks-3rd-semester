#ifndef LIST_H
#define LIST_H

#define ADD_CAPACITY 16

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

typedef struct{
    char **data;
    int size;
    int capacity;
} list;

typedef struct{
    char *data;
    int size;
    int capacity;
} buffer;

void initList(list *lst);
void initBuffer(buffer *buf);
void clearList(list *lst);
void clearBuffer(buffer *buf);
void addSymbol(buffer *buf, int c);
void shrinkBuffer(buffer *buf);
void shrinkList(list *lst);
void addWord(list *lst, buffer *buf);
void printList(list *lst);
void printSize(list *lst);
void insertVariables(list *lst);

#endif
