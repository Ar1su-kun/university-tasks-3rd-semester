#ifndef TREE_H 
#define TREE_H 


#include "list.h"
#include <stdlib.h>
#include <setjmp.h>
#include <string.h>

extern jmp_buf errorHndlr;

enum type_of_next{
    NXT, AND, OR   // виды связей соседних команд в списке команд
};

struct cmd_inf {
    char ** argv; // список из имени команды и аргументов
    char *infile; // переназначенный файл стандартного ввода
    char *outfile; // переназначенный файл стандартного вывода
    int append;    // нужно для того, чтобы отличить > и >>
    int backgrnd; // =1, если команда подлежит выполнению в фоновом режиме
    struct cmd_inf* pipe; // следующая команда после "|"
    struct cmd_inf* next; // следующая после ";" (или после "&")
    enum type_of_next type;// связь со следующей командой через ; или && или ||
};

typedef struct cmd_inf *tree;
typedef struct cmd_inf node;

void print_tree(tree, int);
int inv();
tree build_tree(list);
void clear_tree(tree);

#endif // TREE_H