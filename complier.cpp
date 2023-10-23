#include <iostream>
#include <fstream>
using namespace std;

void excute(string mode){
    string line;
    while(line != "q"){
        cout << mode << endl;
        cin >> line;
        cout<<"ok get it!"<<endl;
        cout<<"Anything else?(q to quit)"<<endl;
        cin >> line;
    }
    cout<<"Bye!"<<endl;
}


int main(int argc, char* argv[]){
    fstream script;    

    if(argc < 2){
        cout << "Usage: " << argv[0] << " <script>" << endl;
    }
    else{
        script.open(argv[1], ios::in);
        if(!script.is_open()){
            cout << "Error: " << argv[1] << " not found" << endl;
        }
    }

    string line;
    script>>line;
    while (!script.eof())
    {
        if(line == "name"){
            excute("tell me your name");
        }
        else if(line == "age"){
            excute("tell me your age");
        }
        else if(line == "投诉"){
            excute("请问您的投诉是什么");
        }
        script >> line;
    }

    return 0;
}