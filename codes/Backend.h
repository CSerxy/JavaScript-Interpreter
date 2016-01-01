#pragma once

#include "Variable.h"
#include "BuiltinFunctions.h"

void CreateClass(string classType, vector<string> memberNames, vector<string> memberTypes);
Variable *CreateVariable(string type);
Variable *GetVariableFromFlag(int flag);
void DeleteVariable(int flag);
bool VariableExist(int flag);

Variable *TestFunction(Variable *self, vector<Variable *> &);

namespace Constants
{
	string FunctionType = "Function";
	vector<string> ValueTypes{ "Integer", "Boolean", "Double", "String", FunctionType };
	template<typename T> string ValueTypeToString();
	template<> string ValueTypeToString<int>(){ return "Integer"; }
	template<> string ValueTypeToString<bool>(){ return "Boolean"; }
	template<> string ValueTypeToString<double>(){ return "Double"; }
	template<> string ValueTypeToString<string>(){ return "String"; }
	template<> string ValueTypeToString<Function>(){ return "Function"; }

	
}
