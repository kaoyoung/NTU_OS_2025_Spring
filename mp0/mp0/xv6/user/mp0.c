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


void traverse(char *path, char *target, int *num_dir, int *num_file, int *is_root){
    int fd = 0;
    struct stat st;
    if((fd = open(path, 0)) < 0){
      printf("%s [error opening dir]\n", path);
      exit(1);
    }

    if(fstat(fd, &st) < 0){
      printf("%s [error opening dir]\n", path);
      close(fd);
      exit(1);
    }

    switch(st.type){
    case T_FILE:
      if(*is_root == 1){
        printf("%s [error opening dir]\n", path);
      }
      else{
        ++(*num_file);
        printf("%s %d\n", path, char_occurance(path, target));
      }      
      *is_root = 0;
      break;

    case T_DIR:
      (*num_dir) = (*is_root == 1)? 0:(*num_dir+1);
      *is_root = 0;
      printf("%s %d\n", path, char_occurance(path, target));

      char buf[MAXLENGTH];
      char *p = buf;
      strcpy(buf, path);
      p += strlen(path);
      *p = '/';
      ++p;
      
      struct dirent de;
      while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if(de.inum == 0)
          continue;
        if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
          continue;

        if(strlen(buf) + 1 + DIRSIZ + 1 > MAXLENGTH){
          printf("the length of name is too long\n");
          exit(1);
        }
        memset(p, ' ', DIRSIZ);
        memmove(p, de.name, DIRSIZ);        
        traverse(buf, target, num_dir, num_file, is_root);
      }
      break;
    
    default:
      printf("It's an illegal st.type\n");
      break;
    }

    close(fd);
    return;
}


int main(int argc, char *argv[]) {
  if(argc != 3) {
    printf("Usage: %s <root_directory> <key>\n", argv[0]);
    exit(1);
  }
  
  int pipe_fd[2];
  if(pipe(pipe_fd) < 0){
    printf("pipe fail\n");
    exit(1);
  }

  int pid = fork();
  if(pid < 0){
    printf("fork fail\n");
    exit(1);
  }
  else if(pid >0){
    close(pipe_fd[1]);
    int num_dir = 0;
    int num_file = 0;   
    read(pipe_fd[0], &num_dir, sizeof(num_dir));
    read(pipe_fd[0], &num_file, sizeof(num_file));
    wait(0);

    printf("\n");
    printf("%d directories, %d files\n", num_dir, num_file);
    close(pipe_fd[0]);
  }
  else{
    close(pipe_fd[0]);
    int num_dir = 0;
    int num_file = 0;
    int is_root = 1;
    traverse(argv[1], argv[2], &num_dir, &num_file, &is_root);

    write(pipe_fd[1], &num_dir, sizeof(num_dir));
    write(pipe_fd[1], &num_file, sizeof(num_file));
    close(pipe_fd[1]);
  }
  exit(0);
}