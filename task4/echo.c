#include<stdio.h>
#include<string.h>

int main(int argc, char *argv[]){
    int i = 1;
    char spec[3] = "%s ";
    char specEnd[4] = "%s\n";
    if ((argc > 1) && (argv[1][0] == '-') && (strlen(argv[0]) == 2)){
        switch (argv[1][1]){
            case 'n':
                i++;
                specEnd[3] = '\0';
                break;
            case 's':
                i++;
                spec[2] = '\0';
                break;
        }
    }
    for(; i < argc; i++){
        printf((i!=argc-1)?spec:specEnd, argv[i]);
    }
}
