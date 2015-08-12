#include "Expression.h"
#include "Parser.h"
#include "Lexer.h"
#include "../network/server.h"
#include "../db/shared_ds.h"

int yyparse(expression **exp, yyscan_t scanner);

class ClientParser {
  Server* server;
  ClientThread* client_thread;

  std::string insert_op(expression* value);
  std::string delete_op(expression* value);
  std::string find_op(expression* value);
  std::string show_op(expression* value);
  std::string evaluate(expression *exp);

  public:
  ClientParser() {}

  SharedDS* shared_ds;
  ClientParser(Server* server, SharedDS* shared_ds, ClientThread* client_thread) {
    this->server = server;
    this->shared_ds = shared_ds;
    this->client_thread = client_thread;
  }

  expression* getCommandTree(const char *expr);
  std::string parse_and_eval(std::string expr);
};
