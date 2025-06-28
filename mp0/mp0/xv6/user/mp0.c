#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#define Maxlength 512

int char_occurance(char *path, char *target){
    int occurance = 0;
    for(int i=0; i<strlen(path); ++i){
        if(path[i]==*target){
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
    fprintf(2, "%s [error opening dir]\n", path);
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
    printf("%s %d\n", path, char_occurance(path, target));
    break;

  case T_DIR:
    *p_num_dir +=1;
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("ls: path too long\n");
      break;
    }

    strcpy(buf, path);
    p = buf+strlen(buf);
    printf("%s %d\n", buf, char_occurance(buf, target));
    *p++ = '/';
    
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if(de.inum == 0)
            continue;
        if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
            continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if(stat(buf, &st) < 0){
            printf("ls: cannot stat %s\n", buf);
            continue;
        }
        
        traverse(buf, target, p_num_file, p_num_dir);
    }

    break;
  }

  close(fd);
}

int main(int argc, char *argv[])
{
    int pipe_fd[2];
    pipe(pipe_fd);  
    
    if(pipe(pipe_fd) == -1){
        //  perror("pipe");
        exit(1);
    }
    
    char buf[2];
    int num_file = 0;
    int num_dir = 0;
    int pid = fork();
    if(pid < 0){
        //  perror("fork");
        exit(1);
    }
    else if(pid > 0){
        close(pipe_fd[1]);
        char result[2];
        read(pipe_fd[0], result, 2);

        int file_dir = result[0] - '0';
        int file_file = result[1] - '0';
        printf("\n");
        printf("%d directories, %d files\n", file_dir, file_file);
        close(pipe_fd[0]);

        wait(0);
    }
    else{
        close(pipe_fd[0]);
        traverse(argv[1], argv[2], &num_file, &num_dir);    

        num_dir = (num_dir==0)?   1:num_dir;
        buf[0] = (num_dir-1) + '0';
        buf[1] = num_file + '0';
        write(pipe_fd[1], buf, 2);  
      
        close(pipe_fd[1]);
        exit(0);
    }

    return 0; 
}
