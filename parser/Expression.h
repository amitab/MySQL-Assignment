/*
 * Expression.h
 * Definition of the structure used to build the syntax tree.
 */
#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

#include <string>
#include <cstring>
#include <iostream>

/**
 * @brief The operation type
 */
enum operand_type {
  eINT,
  eFLOAT,
  eSTRING,
  eALL,
  eSTATUS,
  eCLIENTDETAILS
};

enum token_type {
  eVAL,
  eINSERT,
  eDELETE,
  eFIND,
  eSHOW
};

class expression {
  public:
  token_type tok_type;
  operand_type opr_type;
  std::string str_val;
  int int_val;
  float float_val;
  expression* operand;

  expression(int val);
  expression(double val);
  expression(std::string val);
  expression(operand_type opr_type);
  expression(token_type tok_type, expression* operand);

  ~expression() {
    if(tok_type != eVAL && operand != NULL) delete operand;
  }
};

#endif // __EXPRESSION_H__
