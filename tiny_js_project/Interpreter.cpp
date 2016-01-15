#include "JavaScript.h"
#include "Functions.h"
#include "MathFunctions.h"
#include <assert.h>
#include <stdio.h>
#include <cstring>
#include <iostream>
#include <fstream>
using namespace std;

void js_print(Variable *v, void *userdata) {
    printf("> %s\n", v->getParameter("text")->getString().c_str());
}

void js_dump(Variable *v, void *userdata) {
    JavaScript *js = (JavaScript*)userdata;
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
    JavaScript *js = new JavaScript();
    
    registerFunctions(js);
    registerMathFunctions(js);

    js->addNative("function print(text)", &js_print, 0);
    js->addNative("function dump()", &js_dump, js);
    
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
    
    JavaScript *js = new JavaScript();

    registerFunctions(js);
    registerMathFunctions(js);
    /* Add a native function */
    js->addNative("function print(text)", &js_print, 0);
    js->addNative("function dump()", &js_dump, js);
    
    try {
        js->execute("var lets_quit = 0; function quit() { lets_quit = 1; }");
    } catch (CScriptException *e) {
        printf("ERROR: %s\n", e->text.c_str());
    }

    try {
        js->execute(file);
    } catch (CScriptException *e) {
        printf("ERROR: %s\n", e->text.c_str());
    }
    
    cout<<"You can continue writing script. Type quit(); to exit, or print(...); to print something, or dump() to dump the symbol table!"<<endl;
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