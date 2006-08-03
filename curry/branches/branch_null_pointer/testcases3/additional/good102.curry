void main()
{
	int[][] x;
	int i;
	int j;
	x=NewArray(10,int[]);
	for(i=0;i<10;i=i+1)
	{
		x[i]=NewArray(7,int);
		for(j=0;j<7;j=j+1)
		{
			x[i][j]=i*j+1;
		}
	}
	Print(x.length(),x[3].length());
}
