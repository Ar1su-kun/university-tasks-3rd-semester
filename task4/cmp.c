#include<stdio.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>

int main(int argc, char *argv[]){
    char ch1, ch2;
    ssize_t n;
    ssize_t m;
    struct stat s1, s2;
    if (argc > 3){
        fprintf(stderr, "Usage: cp file_path new_file_path\n");
        return -1;
    }
    if (strcmp(argv[1], argv[2]) == 0){
        fprintf(stderr, "Error: '%s' and '%s' are the same file\n", argv[1], argv[2]);
        return -1;
    }
    int fd1, fd2;
    if ((fd1 = open(argv[1], O_RDONLY)) == -1){
        fprintf(stderr, "Error: can't open the file %s\n", argv[1]);
        return -1;
    }
    if((fd2 = open(argv[2], O_RDONLY)) == -1){
        fprintf(stderr, "Error: can't open the file %s\n", argv[2]);
        return -1;
    }

    int lineN = 1;
    int charN = 1;
    while (((n = read(fd1, &ch1, 1)) > 0) &&
            (m = read(fd2, &ch2, 1)) > 0){

        if (ch1 == '\n'){
            charN = 1;
            lineN++;
        }
        else
            charN++;

        if(ch1 != ch2){
            printf("%s differs from %s: line %d char %d\n",
                    argv[1], argv[2], lineN, charN);
            return 0;
        }
    }
    if (stat(argv[1], &s1) == -1 || stat(argv[2], &s2) == -1) {
        fprintf(stderr, "Error: can't get file stats\n");
        close(fd1);
        close(fd2);
        return -1;
    }
    if (s1.st_size < s2.st_size){
        printf("%s differs from %s: files have different sizes\n",
                argv[1], argv[2]);
        close(fd1);
        close(fd2);
        return 0;
    }
    
    printf("%s equal to %s\n", argv[1], argv[2]);
    close(fd1);
    close(fd2);
    return 0;
}
