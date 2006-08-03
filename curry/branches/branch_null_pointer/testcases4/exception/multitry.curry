void foo()
{
	try
	{
		try
		{
			throw 3;
		}
		catch(int a)
		{
			Print(a, "\n");
			throw a >3;
		}
	}
	catch(bool b)
	{
		Print(b, "\n");
		throw "haha";
	}
	catch(int a)
	{
		Print(a, "\n");
	}	
}

void main()
{
	foo();
}