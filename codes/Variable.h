#pragma once

#include <string>
#include <vector>
#include <map>

using namespace std;

class Function
{
public:
	string Content;
};



class Variable
{
public:
	Variable();
	int Flag;
	string Type;

	void Initialize();
	void Release();

	bool IsValueType();
	template<typename T> T GetValue();
	template<typename T> void SetValue(T value);

	bool IsFunction();
	Variable *CallSelf(vector<Variable *> params);
	Variable *FunctionCall(string functionName, vector<Variable *> &params);

	bool MemberExists(string memberName);
	void AddMember(string memberName);
	Variable *GetMember(string memberName);
	void SetMember(string memberName, Variable *value);
private:
	int ReferenceCount;
	void MemberReferenceDecrease();
	void *Value;
	map<string, Variable *> Members;

};
