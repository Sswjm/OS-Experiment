#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<wait.h>
#include<error.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stack>
#include<vector>
#include<queue>

using namespace std;

const int total_instruction = 12;
const int actual_space = 3;
int Access_Series[12];

typedef struct frame{
    int pageID = -1;
    bool flag = false;
}Frame;

void LRU()
{
    float diseffect = 0.0;		// pages absence counts
	float missing_rate = 0.0;	// missing_rate = diseffect / total_instruction
	float success_rate = 0.0;

    Frame frame[actual_space];  //use array simulate stack
    cout << "LRU Algorithm: " << endl;

    for(int i = 0;i < total_instruction;i++){
        bool is_exist = false; //judge if page exists in memory
        for(int j = 0;j < actual_space;j++){
            if((frame[j].flag == true) && (frame[j].pageID == Access_Series[i])){    //page exists
                is_exist = true;

                //put current page to the top of the stack
                for(int cur = j;cur < actual_space-1;cur++){
                    Frame temp;
                    temp.pageID = frame[cur].pageID;
                    temp.flag = true;

                    if(frame[cur+1].flag == false){  //current page is already at the top of the stack
                        break;
                    }
                    else{  //change position
                        frame[cur].pageID = frame[cur+1].pageID;
                        frame[cur+1].pageID = temp.pageID;
                        frame[cur+1].flag = temp.flag;
                    }
                }
                break;
            }
        }


        if(!is_exist){  //page does not exist in the memory
            diseffect += 1.0;
            int k = 0;

            for(k = 0;k < actual_space;k++){
                if(frame[k].flag == false){  //frame has empty space(not full)
                    frame[k].flag = true;
                    frame[k].pageID = Access_Series[i];
                    break;
                }
            }

            if(k == actual_space){  //frame is full(no empty space)
                for(int num = 0;num < actual_space-1;num++){  //move the bottom page of the stack
                    frame[num].pageID = frame[num+1].pageID;
                }
                frame[actual_space-1].pageID = Access_Series[i]; 
            }
        }

        cout << "Pages in Memory,Instruction " << i << ":" << endl;
        for(int j = 0;j < actual_space;j++){
            if(frame[j].pageID == -1){
                cout << "_ "; 
            }
            else{
                cout << frame[j].pageID << " ";
            }
        }
        cout << endl;

    }

    missing_rate = diseffect / total_instruction;
    success_rate = 1 - missing_rate;
    cout << "Success Rate: " << success_rate << "  Missing Rate: " << missing_rate << endl; 
}

void FIFO()
{
    float diseffect = 0.0;		// pages absence counts
	float missing_rate = 0.0;	// missing_rate = diseffect / total_instruction
	float success_rate = 0.0;

    Frame frame[actual_space];  //use array simulate queue
    cout << "FIFO Algorithm: " << endl;

    for(int i = 0;i < total_instruction;i++){
        bool is_exist = false;    //judge if page exists in memory
        for(int j = 0;j < actual_space;j++){
            if((frame[j].flag == true) && (frame[j].pageID == Access_Series[i])){  //page exists
                is_exist = true;
                break;
            }
        }

        if(!is_exist){  //page does not exist in memory
            diseffect += 1.0;
            int k = 0;

            for(k = 0;k < actual_space;k++){  //frame has empty space(not full)
                if(frame[k].flag == false){
                    frame[k].flag = true;
                    frame[k].pageID = Access_Series[i];
                    break;
                }
            }

            if(k == actual_space){  //frame is full(no empty space)
                for(int num = 0;num < actual_space;num++){
                    frame[num].pageID = frame[num+1].pageID;
                }
                frame[actual_space-1].pageID = Access_Series[i]; 
            }
        }

        cout << "Pages in Memory,Instruction " << i << ":" << endl;
        for(int j = 0;j < actual_space;j++){
            if(frame[j].pageID == -1){
                cout << "_ "; 
            }
            else{
                cout << frame[j].pageID << " ";
            }
        }
        cout << endl;
    }

    missing_rate = diseffect / total_instruction;
    success_rate = 1 - missing_rate;
    cout << "Success Rate: " << success_rate << "  Missing Rate: " << missing_rate << endl; 
}

void Clock()
{
    return;
}

void run()
{
    //create page queue
    srand((int)time(NULL));
    cout << "Page queue:";
    for(int i = 0;i<12;i++)
    {
     Access_Series[i] = (int)rand()%5+1;
     cout << Access_Series[i] << ";";
    }
    cout << endl;

    //create subprocess
    int fd[2];
    pid_t pid_1, pid_2, pid_3, pid_4;

    if((pid_1 = fork()) == 0){
        if((pid_2 = fork()) == 0){
            LRU();
            sleep(1);
            exit(0);
        }
        else{
            if((pid_3 = fork()) == 0){
                FIFO();
                sleep(1);
                exit(0);
            }
            else{
                Clock();
                sleep(1);
                exit(0);
            }
        }
    }
    else{
        exit(0);
    }

}

int main()
{
    run();

    return 0;
}