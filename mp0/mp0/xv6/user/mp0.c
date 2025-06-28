#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#define Maxlength 512

unsigned char_occurance(char *path, char target){
    unsigned occurance = 0;
    for(int i=0; i<strlen(path); ++i){
        if(path[i]==target){
            ++occurance;
        }
    }
    return occurance;
}


void traverse(char *path, char *target)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    printf("%s %u\n", path, char_occurance(path, target));
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("ls: path too long\n");
      break;
    }

    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if(de.inum == 0)
            continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if(stat(buf, &st) < 0){
            printf("ls: cannot stat %s\n", buf);
            continue;
        }
        printf("%s %u\n", buf, char_occurance(buf, target));
        traverse(path, target);
    }

    break;
  }

  close(fd);
}

int main(int argc, char *argv[])
{
    int pipe(int fd[2]);
    int pid = fork();

    if(pid > 0){

    }
    else{
        traverse(argv[1], argv[2]);        
    }

    exit(0);  
}
