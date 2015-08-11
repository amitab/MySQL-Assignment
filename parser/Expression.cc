#include "Expression.h"
#include <stdlib.h>

expression::expression(int val) {
  int_val = val;
  opr_type = eINT;
  tok_type = eVAL;
  operand = NULL;
}

expression::expression(double val) {
  float_val = val;
  opr_type = eFLOAT;
  tok_type = eVAL;
  operand = NULL;
}

expression::expression(std::string val) {
  str_val = val;
  str_val.erase(str_val.begin());
  str_val.erase(str_val.end() - 1);
  opr_type = eSTRING;
  tok_type = eVAL;
  operand = NULL;
}

expression::expression(token_type tok_type, expression* operand) {
  this->tok_type = tok_type;
  this->operand = operand;
}

expression::expression(operand_type opr_type) {
  this->opr_type = opr_type;
  this->tok_type = eVAL;
  this->operand = NULL;
}

