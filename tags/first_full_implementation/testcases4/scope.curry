a a;
void main() {
  int[] c;
  string s;
  {
 	int c;
	int x;
	x=200;
	Print(x,"\n");
  }
  s = "hello";
  c = test(4, 5);
  c = NewArray(5,int);
  c[3]=100;
  c[2]=c[3]+1;
  Print(c[2]);
  Print(s);
}

int[] test(int a, int b) {
//*
  switch(a)
{
	case 0:{
		int abc;
		int xyz;
		while(abc){
		test(abc,xyz);
	   if(abc)
		break;
		} 
		Print(xyz);
	}
	case 1:
{
break;
}
default:{
	int abc1;
	int xyz1;
	switch(abc1)
	{	
		case 0:
		break;

	}
	}
}
//*/
  return a + b;
}
