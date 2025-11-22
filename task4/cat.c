#include<stdio.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>

int main(int argc, char *argv[]){
    char ch;
    ssize_t n;
    int fd;

    int isN = 0;
    int isNext = 0;
    int i = 1;
    if ((argc > 1) && (argv[1][0] == '-') && (strlen(argv[1]) == 2)){
        isN = (argv[1][1] == 'n') ? 1 : isN;
        i++;
        if (!isN){
            fprintf(stderr, "Error: unknown argument %s", argv[1]);
        }
    }
    int count = 1;
    for(; i < argc; i++){
        if ((fd = open(argv[i], O_RDONLY)) == -1){
            fprintf(stderr, "Error: can't open the file %s\n", argv[1]);
            return -1;
        }
        if (isN && count == 1)
            printf("\t%d  ", count);
        while ((n = read(fd, &ch, 1)) > 0){
            if (isN){
                if (isNext){
                    isNext = 0;
                    printf("\n\t%d  ", count);
                }
                if (ch == '\n'){
                    count++;
                    isNext = 1;
                }

                else{
                    printf("%c", ch);
                }
            }
            else{
                printf("%c", ch);
            }
        }
        close(fd);
    }
    return 0;
}
