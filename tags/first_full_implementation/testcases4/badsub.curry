void main()
{
   int[] arr;
   int i;

   arr = NewArray(10, int);
   for (i = 0; i <= 10; i = i + 1) {
      arr[i] = i;
      Print(i, "\n");
  }
  Print("Done\n");
}
