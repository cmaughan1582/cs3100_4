#include <iostream>
#include <unistd.h>
#include <cstring>
#include <vector>

int main(){
    char** argv = new char*[3];
    argv[0] = new char[3];
    strcpy(argv[0], "ls");
    argv[1] = new char[4];
    strcpy(argv[1], "-a");
    argv[2] = NULL;
    execvp(argv[0], argv);
    return 0;
    
}