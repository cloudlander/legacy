// tlm.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#ifdef PARALLEL_OMP
#include <omp.h>
#endif

#define DIMENSION 5
#define DIMENSION2 9
#define MAX_VALUE 100
#define MAX_CUBE MAX_VALUE*MAX_VALUE*MAX_VALUE
#define MAX_LINE 255
#define C 3E8
#define PI 3.1415926


#define DOUBLE long double

DOUBLE SB[DIMENSION+1][DIMENSION+1];
DOUBLE SB2[DIMENSION2+1][DIMENSION2+1];

DOUBLE RVB[DIMENSION2+1][MAX_CUBE];
DOUBLE IVB[DIMENSION2+1][MAX_CUBE];

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
/*
    for(j=1;j<=DIMENSION;j++)
        SB[0][j]=SB[5][j];
    for(i=1;i<=DIMENSION;i++)
        SB[i][0]=SB[i][5];
*/
}

//void SBL2(DOUBLE ER,DOUBLE UR,DOUBLE E0,DOUBLE U0,DOUBLE Z0)
void SBL2(DOUBLE Z0,DOUBLE Zp,DOUBLE Zs,DOUBLE Z0l,DOUBLE Zn1,DOUBLE Zn2)
{

    int i,j;

    DOUBLE a,b,c,d,e,f,g,h,p,q;

    a=(Zs+Zn1-Z0l)/(Zs+Zn1+Z0l);
    b=(Z0l+Zn1-Zs)/(Z0l+Zn1+Zs);
    c=(Zn2-Zp)/(Zn2+Zp);
    d=2*Zn1*Z0l/((Z0l+Zs+Zn1)*(Z0l+Zs));
    e=2*Z0l/(Zs+Zn1+Z0l);
    f=2*Zs/(Zs+Zn1+Z0l);
    g=2*Zn1*Zs/((Z0l+Zs+Zn1)*(Z0l+Zs));
    h=2*Zn1/(Z0l+Zs+Zn1);
    p=2*Zn2*Z0l/((Z0l+Zs)*(Zp+Zn2));
    q=2*Zn2*Zs/((Z0l+Zs)*(Zp+Zn2));

    for(i=1;i<=4;i++)
        for(j=1;j<=4;j++)
            if(i==j)
                SB2[i][j]=a;
            else
                SB2[i][j]=d;

    for(i=1;i<=4;i++)
        for(j=5;j<=8;j++)
            if(i+4==j)
                SB2[i][j]=e;
            else
                SB2[i][j]=-d;

    for(i=5;i<=8;i++)
        for(j=1;j<=4;j++)
            if(i-4==j)
                SB2[i][j]=f;
            else
                SB2[i][j]=-g;

    for(i=5;i<=8;i++)
        for(j=5;j<=8;j++)
            if(i==j)
                SB2[i][j]=b;
            else
                SB2[i][j]=g;

    for(i=1;i<=4;i++)
        SB2[i][9]=p;

    for(i=5;i<=8;i++)
        SB2[i][9]=-q;

    for(i=1;i<=4;i++)
        SB2[9][i]=h;

    for(i=5;i<=8;i++)
        SB2[9][i]=-h;

    SB2[9][9]=c;
}


DOUBLE EJ(DOUBLE Y,DOUBLE G,UINT32 I,UINT32 J,UINT32 K,UINT32 L)
{
	DOUBLE E=0.0;
	for(int i=1;i<=4;i++)
		E+=IVB[i][I*MAX_VALUE*MAX_VALUE+J*MAX_VALUE+K];
	E+=Y*IVB[DIMENSION][I*MAX_VALUE*MAX_VALUE+J*MAX_VALUE+K];
//    E+=Y*IVB[0][I*MAX_VALUE*MAX_VALUE+J*MAX_VALUE+K];
	E=E*2/(L*(4+Y+G));
	return E;
}

DOUBLE EJ2(DOUBLE Zp,DOUBLE Zs,DOUBLE Z0l,DOUBLE Zn1,DOUBLE Zn2,UINT32 I,UINT32 J,UINT32 K)
{
	DOUBLE E=0.0;
    DOUBLE E1=0.0;
    DOUBLE E2=0.0;
	for(int i=1;i<=8;i++)
		E1+=IVB[i][I*MAX_VALUE*MAX_VALUE+J*MAX_VALUE+K];
    E2=2*(Z0l+Zs)*IVB[9][I*MAX_VALUE*MAX_VALUE+J*MAX_VALUE+K];
    E=(2*Zp*E1+E2)/(4*Zp+Z0l+Zs);
	return E;
}


int main(int argc, char* argv[])
{
	INT32 SX,SY,SZ,ENDX,ENDY,ENDZ,X,Y,Z,T,NT,BL;
	INT32 I,J,K,M,N;
	
	DOUBLE H,ER,UR,SGM,ERM,URM;
	UINT32 U,V,W;

	char line[MAX_LINE]="";

    clock_t start,finish,tstart,tfinish;

    FILE* data_output=fopen("ey.out","w");
    FILE* plot_output=fopen("result.gnu","w");

    fprintf(plot_output,"set samples 100, 100\n");
    fprintf(plot_output,"set isosamples 10, 10\n");
    fprintf(plot_output,"set surface\n");
    fprintf(plot_output,"set title \"TLM 2D\"\n");
    fprintf(plot_output,"set xrange [ 2.00000 : 32.0000 ] noreverse nowriteback\n");
    fprintf(plot_output,"set yrange [ 2.00000 : 32.0000 ] noreverse nowriteback\n");
    fprintf(plot_output,"set zrange [ -0.0100000 : 0.0100000 ] noreverse nowriteback\n");
    fprintf(plot_output,"set cbrange [ -0.0100000 : 0.0100000 ] noreverse nowriteback\n");
    fprintf(plot_output,"set zero 1e-0020\n");
    fprintf(plot_output,"set pm3d at s\n");
    fprintf(plot_output,"set dgrid3d 60,60\n");

    fprintf(stdout,"set samples 100, 100\n");
    fprintf(stdout,"set isosamples 10, 10\n");
    fprintf(stdout,"unset autoscale\n");
    fprintf(stdout,"set surface\n");
    fprintf(stdout,"set title \"TLM 2D\"\n");
    fprintf(stdout,"set xrange [ 2.00000 : 32.0000 ] noreverse nowriteback\n");
    fprintf(stdout,"set yrange [ 2.00000 : 32.0000 ] noreverse nowriteback\n");
    fprintf(stdout,"set zrange [ -0.0300000 : 0.0300000 ] noreverse nowriteback\n");
    fprintf(stdout,"set cbrange [ -0.0300000 : 0.0300000 ] noreverse nowriteback\n");
    fprintf(stdout,"set zero 1e-0020\n");
    fprintf(stdout,"set pm3d at s\n");
    fprintf(stdout,"set dgrid3d 60,60\n");

	FILE* input=fopen("twotlme.in","r");
	fscanf(input,"%d %d %d %d %d %d %d %d %d %d %d %d %lf %lf %lf %lf %d %d %lf %lf",&SX,&SY,&SZ,&ENDX,&ENDY,&ENDZ,&X,&Y,&Z,&U,&V,&W,&H,&ER,&UR,&SGM,&NT,&BL,&ERM,&URM);
	fclose(input);

    DOUBLE U0=4*PI*1E-7;
	DOUBLE E0=8.854187818E-12;
	DOUBLE Z0=sqrt(U0/E0);

	DOUBLE YY=2*(U*W*sqrt(H)*ER/V-2);
	DOUBLE GY=SGM*U*W*Z0/V;

	J=10;
	for(I=SX;I<=ENDX;I++)
		for(K=SZ;K<=ENDZ;K++)
			for(T=1;T<=DIMENSION2;T++)
				IVB[T][I*MAX_VALUE*MAX_VALUE+J*MAX_VALUE+K]=0;

/*
	I=1;
	IVB[I][X*MAX_VALUE*MAX_VALUE+Y*MAX_VALUE+Z]=1;
*/
         
    J=10;        

    DOUBLE SQUARE2=sqrt(2.0);
    DOUBLE Z0l=SQUARE2*Z0;
    DOUBLE Vl=SQUARE2*C;
    DOUBLE dl=0.001;
    DOUBLE En=E0;
    DOUBLE Un=U0;
    DOUBLE Em=E0*ERM;
    DOUBLE Um=U0*URM;
    DOUBLE POW2W=pow(2*PI*1E10,2);
    DOUBLE C0=-1/(POW2W*(Um-Un)*dl);
    DOUBLE L0=-1/(POW2W*(Em-En)*dl);

    DOUBLE Zs=dl/(4*C0*Vl);
    DOUBLE Zp=2*L0*Vl/dl;
    DOUBLE Zn1=Zp*(Zs+Z0l)/(3*Zp+Zs+Z0l);
    DOUBLE Zn2=(Zs+Z0l)/4;




    SBL2(Z0,Zp,Zs,Z0l,Zn1,Zn2);
	SBL(YY,GY);

#ifdef PARALLEL_OMP
    omp_set_num_threads(2);
#endif

    start=clock();

	for(T=1;T<=NT;T++)
	{
        tstart=clock();
        printf("% # .7f\n",6.283E10-(double)6.283E10/(2*PI)*2*PI);
        // ¼¤Àø
    	IVB[1][X*MAX_VALUE*MAX_VALUE+Y*MAX_VALUE+Z]=sin(6.283E10*T);
    	IVB[2][X*MAX_VALUE*MAX_VALUE+Y*MAX_VALUE+Z]=sin(6.283E10*T);
    	IVB[3][X*MAX_VALUE*MAX_VALUE+Y*MAX_VALUE+Z]=sin(6.283E10*T);
    	IVB[4][X*MAX_VALUE*MAX_VALUE+Y*MAX_VALUE+Z]=sin(6.283E10*T);

    #pragma omp parallel for private(I)
		for(I=SX;I<=ENDX;I++)
        {
			for(K=SZ;K<=ENDZ;K++)
			{
                if(K>=SZ+(ENDZ-SZ)/3 && K<=SZ+(ENDZ-SZ)/3*2)
//                if(I>=14 && K>= 2 && I<=20 && K<=32)
                {
				    for(M=1;M<=DIMENSION2;M++)
				    {
					    RVB[M][I*MAX_VALUE*MAX_VALUE+J*MAX_VALUE+K]=0;
					    for(N=1;N<=DIMENSION2;N++)
						    RVB[M][I*MAX_VALUE*MAX_VALUE+J*MAX_VALUE+K]=IVB[N][I*MAX_VALUE*MAX_VALUE+J*MAX_VALUE+K]*SB2[M][N]+RVB[M][I*MAX_VALUE*MAX_VALUE+J*MAX_VALUE+K];
				    }
                }
                else
                {
				    for(M=1;M<=DIMENSION;M++)
				    {
					    RVB[M][I*MAX_VALUE*MAX_VALUE+J*MAX_VALUE+K]=0;
					    for(N=1;N<=DIMENSION;N++)
						    RVB[M][I*MAX_VALUE*MAX_VALUE+J*MAX_VALUE+K]=IVB[N][I*MAX_VALUE*MAX_VALUE+J*MAX_VALUE+K]*SB[M][N]+RVB[M][I*MAX_VALUE*MAX_VALUE+J*MAX_VALUE+K];
				    }
                }
			}
        }

		for(I=SX;I<=ENDX;I++)
		{
			for(K=SZ;K<=ENDZ;K++)
			{

				IVB[1][I*MAX_VALUE*MAX_VALUE+J*MAX_VALUE+K]=RVB[3][(I-1)*MAX_VALUE*MAX_VALUE+J*MAX_VALUE+K];
				IVB[2][I*MAX_VALUE*MAX_VALUE+J*MAX_VALUE+K]=RVB[4][I*MAX_VALUE*MAX_VALUE+J*MAX_VALUE+K-1];
				IVB[3][I*MAX_VALUE*MAX_VALUE+J*MAX_VALUE+K]=RVB[1][(I+1)*MAX_VALUE*MAX_VALUE+J*MAX_VALUE+K];
				IVB[4][I*MAX_VALUE*MAX_VALUE+J*MAX_VALUE+K]=RVB[2][I*MAX_VALUE*MAX_VALUE+J*MAX_VALUE+K+1];
 			    
                IVB[5][I*MAX_VALUE*MAX_VALUE+J*MAX_VALUE+K]=RVB[5][I*MAX_VALUE*MAX_VALUE+J*MAX_VALUE+K];

               if(K>=SZ+(ENDZ-SZ)/3 && K<=SZ+(ENDZ-SZ)/3*2)
                {
				    IVB[6][I*MAX_VALUE*MAX_VALUE+J*MAX_VALUE+K]=RVB[6][I*MAX_VALUE*MAX_VALUE+J*MAX_VALUE+K];
				    IVB[7][I*MAX_VALUE*MAX_VALUE+J*MAX_VALUE+K]=RVB[7][I*MAX_VALUE*MAX_VALUE+J*MAX_VALUE+K];
				    IVB[8][I*MAX_VALUE*MAX_VALUE+J*MAX_VALUE+K]=RVB[8][I*MAX_VALUE*MAX_VALUE+J*MAX_VALUE+K];
				    IVB[9][I*MAX_VALUE*MAX_VALUE+J*MAX_VALUE+K]=-RVB[9][I*MAX_VALUE*MAX_VALUE+J*MAX_VALUE+K];
                }
                /*
                else
                {
                    IVB[0][I*MAX_VALUE*MAX_VALUE+J*MAX_VALUE+K]=RVB[0][I*MAX_VALUE*MAX_VALUE+J*MAX_VALUE+K];
                }
                */

                if(1==BL)
				{
					if(K==SZ)
						IVB[2][I*MAX_VALUE*MAX_VALUE+J*MAX_VALUE+K]=-RVB[2][I*MAX_VALUE*MAX_VALUE+J*MAX_VALUE+K];
					if(K==ENDZ)
						IVB[4][I*MAX_VALUE*MAX_VALUE+J*MAX_VALUE+K]=-RVB[4][I*MAX_VALUE*MAX_VALUE+J*MAX_VALUE+K];
					if(I==SX)
						IVB[1][I*MAX_VALUE*MAX_VALUE+J*MAX_VALUE+K]=-RVB[1][I*MAX_VALUE*MAX_VALUE+J*MAX_VALUE+K];
					if(I==ENDX)
						IVB[3][I*MAX_VALUE*MAX_VALUE+J*MAX_VALUE+K]=-RVB[3][I*MAX_VALUE*MAX_VALUE+J*MAX_VALUE+K];
				}
				else if(0==BL)
				{
					if(K==SZ)
						IVB[2][I*MAX_VALUE*MAX_VALUE+J*MAX_VALUE+K]=0;
					if(K==ENDZ)
						IVB[4][I*MAX_VALUE*MAX_VALUE+J*MAX_VALUE+K]=0;
					if(I==SX)
						IVB[1][I*MAX_VALUE*MAX_VALUE+J*MAX_VALUE+K]=0;
					if(I==ENDX)
						IVB[3][I*MAX_VALUE*MAX_VALUE+J*MAX_VALUE+K]=0;
				}

			}
		}
        
        tfinish=clock();    

		if(0==T%10)
		{
			fprintf(stderr,"%d\n",T);
		}

        /*
		EY=EJ(YY,GY,X,Y,Z,V);
		printf(" % #.8f\n",EY);
        */
//        fprintf(data_output,"#T=%d\n",T);
		for(I=SX;I<=ENDX;I++)
        {
			for(K=SZ;K<=ENDZ;K++)
			{
                if(K>=SZ+(ENDZ-SZ)/3 && K<=SZ+(ENDZ-SZ)/3*2)
//                    fprintf(data_output,"%d %d % #.8f\n",K,I,EJ2(Zp,Zs,Z0l,Zn1,Zn2,I,J,K));
                    fprintf(data_output,"  % #.7f\n",EJ2(Zp,Zs,Z0l,Zn1,Zn2,I,J,K));
  //                    fprintf(data_output,"%i %d %d % #.8f\n",T,K,I,EJ(YY,GY,I,J,K,V));
                else
//                    fprintf(data_output,"%d %d % #.8f\n",K,I,EJ(YY,GY,I,J,K,V));
                    fprintf(data_output,"  % #.7f\n",EJ(YY,GY,I,J,K,V));

            }
        }
        fprintf(data_output,"\n\n");

        if(tfinish-tstart<0.1)  
        {
            fprintf(stdout,"splot \"ey.out\" index %d\n",T);
            fprintf(stdout,"pause 0.1\n");
        }
        else
        {
            fprintf(stdout,"splot \"ey.out\" index %d\n",T);
        }

         fprintf(plot_output,"splot \"ey.out\" index %d\n",T);
         fprintf(plot_output,"pause 0.1\n");
	}

    finish=clock();
    fprintf(stdout,"Total Computation Time: %#.8f\n",(double)((finish-start)/CLK_TCK));
			
	return 0;
}
