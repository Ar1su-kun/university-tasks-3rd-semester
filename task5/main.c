#include "list.h"
#include "input.h"
#include "tree.h"
#include "exec.h"

int main(){
    if (setjmp(errorHndlr) == 0) {
        list lst;
        initList(&lst);
        while(inv() && input(&lst)){
            insertVariables(&lst);
            tree tr = build_tree(lst);
            
            if(execute(tr)){
                clearList(&lst);
                clear_tree(tr);
                return 0;
            }
            
            clearList(&lst);
            clear_tree(tr);
        }
    }
    return 0;
}
