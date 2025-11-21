#include<stdio.h>
#include<pwd.h>
#include<dirent.h>
#include<string.h>
#include<stdlib.h>

void output(int isR, int isG, int isL, const char *path);

int main(int argc, char* argv[]){
    int isR = 0;
    int isG = 0;
    int isL = 0;

    int i = 1;
    if ((argc > 1) && (argv[1][0] == '-') && (strlen(argv[1]) == 2)){
        isR = (argv[1][1] == 'R') ? 1 : 0;
        isL = (argv[1][1] == 'l') ? 1 : 0;
        isG = (argv[1][1] == 'G') ? 1 : 0;
        if (!(isR || isL || isG)){
            fprintf(stderr, "Error: unknown argument %s", argv[1]);
            exit(-1);
        }
        i++;
    }
    if (argc == i){
        output(isR, isG, isL, ".");
    }
    for(; i < argc; i++){
        output(isR, isG, isL, argv[i]);
    }
}


void output(int isR, int isG, int isL, const char *path){
    
    DIR *dir;
    struct dirent *entry;
    dir = opendir(path);

    if (dir == NULL){
        fprintf(stderr, "Error opening dir");
        exit(-1);
    }
    
    while ((entry = readdir(dir)) != NULL){
        if (entry->d_name[0] != '.'){
            if (entry->d_type != DT_DIR){
                printf("%s ", entry->d_name);
            }
            else{
                printf("\033[32m%s\033[0m ", entry->d_name);
            }
        }
    }
    printf("\n");
    if (closedir(dir) == -1){
        fprintf(stderr, "Error closing dir");
    }

    if (isR){
        printf("\n");
        dir = opendir(path);
        while ((entry = readdir(dir)) != NULL){
            if (entry->d_type == DT_DIR && entry->d_name[0] != '.'){
                char newpath[PATH_MAX];
                snprintf(newpath, sizeof(newpath), "%s/%s", path, entry->d_name);
                printf("%s:\n", newpath);
                output(isR, isG, isL, newpath);
            }
        }
        if (closedir(dir) == -1){
            fprintf(stderr, "Error closing dir");
        }
    }
}
