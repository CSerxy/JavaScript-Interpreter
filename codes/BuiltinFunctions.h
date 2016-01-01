#pragma once

#include "Variable.h"

namespace Constants
{
	map<string, map<string, Variable *(Variable*, vector<Variable*> &) >> BuiltinFunctions = {
		{ "Integer",
		{
			{ "", TestFunction },
			{ "", TestFunction },
		} },
		{ "Boolean",
		{
			{ "", TestFunction },
			{ "", TestFunction },
			} },
			{
				"Double", {
					{ "", TestFunction },
					{ "", TestFunction },
				} },
				{
					"String", {
						{ "", TestFunction },
						{ "", TestFunction },
					} },
					{ "Function", {
						{ "", TestFunction },
						{ "", TestFunction },
						} },
	};
}

Variable *Integer_ToString(Variable*, vector<Variable*> &);