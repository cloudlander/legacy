// Matrix.h: interface for the CMatrix class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MATRIX_H__EC59CE74_977C_430D_9748_1150C145453D__INCLUDED_)
#define AFX_MATRIX_H__EC59CE74_977C_430D_9748_1150C145453D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Point3D{
public:
	double x;
	double y;
	double z;
public:
	Point3D(){};
	Point3D(Point3D& right)
	{
		x=right.x;
		y=right.y;
		z=right.z;
	}
	Point3D(float px,float py,float pz)
	{
		x=px;
		y=py;
		z=pz;
	}
	Point3D& operator=(Point3D& right)
	{
		x=right.x;
		y=right.y;
		z=right.z;
		return *this;
	}
};

template <class T> class CMatrixT  
{

public:
	CMatrixT();
	CMatrixT(int Row_Dim,int Col_Dim);   // set Matrix's dimension
	CMatrixT(CMatrixT<T>&);
//	CMatrix(Point3D&);
	virtual ~CMatrixT();

// operations:
	void Transform(CMatrixT<T>&);
	void Clear();		// clear all elements
	void ClearAt(int row,int col);  // clear one elements
	void SetAt(int,int,const T&);	// set one elements;
//	void SetAt(int,int,T);
	T& GetAt(int,int) const;
	T GetAt(int,int) ;
	int  GetRowDim();
	int  GetColDim();
	void SetIdentity();

// operation function:
	CMatrixT<T> operator* (CMatrixT<T>& );
	CMatrixT<T>& operator= (CMatrixT<T>&);
//	CMatrixT<T>& operator= (Point3D&);

	enum{
		DIM=4
	};
	
private:
	int RowDim,ColDim;
	int RowCur,ColCur;
// @just for debug
	T **MatrixRow;  // Matrix's line
	T *MatrixCol;   // Matrix's Colume

	void InitMatrix();
};
#ifdef _DEBUG
#define new DEBUG_NEW
//#undef THIS_FILE
//static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
template <class T>
CMatrixT<T>::CMatrixT()
{
	RowDim=ColDim=DIM;
	MatrixCol=NULL;
	MatrixRow=NULL;
	InitMatrix();
}

template <class T>
CMatrixT<T>::CMatrixT(int dimx,int dimy)
{
	RowDim=dimx;
	ColDim=dimy;
	InitMatrix();
}

template <class T>
CMatrixT<T>::CMatrixT(CMatrixT<T>& R_Matrix)
{
	RowDim=R_Matrix.GetRowDim();
	ColDim=R_Matrix.GetColDim();
	InitMatrix();
	*this=R_Matrix;
}

/*
template <class T>
CMatrixT<T>::CMatrix(Point3D& point)
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
*/

template <class T>
CMatrixT<T>::~CMatrixT()
{

	if(MatrixRow!=NULL){
		for(int i=0;i<RowDim;i++)
			delete[] MatrixRow[i];
		delete[] MatrixRow;
	} 

}

template <class T>
void CMatrixT<T>::InitMatrix()
{
	int i,j;
	MatrixRow=new T* [RowDim];
	for(j=0;j<RowDim;j++)
		MatrixRow[j]=NULL;
	for(i=0;i<RowDim;i++)
	{
		MatrixCol=new T[ColDim];
		for(j=0;j<ColDim;j++)
			MatrixCol[j]=0;
//			memset(matrixCol,0,yDim);
		MatrixRow[i]=MatrixCol;
	}
	
}
template <class T>
void CMatrixT<T>::Transform(CMatrixT<T>& left)
{
	int i,j;
	CMatrixT<T> temp(ColDim,RowDim);
	for(i=0;i<ColDim;i++)
		for(j=0;j<RowDim;j++)
			temp.SetAt(i,j,MatrixRow[j][i]);
	left= temp;
}

template <class T>
void CMatrixT<T>::Clear()
{
	for(int i=0;i<RowDim;i++)
		memset(MatrixRow[i],0,ColDim);
}

template <class T>
void CMatrixT<T>::ClearAt(int x,int y)
{
	memset(MatrixRow[x][y],0,1);
}

template <class T>
void CMatrixT<T>::SetAt(int x,int y,const T& elem)
{
	MatrixRow[x][y]=(T)elem;
}
/*
template <class T>
void CMatrixT<T>::SetAt(int x,int y,T elem)
{
	MatrixRow[x][y]=(T)elem;
}
*/
template <class T>
T& CMatrixT<T>::GetAt(int x,int y) const
{
	return MatrixRow[x][y];
}

template <class T>
T CMatrixT<T>::GetAt(int x,int y)
{
	return MatrixRow[x][y];
}

template <class T>
int CMatrixT<T>::GetRowDim()
{
	return RowDim;
}

template <class T>
int CMatrixT<T>::GetColDim()
{
	return ColDim;
}


template <class T>
CMatrixT<T> CMatrixT<T>::operator*(CMatrixT<T>& right)
{
	int i,j,k;
	T s=0;
	CMatrixT<T> temp(RowDim,ColDim);
	for(i=0;i<RowDim;i++)
	{
		for(j=0;j<ColDim;j++)
		{
			for(s=0,k=0;k<ColDim;k++)
			{
				s+=MatrixRow[i][k]*right.GetAt(k,j);
			}
			temp.SetAt(i,j,s);
		}
	}
	return CMatrixT<T>(temp);
}

template <class T>
CMatrixT<T>& CMatrixT<T>::operator=(CMatrixT<T>& right)
{
	int i,j;
	for(i=0;i<RowDim;i++)
		for(j=0;j<ColDim;j++)
			MatrixRow[i][j]=right.GetAt(i,j);
		
	return *this;
}

/*
template <class T>
CMatrix& CMatrix::operator=(Point3D& point)
{
	ASSERT(xDim==1&&yDim==4);
	matrixLine[0][0]=point.x;
	matrixLine[0][1]=point.y;
	matrixLine[0][2]=point.z;
	matrixLine[0][3]=1;
	return *this;
}
*/

template <class T>
void CMatrixT<T>::SetIdentity()
{
	if(RowDim!=ColDim)  return;
	for(int i=0;i<RowDim;i++)
		MatrixRow[i][i]=1;
}
#endif // !defined(AFX_MATRIX_H__EC59CE74_977C_430D_9748_1150C145453D__INCLUDED_)
