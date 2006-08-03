class exception
{
	string error_msg;
	void set_msg(string s)
	{
		error_msg = s;
	}
	void msg()
	{	
		Print(error_msg);
	}
}

int div(int a, int b)
{
	int result;
	try
	{
		if(b == 0)
		{
			exception e;
			e = New(exception);
			e.set_msg("div by zero\n");
			throw e;
		}
		else
		{
			result = a/b;
		}
	}
	catch(exception e)
	{
		e.msg();
		return 0;
	}
	return result;
}

void main()
{
	int a;
	int b;
	Print("Enter a:");
	a = ReadInteger();
	Print("Enter b:");
	b = ReadInteger();
	Print(div(a,b), "\n");
}
