#include "client_parser.cc"

int main(void)
{
  ClientParser p;
  p.shared_ds = new SharedDS(100);
  std::string test;

  while(1) {
    cout << "ENTER: ";
    getline(cin, test);
    if(test == "EXIT") break;
    test += "\n";
    cout <<  p.parse_and_eval(test.c_str());
    cout << "SHOWING DATA: "; p.shared_ds->show();
  }
  delete p.shared_ds;
  return 0;
}
