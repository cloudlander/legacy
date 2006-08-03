class Ex1
{
	void print()
	{
  		Print("Ex1\n");
	}
}

class Ex2 extends Ex1
{
	void print()
	{
		Print("Ex2\n");
	}
}

void main()
{
	int a;
	int b;
	a=10;
	Print("HEHE");
	try{
		a=b=10;
		throw New(Ex2);
	}	
	catch(Ex1 x)
	{
		a=2;	
		Print(a);
		x.print();
	}
}
