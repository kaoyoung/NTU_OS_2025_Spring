#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#define MAXLENGTH 512

int char_occurance(char *path, char *target){
    int occurance = 0;
    for(int i=0; i<strlen(path); ++i){
        if(path[i]==*target){
            ++occurance;
        }
    }
    return occurance;
}


void traverse(char *path, char *target, int *num_dir, int *num_file){
    int fd = 0;
    struct dirent de;
    struct stat st;
    char buf[MAXLENGTH];
    char *p;

    if((fd = open(path, 0)) <0){
      fprintf(2, "%s [error opening dir]\n", path);
      exit(1);
    }

    if(fstat(fd, &st) < 0){
      fprintf(2, "%s [error opening dir]\n", path);
      close(fd);
      exit(1);
    }

    switch(st.type){
      case T_FILE:
        ++*num_file;
        printf("%s %d\n", path, char_occurance(path, target));
        break;
      case T_DIR:
        ++*num_dir;
        if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
          printf("ls: path too long\n");
          break;
        }

        strcpy(buf, path);
        p = buf+strlen(buf);
        printf("%s %d\n", buf, char_occurance(buf, target));
        *p = '/';
        ++p;
    
        while(read(fd, &de, sizeof(de)) == sizeof(de)){
            if(de.inum == 0)
                continue;
            if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
                continue;

            memset(p, ' ', DIRSIZ);
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            if(stat(buf, &st) < 0){
                printf("ls: cannot stat %s\n", buf);
                continue;
            }    
            traverse(buf, target, num_dir, num_file);
        }
        break;
    }
    close(fd);
}


int main(int argc, char *argv[]) {
  if(argc != 3) {
    printf("Usage: %s <root_directory> <key>\n", argv[0]);
    exit(1);
  }
  
  int p[2];
  if(pipe(p) < 0) {
    printf("Error: pipe failed\n");
    exit(1);
  }

  int pid = fork();
  if(pid < 0) {
    printf("Error: fork failed\n");
    exit(1);
  } 
  else if(pid > 0) {
    close(p[1]);
    int dir_count, file_count;
    read(p[0], &dir_count, sizeof(dir_count));
    read(p[0], &file_count, sizeof(file_count));
    wait((int *)0);
    printf("\n");   
    printf("%d directories, %d files\n", dir_count, file_count);
    close(p[0]);  
  } 
  else {  
    close(p[0]);
    int dir_num = 0;
    int file_num = 0;    
    traverse(argv[1], argv[2], &dir_num, &file_num);

    --dir_num;
    dir_num = (dir_num<0)? 0:dir_num;
    write(p[1], &dir_num, sizeof(dir_num));
    write(p[1], &file_num, sizeof(file_num));    
    close(p[1]); 
    
    exit(0);    
  }
  exit(0); 
}