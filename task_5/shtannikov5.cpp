#include <csignal>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <string>
#include <sys/wait.h>
using namespace std;

vector<bool> bits;
int ppid, cpid;
void bit0(int n) {
    bits.push_back(0);
    kill(ppid, SIGUSR1);
}
void bit1(int n) {
    bits.push_back(1);
    kill(ppid, SIGUSR1);
}
void end(int n) {
    for(int i = 0; i < bits.size(); i += 8) {
        char s = 0;
        for(int j = 0; j < 8; j++)
            s |= bits[i + j] << j;
        cout << s;
    }
    exit(0);
}
bool flag = false;
void setflag(int n) {
    flag = true;
}
int main() {
    signal(SIGUSR1, setflag);
    ppid = getpid();
    cpid = fork();
    if(cpid) {
        string message = "this is a test message, how are you today?\n";
        for(int i = 0; i < message.size(); i++)
            for(int j = 0; j < 8; j++) {
                while(!flag) usleep(1);
                flag = false;
                if(message[i] & (1 << j))
                    kill(cpid, SIGUSR2);
                else
                    kill(cpid, SIGUSR1);
            }
        kill(cpid, SIGINT);
    }
    else {
        signal(SIGUSR1, bit0);
        signal(SIGUSR2, bit1);
        signal(SIGINT, end);
        kill(ppid, SIGUSR1);
        while(true) sleep(1);
    }
}