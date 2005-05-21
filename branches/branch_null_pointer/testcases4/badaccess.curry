class A
{
	int a()
	{
		Print("From A");
	}
}

class B extends A
{
	int a()
	{
		Print("From B");
	}
}

void func(A a)
{
	a.a();
}

A a;

void main()
{
	A b;
	b=New(B);
	func(b);
	func(a);
}
