class A
{
	int x;
	int a()
	{
		Print("From A");
	}
}

class B extends A
{
	int a()
	{
		x=1;
		Print("From B");
	}
}

void func(A a)
{
	a.a();
}

A a;

int main()
{
	A b;
	int i;
	try{
	b=New(B);
	func(b);
	switch(3)
	{
		case 3:
			{
				Print(3);
			}
		case 4:
			{
				Print(4);
			}
		case 5:
			{
				break;
			}
		default:
			{
				Print("ABC");
			}
	}
	}
	catch(A x)
	{

	}
	return 7;
}
