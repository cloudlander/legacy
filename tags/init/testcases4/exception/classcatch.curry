//		A
//	   /|\
//	  B	C D
//	   /|\
//	  E	F G	

class A
{
	void print()
	{
		Print("class A\n");
	}
}

class B extends A
{
	void print()
	{
		Print("class B\n");
	}
}

class C extends A
{
	void print()
	{
		Print("class C\n");
	}
}

class D extends A
{
	void print()
	{
		Print("class D\n");
	}
}

class E extends C
{
	void print()
	{
		Print("class E\n");
	}
}

class F extends C
{
	void print()
	{
		Print("class F\n");
	}
}


class G extends C
{
	void print()
	{
		Print("class G\n");
	}
}

void foo()
{
	//throw New(A);
	//throw New(B);
	//throw New(C);
	//throw New(D);
	//throw New(E);
	//throw New(F);
	//throw New(G);
}

void main()
{
	try
	{
		foo();
	}
	catch(C ai)
	{
		ai.print();
	}
}