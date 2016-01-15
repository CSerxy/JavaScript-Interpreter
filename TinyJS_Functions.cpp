#include "TinyJS_Functions.h"
#include <math.h>
#include <cstdlib>
#include <sstream>

using namespace std;
// ----------------------------------------------- Actual Functions
void scTrace(Variable *c, void *userdata) {
    CTinyJS *js = (CTinyJS*)userdata;
    js->root->trace();
}

void scObjectDump(Variable *c, void *) {
    c->getParameter("this")->trace("> ");
}

void scObjectClone(Variable *c, void *) {
    Variable *obj = c->getParameter("this");
    c->getReturnVar()->copyValue(obj);
}

void scMathRand(Variable *c, void *) {
    c->getReturnVar()->setDouble((double)rand()/RAND_MAX);
}

void scMathRandInt(Variable *c, void *) {
    int min = c->getParameter("min")->getInt();
    int max = c->getParameter("max")->getInt();
    int val = min + (int)(rand()%(1+max-min));
    c->getReturnVar()->setInt(val);
}

void scCharToInt(Variable *c, void *) {
    string str = c->getParameter("ch")->getString();;
    int val = 0;
    if (str.length()>0)
        val = (int)str.c_str()[0];
    c->getReturnVar()->setInt(val);
}

void scStringIndexOf(Variable *c, void *) {
    string str = c->getParameter("this")->getString();
    string search = c->getParameter("search")->getString();
    size_t p = str.find(search);
    int val = (p==string::npos) ? -1 : p;
    c->getReturnVar()->setInt(val);
}

void scStringSubstring(Variable *c, void *) {
    string str = c->getParameter("this")->getString();
    int lo = c->getParameter("lo")->getInt();
    int hi = c->getParameter("hi")->getInt();

    int l = hi-lo;
    if (l>0 && lo>=0 && lo+l<=(int)str.length())
      c->getReturnVar()->setString(str.substr(lo, l));
    else
      c->getReturnVar()->setString("");
}

void scStringCharAt(Variable *c, void *) {
    string str = c->getParameter("this")->getString();
    int p = c->getParameter("pos")->getInt();
    if (p>=0 && p<(int)str.length())
      c->getReturnVar()->setString(str.substr(p, 1));
    else
      c->getReturnVar()->setString("");
}

void scStringCharCodeAt(Variable *c, void *) {
    string str = c->getParameter("this")->getString();
    int p = c->getParameter("pos")->getInt();
    if (p>=0 && p<(int)str.length())
      c->getReturnVar()->setInt(str.at(p));
    else
      c->getReturnVar()->setInt(0);
}

void scStringSplit(Variable *c, void *) {
    string str = c->getParameter("this")->getString();
    string sep = c->getParameter("separator")->getString();
    Variable *result = c->getReturnVar();
    result->setArray();
    int length = 0;

    size_t pos = str.find(sep);
    while (pos != string::npos) {
      result->setArrayIndex(length++, new Variable(str.substr(0,pos)));
      str = str.substr(pos+1);
      pos = str.find(sep);
    }

    if (str.size()>0)
      result->setArrayIndex(length++, new Variable(str));
}

void scStringFromCharCode(Variable *c, void *) {
    char str[2];
    str[0] = c->getParameter("char")->getInt();
    str[1] = 0;
    c->getReturnVar()->setString(str);
}

void scIntegerParseInt(Variable *c, void *) {
    string str = c->getParameter("str")->getString();
    int val = strtol(str.c_str(),0,0);
    c->getReturnVar()->setInt(val);
}

void scIntegerValueOf(Variable *c, void *) {
    string str = c->getParameter("str")->getString();

    int val = 0;
    if (str.length()==1)
      val = str[0];
    c->getReturnVar()->setInt(val);
}

void scJSONStringify(Variable *c, void *) {
    std::ostringstream result;
    c->getParameter("obj")->getJSON(result);
    c->getReturnVar()->setString(result.str());
}

void scExec(Variable *c, void *data) {
    CTinyJS *tinyJS = (CTinyJS *)data;
    std::string str = c->getParameter("jsCode")->getString();
    tinyJS->execute(str);
}

void scEval(Variable *c, void *data) {
    CTinyJS *tinyJS = (CTinyJS *)data;
    std::string str = c->getParameter("jsCode")->getString();
    c->setReturnVar(tinyJS->evaluateComplex(str).var);
}

void scArrayContains(Variable *c, void *data) {
  Variable *obj = c->getParameter("obj");
  VarLink *v = c->getParameter("this")->firstChild;

  bool contains = false;
  while (v) {
      if (v->var->equals(obj)) {
        contains = true;
        break;
      }
      v = v->nextSibling;
  }

  c->getReturnVar()->setInt(contains);
}

void scArrayRemove(Variable *c, void *data) {
  Variable *obj = c->getParameter("obj");
  vector<int> removedIndices;
  VarLink *v;
  // remove
  v = c->getParameter("this")->firstChild;
  while (v) {
      if (v->var->equals(obj)) {
        removedIndices.push_back(v->getIntName());
      }
      v = v->nextSibling;
  }
  // renumber
  v = c->getParameter("this")->firstChild;
  while (v) {
      int n = v->getIntName();
      int newn = n;
      for (size_t i=0;i<removedIndices.size();i++)
        if (n>=removedIndices[i])
          newn--;
      if (newn!=n)
        v->setIntName(newn);
      v = v->nextSibling;
  }
}

void scArrayJoin(Variable *c, void *data) {
  string sep = c->getParameter("separator")->getString();
  Variable *arr = c->getParameter("this");

  ostringstream sstr;
  int l = arr->getArrayLength();
  for (int i=0;i<l;i++) {
    if (i>0) sstr << sep;
    sstr << arr->getArrayIndex(i)->getString();
  }

  c->getReturnVar()->setString(sstr.str());
}

// ----------------------------------------------- Register Functions
void registerFunctions(CTinyJS *tinyJS) {
    tinyJS->addNative("function exec(jsCode)", scExec, tinyJS); // execute the given code
    tinyJS->addNative("function eval(jsCode)", scEval, tinyJS); // execute the given string (an expression) and return the result
    tinyJS->addNative("function trace()", scTrace, tinyJS);
    tinyJS->addNative("function Object.dump()", scObjectDump, 0);
    tinyJS->addNative("function Object.clone()", scObjectClone, 0);
    tinyJS->addNative("function Math.rand()", scMathRand, 0);
    tinyJS->addNative("function Math.randInt(min, max)", scMathRandInt, 0);
    tinyJS->addNative("function charToInt(ch)", scCharToInt, 0); //  convert a character to an int - get its value
    tinyJS->addNative("function String.indexOf(search)", scStringIndexOf, 0); // find the position of a string in a string, -1 if not
    tinyJS->addNative("function String.substring(lo,hi)", scStringSubstring, 0);
    tinyJS->addNative("function String.charAt(pos)", scStringCharAt, 0);
    tinyJS->addNative("function String.charCodeAt(pos)", scStringCharCodeAt, 0);
    tinyJS->addNative("function String.fromCharCode(char)", scStringFromCharCode, 0);
    tinyJS->addNative("function String.split(separator)", scStringSplit, 0);
    tinyJS->addNative("function Integer.parseInt(str)", scIntegerParseInt, 0); // string to int
    tinyJS->addNative("function Integer.valueOf(str)", scIntegerValueOf, 0); // value of a single character
    tinyJS->addNative("function JSON.stringify(obj, replacer)", scJSONStringify, 0); // convert to JSON. replacer is ignored at the moment
    // JSON.parse is left out as you can (unsafely!) use eval instead
    tinyJS->addNative("function Array.contains(obj)", scArrayContains, 0);
    tinyJS->addNative("function Array.remove(obj)", scArrayRemove, 0);
    tinyJS->addNative("function Array.join(separator)", scArrayJoin, 0);
}

