bool b;
void main() {
  int a;

  a = 1;
  while (a < 5) {
     b =  (a % 4 == 0);
     Print("Loop ", a, "\n", b, "\n");
     a = a + 1;
  }
}
