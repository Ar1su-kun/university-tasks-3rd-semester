#include<stdio.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>

int main(int argc, char *argv[]){
    char buf[1024];
    ssize_t n;
    struct stat s;
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
    stat(argv[1], &s);
    if((fd2 = creat(argv[2], s.st_mode)) == -1){
        fprintf(stderr, "Error: can't create the file %s\n", argv[2]);
        return -1;
    }
    while ((n = read(fd1, buf, sizeof(buf))) > 0)
        write(fd2, buf, n);
    return 0;
}
