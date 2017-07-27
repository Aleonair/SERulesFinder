#include <iostream>
#include <string>
#include <fstream>
#include <set>

using namespace std;

string ReadFromFile, Rule, Scontext, Tcontext, Tclass, ForCat;
ifstream Input;
ofstream Output;
int TmpBegin, TmpEnd, NumOfFile = 0;
set <string> Cat;

int FindDeniedType(){
    TmpBegin = ReadFromFile.find("denied { ") + 9;
    TmpEnd = ReadFromFile.find(" }");
    for(int i = TmpBegin; i < TmpEnd; i++){
        Rule = Rule + ReadFromFile[i];
    }
    return 0;
}

int FindSCType(){
    if(ReadFromFile.find("scontext=u:r:") != - 1.0){
        TmpBegin = ReadFromFile.find("scontext=u:r:") + 13;
    }else{
        TmpBegin = ReadFromFile.find("scontext=u:object_r:") + 20;
    }
    while(ReadFromFile[TmpBegin] != ':'){
        Scontext = Scontext + ReadFromFile[TmpBegin];
        TmpBegin++;
    }
    return 0;
}

int FindTCType(){
    if(ReadFromFile.find("tcontext=u:r:") != - 1.0){
        TmpBegin = ReadFromFile.find("tcontext=u:r:") + 13;
    }else{
        TmpBegin = ReadFromFile.find("tcontext=u:object_r:") + 20;
    }
    while(ReadFromFile[TmpBegin] != ':'){
        Tcontext = Tcontext + ReadFromFile[TmpBegin];
        TmpBegin++;
    }
    return 0;
}

int FindClassType(){
    TmpBegin = ReadFromFile.find("tclass=") + 7;
    while(ReadFromFile[TmpBegin] != ' '){
        Tclass = Tclass + ReadFromFile[TmpBegin];
        TmpBegin++;
    }
    return 0;
}

int Clean(){
    NumOfFile++;
    Rule = "";
    Scontext = "";
    Tcontext = "";
    Tclass = "";
    return 0;
}

int main()
{
    Input.open("in.txt", ios::in);
    Output.open("out.txt", ios::out);
    while(true){
        getline(Input, ReadFromFile);
        if(ReadFromFile == ""){
            break;
        }
        FindDeniedType();
        FindSCType();
        FindTCType();
        FindClassType();
        ForCat = Rule + Scontext + Tcontext + Tclass;
        if(Cat.find(ForCat) == Cat.end() && Scontext != "untrusted_app"){
            Output << "sepolicy-inject -s " << Scontext << " -t " << Tcontext << " -c " << Tclass <<  " -p " << Rule << " -P sepolicy" << NumOfFile << " -o sepolicy" << NumOfFile + 1 << endl;
            Cat.insert(ForCat);
        }
        Clean();
    }
    Input.close();
    Output.close();
    return 0;
}
