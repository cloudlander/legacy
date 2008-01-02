// tlm.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define DIMENSION 5
#define MAX_VALUE 50
#define MAX_LINE 255
#define long double DOUBLE
DOUBLE SB[DIMENSION+1][DIMENSION+1];

void SBL(DOUBLE Y,DOUBLE G)
{
	int i,j;
	for(i=1;i<=DIMENSION;i++)
		for(j=1;j<=DIMENSION;j++)
		{
			if(DIMENSION==j)
				SB[i][j]=Y;
			else
				SB[i][j]=1;
			SB[i][j]=SB[i][j]*2/(4+Y+G);
			if(i==j)
				SB[i][j]=SB[i][j]-1;
		}
}

DOUBLE EJ(DOUBLE IV[DIMENSION+1][MAX_VALUE][MAX_VALUE][MAX_VALUE],DOUBLE Y,DOUBLE G,UINT32 I,UINT32 J,UINT32 K,UINT32 L)
{
	DOUBLE E=0.0;
	for(int i=1;i<=4;i++)
		E+=IV[i][I][J][K];
	E+=Y*IV[DIMENSION][I][J][K];
	E=E*2/(L*(4+Y+G));
	return E;
}


int main(int argc, char* argv[])
{
	UINT32 SX,SY,SZ,ENDX,ENDY,ENDZ,X,Y,Z,T,NT,BL;
	UINT32 I,J,K,M,N;
	
	DOUBLE H,ER,UR,SGM;
	UINT32 U,V,W;
	DOUBLE RVB[DIMENSION+1][MAX_VALUE][MAX_VALUE][MAX_VALUE];
	DOUBLE IVB[DIMENSION+1][MAX_VALUE][MAX_VALUE][MAX_VALUE];

	DOUBLE EY;

	char line[MAX_LINE]="";

	FILE* input=fopen("twotlme.in","r");
	fscanf(input,"%d %d %d %d %d %d %d %d %d %e %e %e %f %f %f %f %d %d",&SX,&SY,&SZ,&ENDX,&ENDY,&ENDZ,&X,&Y,&Z,&U,&V,&W,&H,&ER,&UR,&SGM,&NT,&BL);

	H=4.0;
	ER=1.0;
	UR=1.0;
	SGM=0.0;
	U=V=W=1;
	DOUBLE U0=12566.3706144;
	DOUBLE E0=8.854187818E-2;
	DOUBLE Z0=sqrt(U0/E0);

	DOUBLE YY=2*(U*W*sqrt(H)*ER/V-2);
	DOUBLE GY=SGM*U*W*Z0/V;

	J=1;
	for(I=SX;I<=ENDX;I++)
		for(K=SZ;K<=ENDZ;K++)
			for(T=1;T<=DIMENSION;T++)
				IVB[T][I][J][K]=0;

	I=1;
	IVB[I][X][Y][Z]=1;
	
	SBL(YY,GY);

	for(T=1;T<=NT;T++)
	{
		for(I=SX;I<=ENDX;I++)
			for(K=SZ;K<=ENDZ;K++)
				for(M=1;M<=DIMENSION;M++)
				{
					RVB[M][I][J][K]=0;
					for(N=1;N<=DIMENSION;N++)
						RVB[M][I][J][K]=IVB[N][I][J][K]*SB[M][N]+RVB[M][I][J][K];
				}

		for(I=SX;I<=ENDX;I++)
		{
			for(K=SZ;K<=ENDZ;K++)
			{
				IVB[1][I][J][K]=RVB[3][I-1][J][K];
				IVB[2][I][J][K]=RVB[4][I][J][K-1];
				IVB[3][I][J][K]=RVB[1][I+1][J][K];
				IVB[4][I][J][K]=RVB[2][I][J][K+1];
				IVB[5][I][J][K]=RVB[5][I][J][K];
				
				if(1==BL)
				{
					if(K==SZ)
						IVB[2][I][J][K]=-RVB[2][I][J][K];
					if(K==ENDZ)
						IVB[4][I][J][K]=-RVB[4][I][J][K];
					if(I==SX)
						IVB[1][I][J][K]=-RVB[1][I][J][K];
					if(I==ENDX)
						IVB[3][I][J][K]=-RVB[3][I][J][K];
				}
				else if(0==BL)
				{
					if(K==SZ)
						IVB[2][I][J][K]=0;
					if(K==ENDZ)
						IVB[4][I][J][K]=0;
					if(I==SX)
						IVB[1][I][J][K]=0;
					if(I==ENDX)
						IVB[3][I][J][K]=0;
				}
			}
		}
		if(0==T%10)
		{
			fprintf(stderr,"%d\n",T);
		}
		EY=EJ(IVB,YY,GY,X,Y,Z,V);
		printf("  % #7g\n",EY);
	}
			

	return 0;
}
