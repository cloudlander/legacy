void bar()
{
	throw 3;
}

void foo()
{
	bar();
}

void main()
{
	try
	{
		foo();
	}
	catch(int a)
	{
		Print(a, "\n");
	}
}