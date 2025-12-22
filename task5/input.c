#include "input.h"


list *lst;
buffer buf;
int c;

int isInQuotes = 0;

int inSpecSet2(int c){
    return  c=='|'  ||
            c == '&'||
            c == '>';
}

int inSpecSet1(int c){
    return inSpecSet2(c) ||
        c==';' ||
        c=='<' ||
        c=='(' ||
        c==')';
}

int inSymSet(int c){
    return !inSpecSet1(c) &&
        c!=' '  &&
        c!='\t' &&
        c!='\n' &&
        c!=EOF;
}

void word();
void specWord1(int prev);
void specWord2();

void start(){
    if(c == '"'){
        isInQuotes = !isInQuotes;
    }
    if(c == ' ' || c == '\t'){
        c = getchar();
        start();
    }
    else if (c == '\n' || c == EOF){
        clearBuffer(&buf);
        shrinkList(lst);
    }
    else if (isInQuotes || inSymSet(c)){
        clearBuffer(&buf);
        addSymbol(&buf, c);
        c = getchar();
        word();
    }
    else if (inSpecSet1(c)){
        clearBuffer(&buf);
        addSymbol(&buf, c);
        int prev = c;
        c = getchar();
        specWord1(prev);
    }
}

void specWord1(int prev){
    if (c == ' ' || c == '\t' || inSymSet(c) ||
        (!inSpecSet2(c)&&inSpecSet1(c)) || (inSpecSet2(c)&&(c!=prev))){

        addWord(lst, &buf);
        start();
    }
    else if (c == '\n' || c == EOF){
        addWord(lst, &buf);
        start();
    }
    else if (inSpecSet2(c) && (c == prev)){
        addSymbol(&buf, c);
        specWord2();
    }
}

void specWord2(){
    if (c == '\n' || c == EOF){
        addWord(lst, &buf);
        start();
    }
    else{
        addWord(lst, &buf);
        c = getchar();
        start();
    }
}

void word(){
    if (c == '"'){
        if (!isInQuotes){
            isInQuotes = !isInQuotes;
            addWord(lst, &buf);
            clearBuffer(&buf);
            addSymbol(&buf, c);
            c = getchar();
            word();
        }
        else{
            isInQuotes = !isInQuotes;
            addSymbol(&buf, c);
            addWord(lst, &buf);
            c = getchar();
            start();
        }
}
    else if(isInQuotes && (c != '\n') && (c != EOF)){
        addSymbol(&buf, c);
        c = getchar();
        word();
    }
    else if (c == '$' ||c == '\n' || c == EOF || inSpecSet1(c)){
        addWord(lst, &buf);
        start();
    }
    else if (inSymSet(c)){
        addSymbol(&buf, c);
        c = getchar();
        word();
    }
    else if (c == ' ' || c == '\t'){
        c = getchar();
        addWord(lst, &buf);
        start();
    }
}

int input(list *inputList){
    c = getchar();
    if (c == EOF){
        return 0;
    }
    else{
        lst = inputList;
        initList(lst);
        initBuffer(&buf);
        start();
        return 1;
    }
}
