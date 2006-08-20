#ifndef _POLY_H
#define _POLY_H
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include <vector>
using namespace std;

typedef vector<int> LineList;		// 边列表的索引数组
typedef vector<int> PointList;		// 点列表的索引数组

// 面结点类型
class AreaNode
{
//	int index;
public:
	LineList ll;
	PointList pl;
	bool bHide;

};

typedef vector<AreaNode> AreaList;

// 多面体结点类型
class PolyNode
{
public:
	int index;						// 多面体号
	AreaList al;					// 面列表
	// 范围
	double XMin,XMax,YMin,YMax,ZMin,ZMax;
};

typedef list<PolyNode> PolyList;	// 多面体列表

typedef struct tagLine
{
	int p1,p2;						// 端点索引
	bool bHide;						// 是否被遮挡 true:被遮挡
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