#include<stdio.h>

int main(int argc, char *argv[]){
    for(int i = 1; i < argc; i++){
        printf((i!=argc-1)?"%s ":"%s\n", argv[i]);
    }
}
