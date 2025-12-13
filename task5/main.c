#include "list.h"
#include "input.h"
#include "tree.h"

void output(list lst){
    insertVariables(&lst);
    printList(&lst);
    clearList(&lst);
}

int main(){
    list lst;
    buffer buf;
    initList(&lst);
    initBuffer(&buf);

    while(input(&lst))
        output(lst);

    return 0;
}
