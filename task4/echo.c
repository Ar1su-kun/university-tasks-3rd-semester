#include<stdio.h>
#include<string.h>

int main(int argc, char *argv[]){
    int i = 1;
    char *spec = "%s ";
    char *specEnd = "%s\n";
    if ((argc > 1) && (argv[1][0] == '-') && (strlen(argv[1]) == 2)){
        switch (argv[1][1]){
            case 'n':
                i++;
                specEnd = "%s\n";
                break;
            case 's':
                i++;
                spec = "%s";
                break;
        }
    }
    for(; i < argc; i++){
        printf((i!=argc-1)?spec:specEnd, argv[i]);
    }
}
