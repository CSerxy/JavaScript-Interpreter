#include "Backend.h"
#include "BuiltinFunctions.h"

template<typename T> bool Variable::IsValueType()
{
	auto &vec = Constants::ValueTypes;
	return std::find(vec.begin(), vec.end(), Type) != vec.end();
}
template<typename T> T Variable::GetValue()
{
	return (T)*Value;
}
template<typename T> void Variable::SetValue(T value)
{
	if (!IsValueType())
	{
		ReleaseMembers();
	}
	void *innerValue = (void *)new T(value);
	Type = Constants::ValueTypeToString<T>();
	Value = innerValue;
}



bool Variable::IsFunction()
{
	return Type == Constants::FunctionType;
}
Variable *Variable::CallSelf(vector<Variable *> params)
{
	if (!IsFunction())
	{
		throw new logic_error("Cannot call self on non-function variable!");
		return nullptr;
	}

	return nullptr;
}
Variable *Variable::FunctionCall(string functionName, vector<Variable *> &params)
{
	if (Constants::BuiltinFunctions.find(Type) != Constants::BuiltinFunctions.end())
	{
		auto &builtinFunctions = Constants::BuiltinFunctions[Type];
		if (builtinFunctions.find(functionName) != builtinFunctions.end())
		{
			Variable *result = builtinFunctions[functionName](this, params);
			return result;
		}
	}
	if (!MemberExists(functionName))
	{
		throw new invalid_argument("Member function does not exist!");
		return nullptr;
	}
	if (!Members[functionName]->IsFunction)
	{
		throw new invalid_argument("Member of this name is not a function!");
		return nullptr;
	}
	return Members[functionName]->CallSelf(params);

}


void Variable::Initialize()
{
	ReferenceCount = 0;
}
void Variable::Release()
{
	for (auto i : Members)
	{
		i.second->ReferenceCount--;
		if (i.second->ReferenceCount <= 0)
		{
			DeleteVariable(i.second->Flag);
			Members[i.first] = nullptr;
		}
	}
	if (IsValueType())
	{
		delete Value;
	}
}
bool Variable::MemberExists(string memberName)
{
	if (Members.find(memberName) != Members.end())
	{
		if (Members[memberName] != nullptr)
			return true;
		Members.erase(memberName);
		return false;
	}
	return false;
}
void Variable::AddMember(string memberName)
{
	if (MemberExists(memberName))
	{
		throw new invalid_argument("Member of the same name already exists!");
		return;
	}
	Members[memberName] = nullptr;
}
Variable *Variable::GetMember(string memberName)
{
	if (!MemberExists(memberName))
	{
		throw new invalid_argument("Member of this name does not exist!");
		return nullptr;
	}
	return Members[memberName];
}
void Variable::SetMember(string memberName, Variable *value)
{
	if (MemberExists(memberName))
	{
		Members[memberName]->ReferenceCount--;
		if (Members[memberName]->ReferenceCount <= 0)
		{
			delete Members[memberName];
			Members[memberName] = nullptr;
		}
	}
	Members[memberName] = value;
	if (value != nullptr)
	{
		value->ReferenceCount++;
	}
}

class ClassVariableInfo
{
public:
	string Type;
	string Name;
};

class ClassInfo
{
public:
	string Type;
	vector<ClassVariableInfo> Variables;
};

map<string, ClassInfo> Classes;
map<int, Variable *> Variables;

void CreateClass(string classType, vector<string> memberNames, vector<string> memberTypes)
{
	if (Classes.find(classType) != Classes.end())
	{
		throw new invalid_argument("Class of this name already exists!");
		return;
	}
	ClassInfo classInfo;
	classInfo.Type = classType;
	for (int i = 0; i < memberNames.size(); i++)
	{
		ClassVariableInfo cvInfo;
		cvInfo.Name = memberNames[i];
		cvInfo.Type = memberTypes[i];
		classInfo.Variables.push_back(cvInfo);
	}
	Classes[classType] = classInfo;
}

bool VariableExist(int flag)
{
	if (Variables.find(flag) != Variables.end())
	{
		if (Variables[flag] != nullptr)
			return true;
		Variables.erase(flag);
		return false;
	}
	return false;
}

int GetNextFlag()
{
	static int a = 0;
	a++;
	return a;
}

Variable *CreateVariable(string type)
{
	Variable *variable = new Variable();
	variable->Flag = GetNextFlag();
	variable->Type = type;
	variable->Initialize();

	auto &cls = Classes[type];
	Variables[variable->Flag] = variable;
}

Variable *GetVariableFromFlag(int flag)
{
	if (!VariableExist(flag))
	{
		throw invalid_argument("Variable of this flag does not exist!");
		return nullptr;
	}
	return Variables[flag];
}

void DeleteVariable(int flag)
{
	if (!VariableExist(flag))
	{
		throw new invalid_argument("Variable of this flag does not exist!");
		return;
	}
	delete Variables[flag];
	Variables.erase(flag);
}

