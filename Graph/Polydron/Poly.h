#ifndef _POLY_H
#define _POLY_H
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include <vector>
using namespace std;

typedef vector<int> LineList;		// ���б����������
typedef vector<int> PointList;		// ���б����������

// ��������
class AreaNode
{
//	int index;
public:
	LineList ll;
	PointList pl;
	bool bHide;

};

typedef vector<AreaNode> AreaList;

// ������������
class PolyNode
{
public:
	int index;						// �������
	AreaList al;					// ���б�
	// ��Χ
	double XMin,XMax,YMin,YMax,ZMin,ZMax;
};

typedef list<PolyNode> PolyList;	// �������б�

typedef struct tagLine
{
	int p1,p2;						// �˵�����
	bool bHide;						// �Ƿ��ڵ� true:���ڵ�
	double XMin,XMax,YMin,YMax,ZMin,ZMax;

}Line;

typedef struct tagLineRaw
{
	double x1,y1,z1;
	double x2,y2,z2;
}LineRaw;

typedef struct tagPoint
{
	double x,y,z;
}Point3D;


#endif