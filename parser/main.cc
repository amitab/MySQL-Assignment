#include "client_parser.cc"

int main(void)
{
  ClientParser p;
  expression *e = NULL;
  std::string test;

  while(1) {
    cout << "ENTER: ";
    getline(cin, test);
    if(test == "EXIT") break;
    test += "\n";
    e = p.getAST(test.c_str());
    p.evaluate(e);
    if(e != NULL) delete e;
  }

  return 0;
}
