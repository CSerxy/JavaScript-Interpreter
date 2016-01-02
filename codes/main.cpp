//#include "Variable.h"
//#include "Backend.h"
//#include "BuiltinFunctions.h"
#include <string>
#include <iostream>
using namespace std;

typedef struct objReturn {
	string next;
	int judge;
	string memory;
} objReturn;

void getString(string u);
int weatherContain(string u, string v);

objReturn* dealWithObejct(string startString, char startChar, char endChar, int judge1, int judge2);

void dealWithFor(string u);
void dealWithWhile(string u);
void dealWithIf(string u);

int main() {
	getString("", "exit();");
}

void getString(string startString, string endString) {
	string current;
	while (cin >> current) {
		if (current == endString)
			return;
		cout << current;
		if (current == "for" || weatherContain(current, "for")) {
			if (current == "for")
				dealWithFor("");
			else
				dealWithFor(current.substr(3));

		}
		else if (current == "while" || weatherContain(current, "while")) {
			if (current == "while")
				dealWithWhile("");
			else 
				dealWithWhile(current.substr(5))

		}
		else if (current == "if" || weatherContain(current, "if")) {
			if (current == "if")
				dealWithIf("");
			else 
				dealWithIf(current.substr(2))

		}


	}
}

int weatherContain(string u, string v) {
	if (u.length() <= v.length())
		return 0;
	for (int i = 0; i < v.length(); i++) {
		if (u[i] != v[i])
			return 0;
	}
	return 1;
}

void dealWithFor(string u) {
	objReturn k, op1, op2;
	if (u == "")
		cin << u;
	k = dealWithObejct(u, "(", ";", 0, 0);
	op1 = dealWithObejct(k.next, "", ";", 1, 1);
	op2 = dealWithObejct(op1.next, "", ")", 0, 1);
	
	while (op1.judge) {
		
	}
}

void dealWithWhile() {
	
}
void dealWithIf() {
	
}
