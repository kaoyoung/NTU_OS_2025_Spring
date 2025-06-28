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


void traverse(char *path, char *target, int *p_num_file, int *p_num_dir)
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
    *p_num_file +=1;
    printf("%s %u\n", path, char_occurance(path, target));
    break;

  case T_DIR:
    *p_num_dir +=1;
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
        traverse(path, target, p_num_file, p_num_dir);
    }

    break;
  }

  close(fd);
}

int main(int argc, char *argv[])
{
    int fd[2];
    int pipe(fd);
    int pid = fork();
    int *pc_num_file;
    int *pc_num_dir;

    if(pid > 0){
      close(fd[1]);

      int *pp_num_file;
      int *pp_num_dir;
      read(fd[0], pp_num_file, 12);
      read(fd[0], pp_num_dir, 12);

      printf("%d directories, %d files\n");
    }
    else{
      close(fd[0]);

      int num_file = 0;
      int *pc_num_file = &num_file;
      int num_dir = 0;
      int *pc_num_dir = &num_dir;
      traverse(argv[1], argv[2], pc_num_file, pc_num_dir);      

      write(fd[1], pc_num_file, 12);
      write(fd[1], pc_num_dir, 12);
      
    }

    exit(0);  
}
