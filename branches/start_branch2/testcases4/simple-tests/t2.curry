int globalCounter;

void main() {
  int a;
  string b;
  bool c;

  c = true;
  b = "wow!";
  a = 3;
  Print(a, b, c);
  a = a * 5;
  c = a < 10;
  globalCounter = a + 3 * 6 + 2;
  Print(globalCounter, c, a);
}  
