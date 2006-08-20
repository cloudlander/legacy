#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>
#include "Matrix.h"

Matrix::Matrix(int l)
{
	int i;
	level=l;
	for(i=0;i<10;i++)
		col_mark[i]=1;
	
	
}

Matrix::~Matrix()
{
}

int Matrix::Lplas(int cur_row)
{
	int i;
	int sum=0;
	int count=1;
	//递归出口
	if(cur_row==level-1)
	{
		for(i=0;i<level;i++)
			if(col_mark[i]>0)  //一阶矩阵
				return mat[cur_row][i];
	}

	for(i=0;i<level;i++)  //遍历所有col
	{
		if(col_mark[i]>0)  //current col is in
		{
			col_mark[i]=0;  //cast current col
			if(mat[cur_row][i])  //if 0 : not neccessary to call next
			{   //calculate and call next
				sum+=(count%2 ? 1 : -1)*Lplas(cur_row+1)*mat[cur_row][i]; 				
			}
			col_mark[i]=1;  //restore current col
			count++;  //子阵序号
		}
	}
	return sum;  //current matrix 's value
}


void Matrix::input()
{
	int i,j;
	cout<<"Enter your Matrix: (level is"<<level<<")";
	for(i=0;i<level;i++)
		for(j=0;j<level;j++)
			cin>>mat[i][j];
}


void Matrix::output()
{
	int i,j;
	printf("Your Matrix is:");
	printf("\n");
	for(i=0;i<level;i++)
	{
		for(j=0;j<level;j++)
			printf("%3d",mat[i][j]);
		printf("\n");
	}
}






