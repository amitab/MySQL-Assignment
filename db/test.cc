#include <iostream>
using namespace std;

class X {
  public:
  X* h;
  int i;

  X(int j) {h = NULL; i = j;}
  void test(X*& t) {
    h->h = t;
  }
};



int main() {
  X *target,*parent,*temp;
  
  target = new X(1);
  parent = new X(2);
  temp = new X(3);

  parent->h = target;
  target->h = parent;

  cout << target->i << endl;
  cout << temp->i << endl;
  cout << parent->i << endl;

  target->test(temp);
  cout << target->i << endl;
  cout << temp->i << endl;
  cout << parent->i << endl;
  return 0;
}
