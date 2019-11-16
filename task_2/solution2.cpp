#include <iostream>
#include <string.h>
#include <unistd.h>
using namespace std;

struct dpipe_t {
    FILE* f[2][2];
    int fd[2][2];
    dpipe_t() {
        pipe(fd[0]);
        pipe(fd[1]);
    }
    void child_init() {
        close(fd[0][0]);
        close(fd[1][1]);
        f[1][0] = fdopen(fd[1][0], "r");
        f[0][1] = fdopen(fd[0][1], "w");
    }
    void parent_init() {
        close(fd[1][0]);
        close(fd[0][1]);
        f[0][0] = fdopen(fd[0][0], "r");
        f[1][1] = fdopen(fd[1][1], "w");
    }
};


int main() {
    dpipe_t dpipe;
    char s[4096];
    int pid = fork();
    if(pid) {
        dpipe.parent_init();
        while(true) {
            printf("message: ");
            scanf("%s", s);
            if(string(s) == "exit")
                return 0;
            dprintf(dpipe.fd[1][1], "%s\n", s);
            fscanf(dpipe.f[0][0], "%s", s);
            printf("response: %s\n", s);
        }
    } else {
        dpipe.child_init();
        while(true) {
            fscanf(dpipe.f[1][0],"%s", s);
            dprintf(dpipe.fd[0][1],"child_recieved_message_%s_with_length_%d\n", s, (int)strlen(s));
        }
    }
}
