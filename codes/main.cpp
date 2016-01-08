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

void getString(string remain, string startString, string endString, int judge1, string a[], int& num);
int weatherContain(string u, string v);

objReturn* dealWithObejct(string startString, char startChar, char endChar, int judge1, int judge2);

void dealWithFor(string u);
void dealWithWhile(string u);
void dealWithIf(string u);

int main(int argc, char ** argv) {
	string input = "";
	if (argc == 2) {
		string temp;
		ifscream re = open(argv[1]);
		stringstream buffer;
		buffer << t.rdbuf();
		input = buffer.str();
	}
	cout << input;
	string kkk[2];
	int kkkk;
	getString(input, "", "exit();", 0, kkk, kkkk);
}

void getString(string remain, string startString, string endString, int judge1, string a[], int& num) {
	int i;
	string current;
	while (remain.length() != 0 || cin >> current) {

		if (remain.length() != 0) {
			for (i = 0; i < remain.length(); i++) 
			if (remain[i] != ' ' && remain[i] != '\t' && remain[i] != '\n')
				continue;
			else 
				break;
			if (i == remain.length()) {
				current = remain;
				remain = "";
			} else {
				current = remain.substr(0, i);
				remain.erase(0, i);
			}
		}

		if (current == endString || weatherContain(current, endString));
			return;

		cout << current;
		if (current == "for" || weatherContain(current, "for")) {
			if (current == "for")
				dealWithFor(remain);
			else
				dealWithFor(current.substr(3) + remain);
			continue;
		}
		else if (current == "while" || weatherContain(current, "while")) {
			if (current == "while")
				dealWithWhile(remain);
			else 
				dealWithWhile(current.substr(5) + remain)
			continue;
		}
		else if (current == "if" || weatherContain(current, "if")) {
			if (current == "if")
				dealWithIf(remain);
			else 
				dealWithIf(current.substr(2) + remain)
			continue;
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

void dealWithFor(string remain) {
	int i;
	string current;
	objReturn k, op1, op2;

	if (remain == "")
		cin << current;
	else {
		for (i = 0; i < remain.length(); i++) 
		if (remain[i] != ' ' && remain[i] != '\t' && remain[i] != '\n')
			continue;
		else 
			break;
		if (i == remain.length()) {
			current = remain;
			remain = "";
		} else {
			current = remain.substr(0, i);
			remain.erase(0, i);
		}
	}

	k = dealWithObejct(remain, "(", ";", 0, 0);
	op1 = dealWithObejct(k.next, "", ";", 1, 1);
	op2 = dealWithObejct(op1.next, "", ")", 0, 1);

	if (op2.next == "")
		cin << op2.next;

	string a[200];									/* This place needs to be attention. */
	int num = 0;

	if (op2.next == "{" || weatherContain(op2.next, "{")) {
		if (op2.next == "{")
			getString("", "}", 1, a, num);
		else
			getString(op2.next.substr(1), "}", 1, a, num);
//		for (int i = 0; i < num; i++)
//			cout << a[i];
	} else if (op2.next == ";" || weatherContain(op2.next, ";")) {
		num = 0;
	} else {
		a[num] = dealWithObejct(op2.next, "", ";", 0, 1).memory;
		num ++;
	}
	string kkk[2];
	int kkkk = 0;
	while (dealWithObejct(op1.memory + ";", "", ";", 1, 0).judge) {
		for (int i = 0; i < num; i++)
			getString(a[i] + "^", "^", 0, kkk, kkkk);
	}
}

void dealWithWhile() {
	
}
void dealWithIf() {
	
}
