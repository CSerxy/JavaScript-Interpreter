#include <math.h>
#include <cstdlib>
#include <sstream>
#include "MathFunctions.h"

using namespace std;

#define k_E                 exp(1.0)
#define k_PI                3.1415926535897932384626433832795

#define F_ABS(a)            ((a)>=0 ? (a) : (-(a)))
#define F_MIN(a,b)          ((a)>(b) ? (b) : (a))
#define F_MAX(a,b)          ((a)>(b) ? (a) : (b))
#define F_SGN(a)            ((a)>0 ? 1 : ((a)<0 ? -1 : 0 ))
#define F_RNG(a,min,max)    ((a)<(min) ? min : ((a)>(max) ? max : a ))
#define F_ROUND(a)          ((a)>0 ? (int) ((a)+0.5) : (int) ((a)-0.5) )
 
//Variable shortcut macro
#define scIsInt(a)          ( c->getParameter(a)->isInt() )
#define scIsDouble(a)       ( c->getParameter(a)->isDouble() )  
#define scGetInt(a)         ( c->getParameter(a)->getInt() )
#define scGetDouble(a)      ( c->getParameter(a)->getDouble() )  
#define scReturnInt(a)      ( c->getReturnVar()->setInt(a) )
#define scReturnDouble(a)   ( c->getReturnVar()->setDouble(a) )  

void scMathAbs(Variable *c, void *userdata) {
    if ( scIsInt("a") ) {
      scReturnInt( F_ABS( scGetInt("a") ) );
    } else if ( scIsDouble("a") ) {
      scReturnDouble( F_ABS( scGetDouble("a") ) );
    }
}

void scMathRound(Variable *c, void *userdata) {
    if ( scIsInt("a") ) {
      scReturnInt( F_ROUND( scGetInt("a") ) );
    } else if ( scIsDouble("a") ) {
      scReturnDouble( F_ROUND( scGetDouble("a") ) );
    }
}

void scMathMin(Variable *c, void *userdata) {
    if ( (scIsInt("a")) && (scIsInt("b")) ) {
      scReturnInt( F_MIN( scGetInt("a"), scGetInt("b") ) );
    } else {
      scReturnDouble( F_MIN( scGetDouble("a"), scGetDouble("b") ) );
    }
}

void scMathMax(Variable *c, void *userdata) {
    if ( (scIsInt("a")) && (scIsInt("b")) ) {
      scReturnInt( F_MAX( scGetInt("a"), scGetInt("b") ) );
    } else {
      scReturnDouble( F_MAX( scGetDouble("a"), scGetDouble("b") ) );
    }
}

void scMathRange(Variable *c, void *userdata) {
    if ( (scIsInt("x")) ) {
      scReturnInt( F_RNG( scGetInt("x"), scGetInt("a"), scGetInt("b") ) );
    } else {
      scReturnDouble( F_RNG( scGetDouble("x"), scGetDouble("a"), scGetDouble("b") ) );
    }
}

void scMathSign(Variable *c, void *userdata) {
    if ( scIsInt("a") ) {
      scReturnInt( F_SGN( scGetInt("a") ) );
    } else if ( scIsDouble("a") ) {
      scReturnDouble( F_SGN( scGetDouble("a") ) );
    }
}

void scMathPI(Variable *c, void *userdata) {
    scReturnDouble(k_PI);
}

void scMathToDegrees(Variable *c, void *userdata) {
    scReturnDouble( (180.0/k_PI)*( scGetDouble("a") ) );
}

void scMathToRadians(Variable *c, void *userdata) {
    scReturnDouble( (k_PI/180.0)*( scGetDouble("a") ) );
}

void scMathSin(Variable *c, void *userdata) {
    scReturnDouble( sin( scGetDouble("a") ) );
}

void scMathASin(Variable *c, void *userdata) {
    scReturnDouble( asin( scGetDouble("a") ) );
}

void scMathCos(Variable *c, void *userdata) {
    scReturnDouble( cos( scGetDouble("a") ) );
}

void scMathACos(Variable *c, void *userdata) {
    scReturnDouble( acos( scGetDouble("a") ) );
}

void scMathTan(Variable *c, void *userdata) {
    scReturnDouble( tan( scGetDouble("a") ) );
}

void scMathATan(Variable *c, void *userdata) {
    scReturnDouble( atan( scGetDouble("a") ) );
}

void scMathSinh(Variable *c, void *userdata) {
    scReturnDouble( sinh( scGetDouble("a") ) );
}

void scMathASinh(Variable *c, void *userdata) {
    scReturnDouble( asinh( scGetDouble("a") ) );
}

void scMathCosh(Variable *c, void *userdata) {
    scReturnDouble( cosh( scGetDouble("a") ) );
}

void scMathACosh(Variable *c, void *userdata) {
    scReturnDouble( acosh( scGetDouble("a") ) );
}

void scMathTanh(Variable *c, void *userdata) {
    scReturnDouble( tanh( scGetDouble("a") ) );
}

void scMathATanh(Variable *c, void *userdata) {
    scReturnDouble( atan( scGetDouble("a") ) );
}

void scMathE(Variable *c, void *userdata) {
    scReturnDouble(k_E);
}

void scMathLog(Variable *c, void *userdata) {
    scReturnDouble( log( scGetDouble("a") ) );
}

void scMathLog10(Variable *c, void *userdata) {
    scReturnDouble( log10( scGetDouble("a") ) );
}

void scMathExp(Variable *c, void *userdata) {
    scReturnDouble( exp( scGetDouble("a") ) );
}

void scMathPow(Variable *c, void *userdata) {
    scReturnDouble( pow( scGetDouble("a"), scGetDouble("b") ) );
}

void scMathSqr(Variable *c, void *userdata) {
    scReturnDouble( ( scGetDouble("a") * scGetDouble("a") ) );
}

void scMathSqrt(Variable *c, void *userdata) {
    scReturnDouble( sqrtf( scGetDouble("a") ) );
}

void registerMathFunctions(CTinyJS *tinyJS) {
    tinyJS->addNative("function Math.abs(a)", scMathAbs, 0);
    tinyJS->addNative("function Math.round(a)", scMathRound, 0);
    tinyJS->addNative("function Math.min(a,b)", scMathMin, 0);
    tinyJS->addNative("function Math.max(a,b)", scMathMax, 0);
    tinyJS->addNative("function Math.range(x,a,b)", scMathRange, 0);
    tinyJS->addNative("function Math.sign(a)", scMathSign, 0);
    
    tinyJS->addNative("function Math.PI()", scMathPI, 0);
    tinyJS->addNative("function Math.toDegrees(a)", scMathToDegrees, 0);
    tinyJS->addNative("function Math.toRadians(a)", scMathToRadians, 0);
    tinyJS->addNative("function Math.sin(a)", scMathSin, 0);
    tinyJS->addNative("function Math.asin(a)", scMathASin, 0);
    tinyJS->addNative("function Math.cos(a)", scMathCos, 0);
    tinyJS->addNative("function Math.acos(a)", scMathACos, 0);
    tinyJS->addNative("function Math.tan(a)", scMathTan, 0);
    tinyJS->addNative("function Math.atan(a)", scMathATan, 0);
    tinyJS->addNative("function Math.sinh(a)", scMathSinh, 0);
    tinyJS->addNative("function Math.asinh(a)", scMathASinh, 0);
    tinyJS->addNative("function Math.cosh(a)", scMathCosh, 0);
    tinyJS->addNative("function Math.acosh(a)", scMathACosh, 0);
    tinyJS->addNative("function Math.tanh(a)", scMathTanh, 0);
    tinyJS->addNative("function Math.atanh(a)", scMathATanh, 0);
       
    tinyJS->addNative("function Math.E()", scMathE, 0);
    tinyJS->addNative("function Math.log(a)", scMathLog, 0);
    tinyJS->addNative("function Math.log10(a)", scMathLog10, 0);
    tinyJS->addNative("function Math.exp(a)", scMathExp, 0);
    tinyJS->addNative("function Math.pow(a,b)", scMathPow, 0);
    
    tinyJS->addNative("function Math.sqr(a)", scMathSqr, 0);
    tinyJS->addNative("function Math.sqrt(a)", scMathSqrt, 0);    
  
}
