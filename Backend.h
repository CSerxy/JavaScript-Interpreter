#include <string>
#include <vector>
#include <map>
using namespace std;

namespace Constants
{
	vector<string> ValueTypes{ "Integer", "Boolean", "Double", "String" };
	template<typename T> string ValueTypeToString();
	template<> string ValueTypeToString<int>(){ return "Integer"; }
	template<> string ValueTypeToString<bool>(){ return "Boolean"; }
	template<> string ValueTypeToString<double>(){ return "Double"; }
	template<> string ValueTypeToString<string>(){ return "String"; }
}

class Variable
{
public:
	Variable();
	string Flag;
	string Type;

	void Initialize();
	void Release();

	bool IsValueType();
	template<typename T> void SetValue(T value);

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



void CreateClass(string classType, vector<string> memberNames, vector<string> memberTypes);
Variable *CreateVariable(string type, string flag, string name);
Variable *GetVariable(string flag);
void DeleteVariable(string flag);
bool VariableExist(string flag);
