#include "client_parser.h"

std::string ClientParser::insert_op(expression* value) {
  if(value == NULL || value->tok_type != eVAL) return "Syntax Error!";

  int status = false;
  switch (value->opr_type) {
    case eINT:
      status = shared_ds->insert_data(value->int_val);
      break;
    case eSTRING:
      status = shared_ds->insert_data(value->str_val);
      break;
    case eFLOAT:
      status = shared_ds->insert_data(value->float_val);
      break;
  }

  if(status) {
    return "Operation performed successfuly!";
  } else {
    return "Operation failed!";
  }
}

std::string ClientParser::delete_op(expression* value) {
  if(value == NULL || value->tok_type != eVAL) return "Syntax Error!";

  switch (value->opr_type) {
    case eINT:
      shared_ds->delete_data(value->int_val);
      break;
    case eSTRING:
      shared_ds->delete_data(value->str_val);
      break;
    case eFLOAT:
      shared_ds->delete_data(value->float_val);
      break;
    case eALL:
      shared_ds->delete_all();
      break;
  }

  return "Operation performed successfuly!";
}

std::string ClientParser::find_op(expression* value) {
  if(value == NULL || value->tok_type != eVAL) return "Syntax Error!";
  std::stringstream response;
  response << "FOUND VALUE ";
  bool found = false;

  switch (value->opr_type) {
    case eINT:
      found = shared_ds->confirm_data(value->int_val);
      response << value->int_val;
      break;
    case eSTRING:
      found = shared_ds->confirm_data(value->str_val);
      response << value->str_val;
      break;
    case eFLOAT:
      found = shared_ds->confirm_data(value->float_val);
      response << value->float_val;
      break;
  }

  if(found) return response.str();
  else return "NOT FOUND";
}

std::string ClientParser::show_op(expression* value) {
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

std::string ClientParser::evaluate(expression *exp) {
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
    case eADMIN:
      if(exp->operand->str_val == "password") {
        client_thread->admin = true;
        return "Authorized";
      }
      else return "Unauthorized";
      break;
    default:
      return "UNKNOWN!";
      break;
  }
}

expression* ClientParser::getCommandTree(const char *expr) {
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

std::string ClientParser::parse_and_eval(std::string expr) {
  expr += "\n";
  expression* e = getCommandTree(expr.c_str());
  std::string response = evaluate(e);
  delete e;
  return response;
}
