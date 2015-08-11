#include "Expression.h"
#include "Parser.h"
#include "Lexer.h"
#include "../network/server.h"
#include "../db/shared_ds.cc"

int yyparse(expression **exp, yyscan_t scanner);

class ClientParser {
  Server* server;

  std::string insert_op(expression* value) {
    if(value == NULL || value->tok_type != eVAL) return "Syntax Error!";

    int status = false;
    switch (value->opr_type) {
      case eINT:
        cout << "INTEGER: " << value->int_val;
        status = shared_ds->insert_data(value->int_val);
        break;
      case eSTRING:
        cout << "STRING: " << value->str_val;
        status = shared_ds->insert_data(value->str_val);
        break;
      case eFLOAT:
        cout << "FLOAT: " << value->float_val;
        status = shared_ds->insert_data(value->float_val);
        break;
    }

    if(status) {
      return "Operation performed successfuly!";
    } else {
      return "Operation failed!";
    }
  }

  std::string delete_op(expression* value) {
    if(value == NULL || value->tok_type != eVAL) return "Syntax Error!";

    switch (value->opr_type) {
      case eINT:
        cout << "INTEGER: " << value->int_val;
        shared_ds->delete_data(value->int_val);
        break;
      case eSTRING:
        cout << "STRING: " << value->str_val;
        shared_ds->delete_data(value->str_val);
        break;
      case eFLOAT:
        cout << "FLOAT: " << value->float_val;
        shared_ds->delete_data(value->float_val);
        break;
      case eALL:
        cout << "ALL!";
        shared_ds->delete_all();
        break;
    }

    return "Operation performed successfuly!";
  }

  std::string find_op(expression* value) {
    if(value == NULL || value->tok_type != eVAL) return "Syntax Error!";
    std::stringstream response;
    response << "FOUND VALUE ";
    bool found = false;

    switch (value->opr_type) {
      case eINT:
        cout << "INTEGER: " << value->int_val;
        found = shared_ds->confirm_data(value->int_val);
        response << value->int_val;
        break;
      case eSTRING:
        cout << "STRING: " << value->str_val;
        found = shared_ds->confirm_data(value->str_val);
        response << value->str_val;
        break;
      case eFLOAT:
        cout << "FLOAT: " << value->float_val;
        found = shared_ds->confirm_data(value->float_val);
        response << value->float_val;
        break;
    }

    if(found) return response.str();
    else return "NOT FOUND";
  }

  std::string show_op(expression* value) {
    if(value == NULL || value->tok_type != eVAL) return "Syntax Error!";

    switch (value->opr_type) {
      case eSTATUS:
        return shared_ds->status();
        break;
      case eCLIENTDETAILS:
        return server->get_clients_connected();
        break;
    }
  }

  std::string evaluate(expression *exp) {
    if(exp == NULL) {
      return "Syntax Error!";
    }

    switch (exp->tok_type) {
      case eINSERT:
        return insert_op(exp->operand);
        break;
      case eDELETE:
        return delete_op(exp->operand);
        break;
      case eFIND:
        return find_op(exp->operand);
        break;
      case eSHOW:
        return show_op(exp->operand);
        break;
    }
  }

  public:
  ClientParser() {}

  SharedDS* shared_ds;
  ClientParser(Server* server, SharedDS* shared_ds) {
    this->server = server;
    this->shared_ds = shared_ds;
  }

  expression* getCommandTree(const char *expr) {
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

  std::string parse_and_eval(const char *expr) {
    expression* e = getCommandTree(expr);
    std::string response = evaluate(e);
    delete e;
    return response;
  }

};
