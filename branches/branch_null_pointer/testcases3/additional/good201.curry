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
	Ex2[][] e;
	a=10;
	Print("HEHE");
	try{
		a=1;
		e=NewArray(10,Ex2[]);
		e[3]=NewArray(5,Ex2);
		e[3][2]=New(Ex2);
		throw e;
	}	
	catch(Ex2[][] x)
	{
		a=2;	
		Print(a);
		x[3][2].print();
		x[1][1].print();
		x[20][1].print();
	}
}
