int a;
string[] b;

int[] tester(int sz) {
  int i;
  int[] result;
  b = NewArray(1, string);
  result = NewArray(sz, int);
  for (i = 0; i < sz; i = i + 1)
	result[i] = i;
  b[0] = "Done";
  return result;
}

void main() {
  int[] d;
  d = tester(8);
  Print(d[d[1]], "\n", d.length(), "\n");
  Print(b[0], "\n");
}
