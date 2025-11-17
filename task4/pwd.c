#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(){
    char *path = getcwd(NULL, 0);
    printf("%s\n", path);
    free(path);
}
