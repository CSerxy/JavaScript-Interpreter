#include "Backend.h"
#include "BuiltinFunctions.h"

Variable *Integer_ToString(Variable* self, vector<Variable*> & params)
{
	auto pVariable = CreateVariable("String");
	pVariable->SetValue<string>(to_string(self->GetValue<int>()));
	return pVariable;
}

