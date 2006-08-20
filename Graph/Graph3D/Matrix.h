// Matrix.h: interface for the CMatrix class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MATRIX_H__EC59CE74_977C_430D_9748_1150C145453D__INCLUDED_)
#define AFX_MATRIX_H__EC59CE74_977C_430D_9748_1150C145453D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct tag3DPoint{
	float x;
	float y;
	float z;
}Point3D;

class CMatrix  
{
#define MATRIX_INT      101
#define MATRIX_FLOAT    102
typedef int MATRIX_TYPE;

public:
	CMatrix();
	CMatrix(int i_Dim,int j_Dim,MATRIX_TYPE);   // set Matrix's dimension
	CMatrix(CMatrix&);
	CMatrix(Point3D&);
	virtual ~CMatrix();
// operations:
	void Clear();		// clear all elements
	void ClearAt(int,int);  // clear one elements
	void SetAt(int,int,float);	// set one elements;
	float GetAt(int,int);
	int  GetLineDim();
	int  GetColDim();
	int  GetMatrixType();
	void SetIdentity();
// operation function:
	CMatrix operator* (CMatrix& );
	CMatrix& operator= (CMatrix&);
	CMatrix& operator= (Point3D&);

	enum{
		DIM=4
	};
	
private:
	int xDim,yDim;
	int xCur,yCur;
	int type;     // Matrix's type
// @just for debug
	float **matrixLine;  // Matrix's line
	float *matrixCol;   // Matrix's Colume

	void InitMatrix(MATRIX_TYPE);
};

#endif // !defined(AFX_MATRIX_H__EC59CE74_977C_430D_9748_1150C145453D__INCLUDED_)
