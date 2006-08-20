// Matrix.cpp: implementation of the CMatrix class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Graph3D.h"
#include "Matrix.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMatrix::CMatrix()
{
	xDim=yDim=DIM;
	type=MATRIX_INT;
	matrixCol=NULL;
	matrixLine=NULL;
	InitMatrix(type);
}

CMatrix::CMatrix(int dimx,int dimy,int M_Type)
{
	xDim=dimx;
	yDim=dimy;
	type=M_Type;
	InitMatrix(type);
}

CMatrix::CMatrix(CMatrix& R_Matrix)
{
	xDim=R_Matrix.GetLineDim();
	yDim=R_Matrix.GetColDim();
	type=R_Matrix.GetMatrixType();
	InitMatrix(type);
	*this=R_Matrix;
}

CMatrix::CMatrix(Point3D& point)
{
	xDim=1;
	yDim=4;
	type=MATRIX_FLOAT;
	InitMatrix(type);
	matrixLine[0][0]=point.x;
	matrixLine[0][1]=point.y;
	matrixLine[0][2]=point.z;
	matrixLine[0][3]=1;
}

CMatrix::~CMatrix()
{

	if(matrixLine!=NULL){
		for(int i=0;i<xDim;i++)
			delete[] matrixLine[i];
		delete[] matrixLine;
	}

}

void CMatrix::InitMatrix(int M_Type)
{
	int i,j;
	switch(M_Type)
	{
	case MATRIX_FLOAT:
		matrixLine=new float*[xDim];
		for(j=0;j<xDim;j++)
			matrixLine[j]=NULL;
		for(i=0;i<xDim;i++)
		{
			matrixCol=new float[yDim];
			for(j=0;j<yDim;j++)
				matrixCol[j]=0;
//			memset(matrixCol,0,yDim);
			matrixLine[i]=matrixCol;
		}
		break;
	case MATRIX_INT:
		break;
	}
	
}

void CMatrix::Clear()
{
	for(int i=0;i<xDim;i++)
		memset(matrixLine[i],0,yDim);
}

void CMatrix::ClearAt(int x,int y)
{
	matrixLine[x][y]=0;
}

void CMatrix::SetAt(int x,int y,float elem)
{
	switch(type)
	{
	case MATRIX_INT:
//		matrixLine[x][y]=(int)(*elem);
		break;
	case MATRIX_FLOAT:
		matrixLine[x][y]=(float)(elem);
		break;
	}

}

float CMatrix::GetAt(int x,int y)
{
	return (float)matrixLine[x][y];
}

int CMatrix::GetLineDim()
{
	return xDim;
}

int CMatrix::GetColDim()
{
	return yDim;
}

int CMatrix::GetMatrixType()
{
	return type;
}

CMatrix CMatrix::operator*(CMatrix& right)
{
	int i,j,k;
	float s=0;
	CMatrix temp(xDim,yDim,type);
	for(i=0;i<xDim;i++)
	{
		for(j=0;j<yDim;j++)
		{
			for(s=0,k=0;k<yDim;k++)
			{
				s+=matrixLine[i][k]*right.GetAt(k,j);
			}
			temp.SetAt(i,j,s);
		}
	}
	return CMatrix(temp);
}

CMatrix& CMatrix::operator=(CMatrix& right)
{
	int i,j;
	for(i=0;i<xDim;i++)
		for(j=0;j<yDim;j++)
			matrixLine[i][j]=right.GetAt(i,j);

	return *this;
}

CMatrix& CMatrix::operator=(Point3D& point)
{
	ASSERT(xDim==1&&yDim==4);
	matrixLine[0][0]=point.x;
	matrixLine[0][1]=point.y;
	matrixLine[0][2]=point.z;
	matrixLine[0][3]=1;
	return *this;
}

void CMatrix::SetIdentity()
{
	if(xDim!=yDim)  return;
	for(int i=0;i<xDim;i++)
		matrixLine[i][i]=1;
}