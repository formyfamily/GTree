#pragma once 
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<vector>
#include<ctime>
#include<fstream>
#include<algorithm>
#include<map>
#include<cmath>
#include<queue>
#include<sys/time.h>
#include<metis.h>
#include<unistd.h>

const bool DEBUG_ = false;
const bool Optimization_G_tree_Search = true;			 //是否开启全连接加速算法
const bool Optimization_KNN_Cut = true;					 //是否开启KNN剪枝查询算法
const bool Optimization_Euclidean_Cut = false;			 //是否开启Catch查询中基于欧几里得距离剪枝算法
const char Edge_File[] = "data//road.nedge";			 //第一行两个整数n,m表示点数和边数，接下来m行每行三个整数U,V,C表示U->V有一条长度为C的边
const char Node_File[] = "data//road.cnode";			 //共N行每行一个整数两个实数id,x,y表示id结点的经纬度(但输入不考虑id，只顺序从0读到n-1，整数N在Edge文件里)
const int Global_Scheduling_Cars_Per_Request = 30000000; //每次规划精确计算前至多保留的车辆数目(时间开销)
const double Unit = 0.1;								 //路网文件的单位长度/m
const double R_earth = 6371000.0;						 //地球半径，用于输入经纬度转化为x,y坐标
const double PI = acos(-1.0);
const int Partition_Part = 4;		//K叉树
const int Naive_Split_Limit = 33;   //子图规模小于该数值全划分
const int INF = 0x3fffffff;			//无穷大常量
const bool RevE = true;			//false代表有向图，true代表无向图读入边复制反向一条边
const bool Distance_Offset = false; //KNN是否考虑车辆距离结点的修正距离
const bool DEBUG1 = false;

struct G_Tree ;
struct Graph ;

void init() ;//初始化
void read() ;//读取 const char Edge_File[] 路网图边权值文件

void save() ;//将 gptree 存储如文件"GP_Tree.data"
void load() ;
void build() ;

int search(int S, int T) ; //查询路网点 S、T 之间的最短路,返回值为距离
int search_catch(int S,int T) ;//带 cache 的两点最短路,速度慢于 search,频繁查询速度会提高
int find_path(int S,int T,std::vector<int> &order) ;//返回 S-T 最短路长度,并将沿途经过的结点存储到 order 数组中

int Gn() ;
int Grealnode() ;