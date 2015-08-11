#include "Expression.h"
#include "Parser.h"
#include "Lexer.h"
#include "../network/server.h"
#include "../db/shared_ds.cc"

int yyparse(expression **exp, yyscan_t scanner);

class ClientParser {
  public:
  Server* server;
  SharedDS* shared_ds;

  void execute(expression *exp) {
    if(exp->tok_type == eVAL) return;

    expression* command = exp;
    expression* value = exp->operand;
    cout << " Value of type: ";
    switch (value->opr_type) {
      case eINT:
        cout << "INTEGER: " << value->int_val;
        break;
      case eSTRING:
        cout << "STRING: " << value->str_val;
        break;
      case eFLOAT:
        cout << "FLOAT: " << value->float_val;
        break;
      case eALL:
        cout << "ALL";
        break;
      case eSTATUS:
        cout << "STATUS";
        break;
      case eCLIENTDETAILS:
        cout << "CLIENT DETAILS";
        break;
    }
    cout << endl;
  }

  void evaluate(expression *exp) {
    if(exp == NULL) {
      cout << "Syntax Error!" << endl;
      return;
    }

    switch (exp->tok_type) {
      case eINSERT:
        cout << "INSERT command invoked!";
        execute(exp);
        break;
      case eVAL:
        cout << "Bad man!" << endl;
        break;
      case eDELETE:
        cout << "DELETE command invoked!";
        execute(exp);
        break;
      case eFIND:
        cout << "FIND command invoked!";
        execute(exp);
        break;
      case eSHOW:
        cout << "SHOW command invoked!";
        execute(exp);
        break;
    }
  }

  expression* getAST(const char *expr) {
    expression *exp;
    yyscan_t scanner;
    YY_BUFFER_STATE state;

    try {
      if (yylex_init(&scanner)) throw 1;
      state = yy_scan_string(expr, scanner);
      if (yyparse(&exp, scanner)) throw 1;
    } catch (int e) {
      exp = NULL;
    }
    yy_delete_buffer(state, scanner);
    yylex_destroy(scanner);

    return exp;
  }

};
