int a;
string[] b;

int[] tester(int d) {
  b = NewArray(1, string);
  return NewArray(d, int);
}

void main() {
  int a;
  bool b;
  int[] d;
  a = 1;
  while (a < 5) {
     double b;
     if (a % 2 == 0) {
        d = tester(a);
        break;
    }
     Print("Loop ", a, "\n");
     a = a + 1;
  }
  d[0] = 0;
  Print(d[d[0]], "\n", d.length(), "\n");
}
