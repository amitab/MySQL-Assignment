#include "client_parser.h"

int main(void)
{
  ClientParser p;
  p.shared_ds = new SharedDS(100);
  std::string test;

  while(1) {
    cout << "ENTER: ";
    getline(cin, test);
    if(test == "EXIT") break;
    cout <<  p.parse_and_eval(test);
    cout << "SHOWING DATA: "; p.shared_ds->show();
  }
  delete p.shared_ds;
  return 0;
}
