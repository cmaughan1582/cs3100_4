#define USE_MATH_DEFINES

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <vector>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <stdexcept>

using namespace std;

vector<string> splitString(string txt, char space);
void childexec(vector<string> vecs);
void childFunc(string test, vector<string> history, vector<string> args, chrono::duration<double> totaltime, chrono::duration<double> time1);

int main(){
    
    string prompt = "[cmd]:";
    vector<string>args;
    vector<string>history;
    string test = "";
    string helpText = "That command was not found";
    int pushNum = 0;
    chrono::duration<double> time1;
    chrono::duration<double> totaltime;
    
    cout<<prompt;
    getline(cin, test);
    
    while(test.substr(0, 4) != "exit"){
        if(fork()){
            chrono::time_point<chrono::high_resolution_clock> start = chrono::high_resolution_clock::now();
            wait(NULL);
            chrono::time_point<chrono::high_resolution_clock> end = chrono::high_resolution_clock::now();
            time1 = end - start;
            totaltime += time1;
        }
        else{
            childFunc(test, history, args, totaltime, time1);
            return 0;
        }
        
        
        if(test.at(0) == '^'){
            pushNum = 0;
            args = splitString(test, ' ');
            try{
                pushNum = stoi(args[1]);
            }catch(exception& e){
                
            }
            if(pushNum != 0){
                test = history[pushNum - 1];
                history.push_back(test);
            }
        }
        else{
            history.push_back(test);
        }
        cout<<prompt;
        getline(cin, test);
    }

    return 0;
    
}

vector<string> splitString(string str, char space)
{
    int pos = str.find(space);
    int initialPos = 0;
    vector<string> vecs;
    string test = "";
    

    while( pos != string::npos ) {
        test = str.substr(initialPos, pos - initialPos);
        vecs.push_back(test);
        initialPos = pos + 1;

        pos = str.find(space, initialPos);
    }
    test = str.substr(initialPos, str.size()-1 - initialPos + 1);
    vecs.push_back(test);
    return vecs;

}

void childexec(vector<string> vecs){
    char** argv = new char*[vecs.size() + 1];
    for(int i = 0; i < vecs.size(); i++){
        const char* test = vecs[i].c_str();
        argv[i] = new char[vecs[i].length() + 1];
        strcpy(argv[i], test);
    }
    argv[vecs.size()] = NULL;
    execvp(argv[0], argv);
}

void childFunc(string test, vector<string> history, vector<string> args, chrono::duration<double> totaltime, chrono::duration<double> time1){
    args = splitString(test, ' ');
    string helpText1 = "That command was not found";
    if(args[0] == "^"){
        if(history.size() == 0){
            cout<<helpText1<<endl;
            return;
        }
        int numBack;
        try{
            numBack = stoi(args[1]);
        }catch(exception& e){
            cout<<helpText1<<endl;
            return;
        }
        if(numBack == 0){
            cout<<helpText1<<endl;
            return;
        }
        else if(numBack >= history.size()){
            cout<<helpText1<<endl;
            return;
        }
                
        else{
            test = history[numBack - 1];
        }
    }
    args = splitString(test, ' ');
    childexec(args);
    if(args[0] == "ptime"){
        cout<<"The total time spent executing child processes is: "<<totaltime.count()<<" seconds"<<endl;
        cout<<"The time spent executing the last process was: "<<time1.count()<<" seconds"<<endl;
        return;
    }
    else if(args[0] == "history"){
        cout<<"--History--"<<endl<<endl;
        for(int i = 0; i < history.size(); i++){
            cout<<i+1<<". "<<history[i]<<endl;
        }
        return;
    }
    cout<<helpText1<<endl;
    return;
}