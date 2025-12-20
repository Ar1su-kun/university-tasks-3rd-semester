#include "tree.h"
#include "input.h"

jmp_buf errorHndlr;

void error(void){
    perror("Error!");
    longjmp(errorHndlr, 1);
}

tree make_cmd(){
    tree new_cmd = (tree)malloc(sizeof(struct cmd_inf));
    if (new_cmd == NULL) {
        error();
    }
    
    new_cmd->argv = NULL;
    new_cmd->infile = NULL;
    new_cmd->outfile = NULL;
    new_cmd->append = 0;
    new_cmd->backgrnd = 0;
    new_cmd->pipe = NULL;
    new_cmd->next = NULL;
    new_cmd->type = NXT;
    
    return new_cmd;    
}

void add_arg(tree cmd, char *arg) {
    if (cmd == NULL || arg == NULL) {
        return;
    }

    int argc = 0;
    if (cmd->argv != NULL)
        while (cmd->argv[argc] != NULL)
            argc++;

    char **new_argv = realloc(cmd->argv, (argc + 2) * sizeof(char *));
    if (new_argv == NULL) {
        error();
    }

    new_argv[argc] = strdup(arg);
    if (new_argv[argc] == NULL) {
        error();
    }

    new_argv[argc + 1] = NULL;
    cmd->argv = new_argv;
}

void clear_tree(tree t) {
    if (t == NULL) return;
    
    clear_tree(t->pipe);
    clear_tree(t->next);
    
    if (t->argv != NULL) {
        for (int i = 0; t->argv[i] != NULL; i++) {
            free(t->argv[i]); 
        }
        free(t->argv); 
    }
        if (t->infile != NULL) {
        free(t->infile);
    }
    if (t->outfile != NULL) {
        free(t->outfile);
    }
        free(t);
}

void make_shift(int n){
    while(n--)
        putc(' ', stderr);
}

void print_argv(char **p, int shift){
    char **q=p;
    if(p!=NULL){
        while(*p!=NULL){
             make_shift(shift);
             fprintf(stderr, "argv[%d]=%s\n",(int) (p-q), *p);
             p++;
        }
    }
}

void print_tree(tree t, int shift){
    char **p;
    if(t==NULL)
        return;
    p=t->argv;
    if(p!=NULL)
        print_argv(p, shift);
    else{
        make_shift(shift);
        fprintf(stderr, "psubshell\n");
    }
    make_shift(shift);
    if(t->infile==NULL)
        fprintf(stderr, "infile=NULL\n");
    else
        fprintf(stderr, "infile=%s\n", t->infile);
    make_shift(shift);
    if(t->outfile==NULL)
        fprintf(stderr, "outfile=NULL\n");
    else
        fprintf(stderr, "outfile=%s\n", t->outfile);
    make_shift(shift);
    fprintf(stderr, "append=%d\n", t->append);
    make_shift(shift);
    fprintf(stderr, "background=%d\n", t->backgrnd);
    make_shift(shift);
    fprintf(stderr, "type=%s\n", t->type==NXT?"NXT": t->type==OR?"OR": "AND" );
    make_shift(shift);
    if(t->pipe==NULL)
        fprintf(stderr, "pipe=NULL \n");
    else{
        fprintf(stderr, "pipe---> \n");
        print_tree(t->pipe, shift+5);
    }
    make_shift(shift);
    if(t->next==NULL)
        fprintf(stderr, "next=NULL \n");
    else{
        fprintf(stderr, "next---> \n");
        print_tree(t->next, shift+5);
    }
}

int getword(char** word, int *i, list lst){
    (*i)++;
    if (*i < lst.size) {
        *word = lst.data[*i];
        return 0;
    }
    *word = NULL;
    return 1;
}

tree build_tree(list lst){
    setjmp(errorHndlr);
    typedef enum {Begin, Conv, Conv1, In, In1, Out, Out1, Backgrnd, End} vertex;
    int i = 0;
    char *word;
    tree beg_cmd, cur_cmd, prev_cmd;

    vertex V = Begin;

    if (lst.size == 0) {
        return NULL;
    }


    while(1)
        switch(V){
            case Begin:
                i = -1;
                
                if (getword(&word, &i, lst)) {
                    V = End;
                    break;
                }
                
                beg_cmd = cur_cmd = make_cmd();
                add_arg(cur_cmd, word);
                prev_cmd = cur_cmd;
                
                if (getword(&word, &i, lst)) {
                    V = End;
                } else {
                    V = Conv;
                }
                    break;
            case Conv:
                if (strcmp(word, "|") == 0) {
                    if (i >= lst.size)
                        error();
                    getword(&word, &i, lst);
                    V = Conv1;
                }
                else if (strcmp(word, "<") == 0) {
                    if (i >= lst.size)
                        error();
                    getword(&word, &i, lst);
                    V = In;
                }
                else if (strcmp(word, ">") == 0) {
                    if (i >= lst.size)
                        error();
                    getword(&word, &i, lst);
                    cur_cmd->append = 0;
                    V = Out;
                }
                else if (strcmp(word, ">>") == 0) {
                    if (i >= lst.size)
                        error();
                    getword(&word, &i, lst);
                    cur_cmd->append = 1;
                    V = Out;
                }
                else if (strcmp(word, ";") == 0) {
                    if (i >= lst.size) {
                        V = End;
                        break;
                    }
                    getword(&word, &i, lst);
                    tree next_cmd = make_cmd();
                    add_arg(next_cmd, word);
                    
                    cur_cmd->next = next_cmd;
                    cur_cmd->type = NXT; 
                    prev_cmd = cur_cmd;
                    cur_cmd = next_cmd;
                    
                    if (getword(&word, &i, lst)) {
                        V = End;
                    } else {
                        V = Conv;
                    }
                }
                else if (strcmp(word, "||") == 0) {
                    if (i >= lst.size) {
                        V = End;
                        break;
                    }
                    getword(&word, &i, lst);
                    tree next_cmd = make_cmd();
                    add_arg(next_cmd, word);
                    
                    cur_cmd->next = next_cmd;
                    cur_cmd->type = OR;
                    prev_cmd = cur_cmd;
                    cur_cmd = next_cmd;
                    
                    if (getword(&word, &i, lst)) {
                        V = End;
                    } else {
                        V = Conv;
                    }
                }
                else if (strcmp(word, "&&") == 0) {
                    if (i >= lst.size) {
                        V = End;
                        break;
                    }
                    getword(&word, &i, lst);
                    tree next_cmd = make_cmd();
                    add_arg(next_cmd, word);
                    
                    cur_cmd->next = next_cmd;
                    cur_cmd->type = AND;
                    prev_cmd = cur_cmd;
                    cur_cmd = next_cmd;
                    
                    if (getword(&word, &i, lst)) {
                        V = End;
                    } else {
                        V = Conv;
                    }
                }
                else if (strcmp(word, "&") == 0) {
                cur_cmd->backgrnd = 1;
                if (getword(&word, &i, lst))
                    V = End;
                else 
                    V = Conv;
                }
                else {
                    add_arg(cur_cmd, word);
                    if (i >= lst.size) {
                        V = End;
                        break;
                    }
                    getword(&word, &i, lst);
                    V = Conv;
                }
                    break;
            case Conv1:
                if (word == NULL) error();
                cur_cmd = make_cmd();
                add_arg(cur_cmd, word);
                prev_cmd->pipe = cur_cmd;
                prev_cmd = cur_cmd;
                
                if (getword(&word, &i, lst)) {
                    V = End;
                } else {
                    V = Conv;
                }         
                    break;
            case In:
                cur_cmd->infile = strdup(word);
                if (cur_cmd->infile == NULL)
                    error();
                V = In1;
                    break;
            case In1:
                if (i >= lst.size) {
                    V = End;
                    break;
                }
                getword(&word, &i, lst);
                V = Conv;
                        break;
            case Out:
                cur_cmd->outfile = strdup(word);
                if (cur_cmd->outfile == NULL) {
                    error();
                }
                V = Out1;
                break;
                
            case Out1:
                if (getword(&word, &i, lst)) {
                    V = End;
                } else {
                    V = Conv;
                }
                break;
                
            case Backgrnd:
                cur_cmd->backgrnd = 1;
                if (word == NULL)
                    V = End;
                else
                    V = Conv;
                break;
                
            case End:
                return beg_cmd;
        
        }
}


int inv(){
    printf("%s", "\x1b[32m");
    char s[100];
    gethostname(s, 100);
    printf("%s@%s", getenv("USER"), s);
    printf("%s", "\x1B[37m"); 
    getcwd(s, 100);
    printf(":%s$ ", s);
    return 1;
}


int main(){
    list lst;
    initList(&lst);
    while(inv() && input(&lst)){
        insertVariables(&lst);
        tree tr = build_tree(lst);
        print_tree(tr, 4);
        printList(&lst);
        clearList(&lst);
        clear_tree(tr);
    }
    return 0;
}