#ifndef JavaScript_H
#define JavaScript_H

#define JS_CALL_STACK

#include <stdlib.h>
#include <string>
#include <vector>

#ifndef TRACE
#define TRACE printf
#endif // TRACE


const int TINYJS_LOOP_MAX_ITERATIONS = 8192;

enum LEX_TYPES {
    LEX_EOF = 0,
    LEX_ID = 256,
    LEX_INT,
    LEX_FLOAT,
    LEX_STR,

    LEX_EQUAL,
    LEX_TYPEEQUAL,
    LEX_NEQUAL,
    LEX_NTYPEEQUAL,
    LEX_LEQUAL,
    LEX_LSHIFT,
    LEX_LSHIFTEQUAL,
    LEX_GEQUAL,
    LEX_RSHIFT,
    LEX_RSHIFTUNSIGNED,
    LEX_RSHIFTEQUAL,
    LEX_PLUSEQUAL,
    LEX_MINUSEQUAL,
    LEX_PLUSPLUS,
    LEX_MINUSMINUS,
    LEX_ANDEQUAL,
    LEX_ANDAND,
    LEX_OREQUAL,
    LEX_OROR,
    LEX_XOREQUAL,
    // reserved words
#define LEX_R_LIST_START LEX_R_IF
    LEX_R_IF,
    LEX_R_ELSE,
    LEX_R_DO,
    LEX_R_WHILE,
    LEX_R_FOR,
    LEX_R_BREAK,
    LEX_R_CONTINUE,
    LEX_R_FUNCTION,
    LEX_R_RETURN,
    LEX_R_VAR,
    LEX_R_TRUE,
    LEX_R_FALSE,
    LEX_R_NULL,
    LEX_R_UNDEFINED,
    LEX_R_NEW,

	LEX_R_LIST_END /* always the last entry */
};

enum SCRIPTVAR_FLAGS {
    SCRIPTVAR_UNDEFINED   = 0,
    SCRIPTVAR_FUNCTION    = 1,
    SCRIPTVAR_OBJECT      = 2,
    SCRIPTVAR_ARRAY       = 4,
    SCRIPTVAR_DOUBLE      = 8,  // floating point double
    SCRIPTVAR_INTEGER     = 16, // integer number
    SCRIPTVAR_STRING      = 32, // string
    SCRIPTVAR_NULL        = 64, // it seems null is its own data type

    SCRIPTVAR_NATIVE      = 128, // to specify this is a native function
    SCRIPTVAR_NUMERICMASK = SCRIPTVAR_NULL |
                            SCRIPTVAR_DOUBLE |
                            SCRIPTVAR_INTEGER,
    SCRIPTVAR_VARTYPEMASK = SCRIPTVAR_DOUBLE |
                            SCRIPTVAR_INTEGER |
                            SCRIPTVAR_STRING |
                            SCRIPTVAR_FUNCTION |
                            SCRIPTVAR_OBJECT |
                            SCRIPTVAR_ARRAY |
                            SCRIPTVAR_NULL,

};

#define TINYJS_RETURN_VAR "return"
#define TINYJS_PROTOTYPE_CLASS "prototype"
#define TINYJS_TEMP_NAME ""
#define TINYJS_BLANK_DATA ""

std::string getJSString(const std::string &str);

class CScriptException {
public:
    std::string text;
    CScriptException(const std::string &exceptionText);
};

class Lexeme
{
public:
    Lexeme(const std::string &input);
    Lexeme(Lexeme *owner, int startChar, int endChar);
    ~Lexeme(void);

    char currCh, nextCh;
    int tk;
    int tokenStart;
    int tokenEnd;
    int tokenLastEnd;
    std::string tkStr;
    void match(int expected_tk);
    static std::string getTokenStr(int token);
    void reset();

    std::string getSubString(int pos);
    Lexeme *getSubLex(int lastPosition);

    std::string getPosition(int pos=-1);

protected:
    char *data;
    int dataStart, dataEnd;
    bool dataOwned;

    int dataPos;

    void getNextCh();
    void getNextToken();
};

class Variable;

typedef void (*JSCallback)(Variable *var, void *userdata);

class VarLink
{
public:
  std::string name;
  VarLink *nextSibling;
  VarLink *prevSibling;
  Variable *var;
  bool owned;

  VarLink(Variable *var, const std::string &name = TINYJS_TEMP_NAME);
  VarLink(const VarLink &link);
  ~VarLink();
  void replaceWith(Variable *newVar);
  void replaceWith(VarLink *newVar);
  int getIntName();
  void setIntName(int n);
};

class Variable
{
public:
    Variable();
    Variable(const std::string &varData, int varFlags);
    Variable(const std::string &str);
    Variable(double varData);
    Variable(int val);
    ~Variable(void);

    Variable *getReturnVar();
    void setReturnVar(Variable *var);
    Variable *getParameter(const std::string &name);

    VarLink *findChild(const std::string &childName);
    VarLink *findChildOrCreate(const std::string &childName, int varFlags=SCRIPTVAR_UNDEFINED);
    VarLink *findChildOrCreateByPath(const std::string &path);
    VarLink *addChild(const std::string &childName, Variable *child=NULL);
    VarLink *addChildNoDup(const std::string &childName, Variable *child=NULL);
    void removeChild(Variable *child);
    void removeLink(VarLink *link);
    void removeAllChildren();
    Variable *getArrayIndex(int idx);
    void setArrayIndex(int idx, Variable *value);
    int getArrayLength();
    int getChildren();

    int getInt();
    bool getBool() { return getInt() != 0; }
    double getDouble();
    const std::string &getString();
    std::string getParsableString();
    
    void setInt(int num);
    void setDouble(double val);
    void setString(const std::string &str);
    void setUndefined();
    void setArray();
    bool equals(Variable *v);

    bool isInt() { return (flags&SCRIPTVAR_INTEGER)!=0; }
    bool isDouble() { return (flags&SCRIPTVAR_DOUBLE)!=0; }
    bool isString() { return (flags&SCRIPTVAR_STRING)!=0; }
    bool isNumeric() { return (flags&SCRIPTVAR_NUMERICMASK)!=0; }
    bool isFunction() { return (flags&SCRIPTVAR_FUNCTION)!=0; }
    bool isObject() { return (flags&SCRIPTVAR_OBJECT)!=0; }
    bool isArray() { return (flags&SCRIPTVAR_ARRAY)!=0; }
    bool isNative() { return (flags&SCRIPTVAR_NATIVE)!=0; }
    bool isUndefined() { return (flags & SCRIPTVAR_VARTYPEMASK) == SCRIPTVAR_UNDEFINED; }
    bool isNull() { return (flags & SCRIPTVAR_NULL)!=0; }
    bool isBasic() { return firstChild==0; }

    Variable *mathsOp(Variable *b, int op);
    
    void copyValue(Variable *val);
    Variable *deepCopy();

    void trace(std::string indentStr = "", const std::string &name = "");
    
    std::string getFlagsAsString();
    
    void getJSON(std::ostringstream &destination, const std::string linePrefix="");
    void setCallback(JSCallback callback, void *userdata);

    VarLink *firstChild;
    VarLink *lastChild;

    Variable *ref();
    void unref();
    int getRefs();
protected:
    int refs;
    
    std::string data;
    long intData;
    double doubleData;
    int flags;
    JSCallback jsCallback;
    
    void *jsCallbackUserData;

    void init();
    void copySimpleData(Variable *val);

    friend class JavaScript;
};

class JavaScript {
public:
    JavaScript();
    ~JavaScript();

    void execute(const std::string &code);

    VarLink evaluateComplex(const std::string &code);
    std::string evaluate(const std::string &code);

    void addNative(const std::string &funcDesc, JSCallback ptr, void *userdata);

    Variable *getScriptVariable(const std::string &path);
    const std::string *getVariable(const std::string &path);
    bool setVariable(const std::string &path, const std::string &varData);

    void trace();

    Variable *root;
private:
    Lexeme *l;
    std::vector<Variable*> scopes;
#ifdef JS_CALL_STACK
    std::vector<std::string> call_stack;
#endif

    Variable *stringClass;
    Variable *objectClass;
    Variable *arrayClass;

    VarLink *functionCall(bool &execute, VarLink *function, Variable *parent);
    VarLink *factor(bool &execute);
    VarLink *unary(bool &execute);
    VarLink *term(bool &execute);
    VarLink *expression(bool &execute);
    VarLink *shift(bool &execute);
    VarLink *condition(bool &execute);
    VarLink *logic(bool &execute);
    VarLink *ternary(bool &execute);
    VarLink *base(bool &execute);
    void block(bool &execute);
    void statement(bool &execute);

    VarLink *parseFunctionDefinition();
    void parseFunctionArguments(Variable *funcVar);

    VarLink *findInScopes(const std::string &childName);
    VarLink *findInParentClasses(Variable *object, const std::string &name);
};

#endif
