#include "TinyJS.h"
#include "TinyJS_Functions.h"
#include "TinyJS_MathFunctions.h"
#include <assert.h>
#include <stdio.h>
#include <cstring>
#include <iostream>
#include <fstream>
using namespace std;

//const char *code = "var a = 5; if (a==5) a=4; else a=3;";
//const char *code = "{ var a = 4; var b = 1; while (a>0) { b = b * 2; a = a - 1; } var c = 5; }";
//const char *code = "{ var b = 1; for (var i=0;i<4;i=i+1) b = b * 2; }";
const char *code = "function myfunc(x, y)\n { return x + y; } \nvar a = myfunc(1,2); \nprint(a);";

void js_print(Variable *v, void *userdata) {
    printf("> %s\n", v->getParameter("text")->getString().c_str());
}

void js_dump(Variable *v, void *userdata) {
    CTinyJS *js = (CTinyJS*)userdata;
    js->root->trace(">  ");
}


void runFile(string);
void runTerminal();
string readFile(string);

int main(int argc, char **argv)
{
    int mode;
    cout<<"0:Terminal 1:File"<<endl;
    cin>>mode;
    if (!mode)
    {
        runTerminal();
    } else
    {
        cout<<"please input file name"<<endl;
        string fileName;
        cin>>fileName;
        cout<<endl<<"running script: "<<fileName<<endl<<endl;
        runFile(fileName);
    }

    return 0;
}

void runTerminal()
{
    CTinyJS *js = new CTinyJS();
    /* add the functions from TinyJS_Functions.cpp */
    registerFunctions(js);
    registerMathFunctions(js);
    /* Add a native function */
    js->addNative("function print(text)", &js_print, 0);
    js->addNative("function dump()", &js_dump, js);
    /* Execute out bit of code - we could call 'evaluate' here if
     we wanted something returned */
    try {
        js->execute("var lets_quit = 0; function quit() { lets_quit = 1; }");
        js->execute("print(\"Interactive mode... Type quit(); to exit, or print(...); to print something, or dump() to dump the symbol table!\");");
    } catch (CScriptException *e) {
        printf("ERROR: %s\n", e->text.c_str());
    }
    
    while (js->evaluate("lets_quit") == "0") {
        char buffer[2048];
        fgets ( buffer, sizeof(buffer), stdin );
        try {
            js->execute(buffer);
        } catch (CScriptException *e) {
            printf("ERROR: %s\n", e->text.c_str());
        }
    }
    delete js;
}

void runFile(string fileName)
{
    ifstream myFile(fileName);
    string file = readFile(fileName);
    cout<<"############### Source Code: "<< fileName <<"################"<<endl;
    cout<<file<<endl;
    cout<<"#################### Result: "<< fileName <<"################"<<endl;
    
    CTinyJS *js = new CTinyJS();
    /* add the functions from TinyJS_Functions.cpp */
    registerFunctions(js);
    /* Add a native function */
    js->addNative("function print(text)", &js_print, 0);
    js->addNative("function dump()", &js_dump, js);
    
    try {
        js->execute("var lets_quit = 0; function quit() { lets_quit = 1; }");
//        js->execute("print(\"Interactive mode... Type quit(); to exit, or print(...); to print something, or dump() to dump the symbol table!\");");
    } catch (CScriptException *e) {
        printf("ERROR: %s\n", e->text.c_str());
    }

    try {
        js->execute(file);
    } catch (CScriptException *e) {
        printf("ERROR: %s\n", e->text.c_str());
    }
    
    while (js->evaluate("lets_quit") == "0") {
        char buffer[2048];
        fgets ( buffer, sizeof(buffer), stdin );
        try {
            js->execute(buffer);
        } catch (CScriptException *e) {
            printf("ERROR: %s\n", e->text.c_str());
        }
    }
    delete js;
}

string readFile(string fileName)
{
    string file = "";
    string line;
    ifstream myFile(fileName);
    if (myFile.is_open()) {
        while (getline(myFile, line)) {
            file+=line+'\n';
        }
    }
    return file;
}