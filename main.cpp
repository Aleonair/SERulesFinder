#include <iostream>
#include <string>
#include <fstream>
#include <set>

using namespace std;

string READ_FROM_FILE, DENIED_ARG, SCONTEXT, TCONTEXT, CLASS_TYPE, FOR_DTB;
ifstream INPUT_FILE;
ofstream OUTPUT_FILE;
int TMP_BEGIN, TMP_END, CURRENT_FILE = 0;
set <string> DTB;

int CLEAN_STRINGS(){
    DENIED_ARG = "";
    SCONTEXT = "";
    TCONTEXT = "";
    CLASS_TYPE = "";
    return 0;
}

int FIND_DENIED_ARG(){
    TMP_BEGIN = READ_FROM_FILE.find("denied") + 9;
    TMP_END = READ_FROM_FILE.find(" }");
    for(int i = TMP_BEGIN; i < TMP_END; i++){
        DENIED_ARG = DENIED_ARG + READ_FROM_FILE[i];
    }
    return 0;
}

int FIND_SCONTEXT(){
    if(READ_FROM_FILE.find("scontext=u:r:") != - 1){
        TMP_BEGIN = READ_FROM_FILE.find("scontext=u:r:") + 13;
    }else{
        TMP_BEGIN = READ_FROM_FILE.find("scontext=u:object_r:") + 20;
    }
    while(READ_FROM_FILE[TMP_BEGIN] != ':'){
        SCONTEXT = SCONTEXT + READ_FROM_FILE[TMP_BEGIN];
        TMP_BEGIN++;
    }
    return 0;
}

int FIND_TCONTEXT(){
    if(READ_FROM_FILE.find("tcontext=u:r:") != - 1){
        TMP_BEGIN = READ_FROM_FILE.find("tcontext=u:r:") + 13;
    }else{
        TMP_BEGIN = READ_FROM_FILE.find("tcontext=u:object_r:") + 20;
    }
    while(READ_FROM_FILE[TMP_BEGIN] != ':'){
        TCONTEXT = TCONTEXT + READ_FROM_FILE[TMP_BEGIN];
        TMP_BEGIN++;
    }
    return 0;
}

int FIND_CLASS_TYPE(){
    TMP_BEGIN = READ_FROM_FILE.find("tclass=") + 7;
    while(READ_FROM_FILE[TMP_BEGIN] != ' '){
        CLASS_TYPE = CLASS_TYPE + READ_FROM_FILE[TMP_BEGIN];
        TMP_BEGIN++;
    }
    return 0;
}

int main()
{
    INPUT_FILE.open("in.txt", ios::in);
    OUTPUT_FILE.open("out.txt", ios::out);
    while(true){
        getline(INPUT_FILE, READ_FROM_FILE);
        if(READ_FROM_FILE == ""){
            break;
        }
        FIND_DENIED_ARG();
        FIND_SCONTEXT();
        FIND_TCONTEXT();
        FIND_CLASS_TYPE();
        FOR_DTB = DENIED_ARG + SCONTEXT + TCONTEXT + CLASS_TYPE;
        if(DTB.find(FOR_DTB) == DTB.end() && SCONTEXT != "untrusted_app"){
            OUTPUT_FILE << "sepolicy-inject -s " << SCONTEXT << " -t " << TCONTEXT << " -c " << CLASS_TYPE << " -p " << DENIED_ARG << " -P sepolicy" << CURRENT_FILE << " -o sepolicy" << CURRENT_FILE + 1 << endl;
            DTB.insert(FOR_DTB);
            CURRENT_FILE++;
        }
        CLEAN_STRINGS();
    }
    INPUT_FILE.close();
    OUTPUT_FILE.close();
    return 0;
}
