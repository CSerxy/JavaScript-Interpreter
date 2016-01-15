#ifndef TINYJS_H
#define TINYJS_H

// If defined, this keeps a note of all calls and where from in memory. This is slower, but good for debugging
#define TINYJS_CALL_STACK

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

/// convert the given string into a quoted string suitable for javascript
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
    int tk; ///< The type of the token that we have
    int tokenStart; ///< Position in the data at the beginning of the token we have here
    int tokenEnd; ///< Position in the data at the last character of the token we have here
    int tokenLastEnd; ///< Position in the data at the last character of the last token
    std::string tkStr; ///< Data contained in the token we have here

    void match(int expected_tk); ///< Lexical match wotsit
    static std::string getTokenStr(int token); ///< Get the string representation of the given token
    void reset(); ///< Reset this lex so we can start again

    std::string getSubString(int pos); ///< Return a sub-string from the given position up until right now
    Lexeme *getSubLex(int lastPosition); ///< Return a sub-lexer from the given position up until right now

    std::string getPosition(int pos=-1); ///< Return a string representing the position in lines and columns of the character pos given

protected:
    /* When we go into a loop, we use getSubLex to get a lexer for just the sub-part of the
       relevant string. This doesn't re-allocate and copy the string, but instead copies
       the data pointer and sets dataOwned to false, and dataStart/dataEnd to the relevant things. */
    char *data; ///< Data string to get tokens from
    int dataStart, dataEnd; ///< Start and end position in data string
    bool dataOwned; ///< Do we own this data string?

    int dataPos; ///< Position in data (we CAN go past the end of the string here)

    void getNextCh();
    void getNextToken(); ///< Get the text token from our text string
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
  VarLink(const VarLink &link); ///< Copy constructor
  ~VarLink();
  void replaceWith(Variable *newVar); ///< Replace the Variable pointed to
  void replaceWith(VarLink *newVar); ///< Replace the Variable pointed to (just dereferences)
  int getIntName(); ///< Get the name as an integer (for arrays)
  void setIntName(int n); ///< Set the name as an integer (for arrays)
};

/// Variable class (containing a doubly-linked list of children)
class Variable
{
public:
    Variable(); ///< Create undefined
    Variable(const std::string &varData, int varFlags); ///< User defined
    Variable(const std::string &str); ///< Create a string
    Variable(double varData);
    Variable(int val);
    ~Variable(void);

    Variable *getReturnVar(); ///< If this is a function, get the result value (for use by native functions)
    void setReturnVar(Variable *var); ///< Set the result value. Use this when setting complex return data as it avoids a deepCopy()
    Variable *getParameter(const std::string &name); ///< If this is a function, get the parameter with the given name (for use by native functions)

    VarLink *findChild(const std::string &childName); ///< Tries to find a child with the given name, may return 0
    VarLink *findChildOrCreate(const std::string &childName, int varFlags=SCRIPTVAR_UNDEFINED); ///< Tries to find a child with the given name, or will create it with the given flags
    VarLink *findChildOrCreateByPath(const std::string &path); ///< Tries to find a child with the given path (separated by dots)
    VarLink *addChild(const std::string &childName, Variable *child=NULL);
    VarLink *addChildNoDup(const std::string &childName, Variable *child=NULL); ///< add a child overwriting any with the same name
    void removeChild(Variable *child);
    void removeLink(VarLink *link); ///< Remove a specific link (this is faster than finding via a child)
    void removeAllChildren();
    Variable *getArrayIndex(int idx); ///< The the value at an array index
    void setArrayIndex(int idx, Variable *value); ///< Set the value at an array index
    int getArrayLength(); ///< If this is an array, return the number of items in it (else 0)
    int getChildren(); ///< Get the number of children

    int getInt();
    bool getBool() { return getInt() != 0; }
    double getDouble();
    const std::string &getString();
    std::string getParsableString(); ///< get Data as a parsable javascript string
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
    bool isBasic() { return firstChild==0; } ///< Is this *not* an array/object/etc

    Variable *mathsOp(Variable *b, int op); ///< do a maths op with another script variable
    void copyValue(Variable *val); ///< copy the value from the value given
    Variable *deepCopy(); ///< deep copy this node and return the result

    void trace(std::string indentStr = "", const std::string &name = ""); ///< Dump out the contents of this using trace
    std::string getFlagsAsString(); ///< For debugging - just dump a string version of the flags
    void getJSON(std::ostringstream &destination, const std::string linePrefix=""); ///< Write out all the JS code needed to recreate this script variable to the stream (as JSON)
    void setCallback(JSCallback callback, void *userdata); ///< Set the callback for native functions

    VarLink *firstChild;
    VarLink *lastChild;

    /// For memory management/garbage collection
    Variable *ref(); ///< Add reference to this variable
    void unref(); ///< Remove a reference, and delete this variable if required
    int getRefs(); ///< Get the number of references to this script variable
protected:
    int refs; ///< The number of references held to this - used for garbage collection

    std::string data; ///< The contents of this variable if it is a string
    long intData; ///< The contents of this variable if it is an int
    double doubleData; ///< The contents of this variable if it is a double
    int flags; ///< the flags determine the type of the variable - int/double/string/etc
    JSCallback jsCallback; ///< Callback for native functions
    void *jsCallbackUserData; ///< user data passed as second argument to native functions

    void init(); ///< initialisation of data members

    /** Copy the basic data and flags from the variable given, with no
      * children. Should be used internally only - by copyValue and deepCopy */
    void copySimpleData(Variable *val);

    friend class CTinyJS;
};

class CTinyJS {
public:
    CTinyJS();
    ~CTinyJS();

    void execute(const std::string &code);
    /** Evaluate the given code and return a link to a javascript object,
     * useful for (dangerous) JSON parsing. If nothing to return, will return
     * 'undefined' variable type. VarLink is returned as this will
     * automatically unref the result as it goes out of scope. If you want to
     * keep it, you must use ref() and unref() */
    VarLink evaluateComplex(const std::string &code);
    /** Evaluate the given code and return a string. If nothing to return, will return
     * 'undefined' */
    std::string evaluate(const std::string &code);

    /// add a native function to be called from TinyJS
    /** example:
       \code
           void scRandInt(Variable *c, void *userdata) { ... }
           tinyJS->addNative("function randInt(min, max)", scRandInt, 0);
       \endcode

       or

       \code
           void scSubstring(Variable *c, void *userdata) { ... }
           tinyJS->addNative("function String.substring(lo, hi)", scSubstring, 0);
       \endcode
    */
    void addNative(const std::string &funcDesc, JSCallback ptr, void *userdata);

    /// Get the given variable specified by a path (var1.var2.etc), or return 0
    Variable *getScriptVariable(const std::string &path);
    /// Get the value of the given variable, or return 0
    const std::string *getVariable(const std::string &path);
    /// set the value of the given variable, return trur if it exists and gets set
    bool setVariable(const std::string &path, const std::string &varData);

    /// Send all variables to stdout
    void trace();

    Variable *root;   /// root of symbol table
private:
    Lexeme *l;             /// current lexer
    std::vector<Variable*> scopes; /// stack of scopes when parsing
#ifdef TINYJS_CALL_STACK
    std::vector<std::string> call_stack; /// Names of places called so we can show when erroring
#endif

    Variable *stringClass; /// Built in string class
    Variable *objectClass; /// Built in object class
    Variable *arrayClass; /// Built in array class

    // parsing - in order of precedence
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
    // parsing utility functions
    VarLink *parseFunctionDefinition();
    void parseFunctionArguments(Variable *funcVar);

    VarLink *findInScopes(const std::string &childName); ///< Finds a child, looking recursively up the scopes
    /// Look up in any parent classes of the given object
    VarLink *findInParentClasses(Variable *object, const std::string &name);
};

#endif
