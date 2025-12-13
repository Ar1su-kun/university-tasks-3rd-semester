#ifndef LIST_H
#define LIST_H

#define ADD_CAPACITY 16

#include<stdio.h>
#include<stdlib.h>

static typedef struct{
    char **data;
    int size;
    int capacity;
} list;

static typedef struct{
    char *data;
    int size;
    int capacity;
} buffer;   
static void initList(list *lst);
static void initBuffer(buffer *buf);
static void clearList(list *lst);
static void clearBuffer(buffer *buf);
static void addSymbol(buffer *buf, int c);
static void shrinkBuffer(buffer *buf);
static void shrinkList(list *lst);
static void addWord(list *lst, buffer *buf);
static void printList(list *lst);
static void printSize(list *lst);

#endif
