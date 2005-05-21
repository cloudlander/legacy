class A
{
	bool y;

	void set(bool x)
	{
		y=x;
	}

	bool get()
	{
		return y;
	}

	B[][] func(int m,int n)
	{
		B[][] y;
		y=NewArray(m,B[]);
		y[m/2]=NewArray(n,B);
		y[m/2][n/2]=New(B);
		y[m/2][n/2].init();
		y[m/2][n/2].set(true);
		return y;
	}	
}

class B extends A
{
	int[] x;
	void init()
	{
		y=false;
		x=NewArray(20,int);
		x[7]=153;
	}

	int GetX(int i)
	{
		return x[i];
	}
}

void main()
{
	A x;
	B[][] y;
	x=New(A);
	x.set(false);
	y=x.func(10,7);
	Print(x.get(),y[5][3].GetX(7),y[5][3].get());

	Print(x.func(100,70)[50][35].get(),x.func(10,10)[5].length());
	Print(x.func(100,70)[50][35].GetX(7));
	Print(x.func(100,70)[50][35].GetX(7));

	Print(x.func(100,70)[50][35].get(),x.func(10,10)[5].length());
	Print(x.func(100,70)[50][35].GetX(7));
	Print(x.func(100,70)[50][35].GetX(7));
}
