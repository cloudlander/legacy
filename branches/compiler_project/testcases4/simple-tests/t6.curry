string c;

void main() {
  int a;
  a = 0;
  
  while (a != 10) {
    Print(a, " ");
    a = a + 1;
  }
  Print("\na now = ", a, " which is ");
  if (a % 2 == 0)
	Print("even\n");
  else
	Print("odd\n");

  a = 0;
  c = "Done.\n";
  while (true) break;
  while (a != 10) {
    Print(a, " ");
    a = a + 1;
    if (a == 7)
       Print(a);
    else
      break;
  }

  for (a = 0; a != 10; 1) {
    bool c;
    Print(a, " ");
    a = a + 1;
    c = false;
    if (a > 7) 
       if (a < 10)
	    c = c || a % 2 == 0;
    if (c) break;
  }
  Print(c);
}







