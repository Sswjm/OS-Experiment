#include <iostream>
#include<stdlib.h>
#include<queue>
#include<vector>

using namespace std;

typedef struct pcb {
    int pid;

} pcb;

queue<pcb> ready, running, block;

void createProgress()
{
    int n;
    cout << "Please enter the number of progress:";
    cin >> n;
    for (int i = 0; i < n; i++) {
        pcb temp;
        cout << "Please enter progress " << i+1 << endl;
        cin >> temp.pid;
        ready.push(temp);
    }
}
void showQueue()
{
    cout << "Ready Queue:";
    for (int i = 0; i < ready.size(); i++) {
        //cout << ready[i].pid << "\t";
        cout << ready.front().pid << "\t";
        ready.push(ready.front());
        ready.pop();
    }
    cout << endl;

    cout << "Running Queue:";
    for (int i = 0; i < running.size(); i++) {
        //cout << running[i].pid << "\t";
        cout << running.front().pid << "\t";
        running.push(running.front());
        running.pop();
    }
    cout << endl;

    cout << "Block Queue:";
    for (int i = 0; i < block.size(); i++) {
        //cout << block[i].pid << "\t";
        cout << block.front().pid << "\t";
        block.push(block.front());
        block.pop();
    }
    cout << endl;
}

void dispatch()
{
    pcb temp = ready.front();
    ready.pop();
    running.push(temp);
}

void timeout()
{
    pcb temp = running.front();
    running.pop();
    ready.push(temp);
}

void eventWait()
{
    pcb temp = running.front();
    running.pop();
    block.push(temp);
}

void eventOccur()
{
    pcb temp = block.front();
    block.pop();
    ready.push(temp);
}

void run()
{
    cout << "Please choose your action: 0.exit 1.dispatch 2.timeout 3.event wait 4.event occur" << endl;
    int action;
    while (cin >> action) {
        if (action == 0) {
            break;
        }
        else if (action == 1) {
            if (!ready.empty()) {     //ready非空，执行dispatch
                dispatch();
            }

            showQueue();
        }
        else if (action == 2) {
            if (!running.empty()) {  //running非空，执行timeout，timeout必然引起dispatcjh
                if (!ready.empty()) {
                    timeout();
                    dispatch();
                }
            }

            showQueue();
        }
        else if (action == 3) {  //running非空，执行event wait，如果存在就绪进程，执行dispatch
            if (!running.empty()) {
                eventWait();
                if (!ready.empty()) {
                    dispatch();
                }
            }

            showQueue();
        }
        else if (action == 4) {  //block非空，执行event occur，如果无就绪进程，执行dispatch
            if (!block.empty()) {
                if (ready.empty()) {
                    eventOccur();
                    dispatch();
                }
                else {
                    eventOccur();
                }
            }

            showQueue();
        }
        else {
            cout << "Wrong, please enter again" << endl;
        }
    }
}

int main()
{
    createProgress();
    showQueue();
    run();

    return 0;
}