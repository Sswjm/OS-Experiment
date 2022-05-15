#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<wait.h>
#include<error.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<cstring>

using namespace std;

void run()
{
    int fd[2];
    pid_t pid;
    int flag;

    flag = pipe(fd);
    if(flag == -1){
        cout << "Cannot create pipe" << endl;
        exit(-1);
    }
    for(int i = 0;i < 3;i++){
        pid = fork();
        char message[40];
        sprintf(message, "Child process %d is sending a message", i+1);
        
        if(pid == -1){
            cout<<"Create Process Failed"<<endl;
            exit(0);
        }
        else if(pid == 0){  //child process
            lockf(fd[1], 1, 0);
            write(fd[1], message, sizeof(message));
            sleep(1);
            lockf(fd[1], 0, 0);

            exit(0);
        }
        else if(pid > 0){
            wait(0);
            
            char outpipe[40];
            read(fd[0], outpipe, sizeof(outpipe));
            cout << outpipe << endl;
        }
    }
}

int main()
{
    run();

    return 0;
}