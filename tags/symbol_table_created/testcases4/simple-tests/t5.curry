void main()
{
	int i;
	int n;

	Print("Enter a number: ");
	n = ReadInteger();
 	Print("You entered ", n, ".\nWatch me count:\n");
	for (i = 1; i <= n; i = i + 1) {
		Print(i, "\n");
      }
}