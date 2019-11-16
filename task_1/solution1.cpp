#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
using namespace std;

int main() {
    while(true) {
        string command;
        cout << "enter command: ";
        getline(cin, command);

        if(command == "exit")
            return 0;

        int pid = fork();
        if(pid) {
            int status;
            waitpid(pid, &status, 0);
            cout << "status: " << WEXITSTATUS(status) << endl;
        }
        else {
            vector<char*> args;
            command += ' ';
            for(int i = 0; i < command.size() - 1; i = command.find(' ', i) + 1)
                args.push_back(strdup(command.substr(i, command.find(' ', i) - i).c_str()));
            args.push_back(nullptr);
            if(execvp(args[0], &args[0]) == -1)
                cout << "execvp error" << endl;
            return 0;
        }
    }
}
