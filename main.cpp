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

int main(){
    
    string prompt = "[cmd]:";
    vector<string>args;
    vector<string>history;
    string test = "";
    string helpText = "That command was not found";
    chrono::duration<double> time1;
    chrono::duration<double> totaltime;
    
    cout<<prompt;
    getline(cin, test);
    history.push_back(test);
    
    while(test.substr(0, 4) != "exit"){
        if(fork()){
            chrono::time_point<chrono::high_resolution_clock> start = chrono::high_resolution_clock::now();
            wait(NULL);
            chrono::time_point<chrono::high_resolution_clock> end = chrono::high_resolution_clock::now();
            time1 = end - start;
            totaltime += time1;
        }
        else{
            args = splitString(test, ' ');
            childexec(args);
            if(args[0] == "ptime"){
                cout<<"The total time spent executing child processes is: "<<totaltime.count()<<" seconds"<<endl;
                return 0;
            }
            else if(args[0] == "history"){
                cout<<"--History--"<<endl<<endl;
                for(int i = 0; i < history.size() - 1; i++){
                    cout<<i+1<<". "<<history[i]<<endl;
                }
                return 0;
            }
            else if(args[0] == "^"){
                int numBack;
                try{
                    numBack = stoi(args[1]);
                }catch(exception& e){
                    cout<<helpText<<endl;
                    return 0;
                }
                if(numBack == 0){
                    cout<<helpText<<endl;
                    return 0;
                }
                else{
                    cout<<history[numBack - 1]<<endl;
                    return 0;
                }
            }
            cout<<helpText<<endl;
            return 0;
        }
        
        cout<<prompt;
        getline(cin, test);
        history.push_back(test);
        
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