#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/sysmacros.h>
#include<pwd.h>
#include<grp.h>
#include<dirent.h>
#include<string.h>
#include<stdlib.h>

void output(int isR, int isG, int isL, const char *path);
void longList(int isG, const char *path);

int main(int argc, char* argv[]){
    int isR = 0;
    int isG = 0;
    int isL = 0;

    int i = 1;
    if ((argc > 1) && (argv[1][0] == '-') && (strlen(argv[1]) >= 2)){
        for(int j = 1;  j < strlen(argv[1]); j++){
            isR = (argv[1][j] == 'R') ? 1 : isR;
            isL = (argv[1][j] == 'l') ? 1 : isL;
            isG = (argv[1][j] == 'g') ? 1 : isG;
            if (!(isR || isL || isG)){
                fprintf(stderr, "Error: unknown argument %s", argv[1]);
                exit(-1);
            }
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
            if (isG || isL){
                char newpath[PATH_MAX];
                snprintf(newpath, sizeof(newpath), "%s/%s", path, entry->d_name);
                longList(isG, newpath);
            }
            if (entry->d_type != DT_DIR){
                printf((isG||isL)?"%s\n":"%s ", entry->d_name);
            }
            else{
                printf((isG||isL)?"\033[32m%s\033[0m\n":"\033[32m%s\033[0m ",
                        entry->d_name);
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


void display_file_type (int st_mode){                                   
    switch (st_mode & S_IFMT)
    {
        case S_IFDIR:  putchar ('d'); return;
        case S_IFCHR:  putchar ('c'); return;
        case S_IFBLK:  putchar ('b'); return;
        case S_IFREG:  putchar ('-'); return;
        case S_IFLNK:  putchar ('l'); return;
        case S_IFSOCK: putchar ('s'); return;
    }
}

void display_permission (int st_mode){
    static const char xtbl[10] = "rwxrwxrwx";
    char     amode[10];
    int      i, j;

    for ( i = 0, j = ( 1 << 8 ); i < 9; i++, j >>= 1 )
        amode[i] = ( st_mode&j ) ? xtbl[i]: '-';

    if ( st_mode & S_ISUID )   amode[2]= 's';
    if ( st_mode & S_ISGID )   amode[5]= 's';
    if ( st_mode & S_ISVTX )   amode[8]= 't';
  amode[9]='\0';
  printf ( "%s ",amode );
}

void longList(int isG, const char *path){
    struct stat     statv;
    struct passwd  *pw_d;
    struct group *gr_d;
 
    if (lstat(path, &statv)){ 
        perror (path); 
        return;
    }

    display_file_type(statv.st_mode);
    display_permission(statv.st_mode);
    printf("%ld ", statv.st_nlink);
    pw_d = getpwuid(statv.st_uid);
    printf("%s ", pw_d->pw_name);
    if (!isG){
        gr_d = getgrgid(statv.st_gid);
        printf("%s ", gr_d->gr_name);
    }
 
    if ((statv.st_mode & S_IFMT) == S_IFCHR  ||
         (statv.st_mode & S_IFMT) == S_IFBLK)
    printf ("%d, %d", major(statv.st_rdev), minor(statv.st_rdev) );
    else
        printf("%ld\t", statv.st_size);

}
