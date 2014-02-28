//说明：哈夫曼树的创建及其编码实现
//编程时间：2012.10.31

#ifndef HUFFTREE_H_
#define HUFFTREE_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#define N 256
#define M (2*N-1)

typedef struct huff 
{
	int		weigth; //节点的权值
	int 	parent; //双亲的下标
	int 	lchild; //左孩子下标
	int		rchild;	//右孩子下标
}hufftree; 


typedef struct bianma
{
	unsigned char	zifu;//记录字符
   	 char 	hu[20];//保存此字符的编码
}MODE;

int readfile(int w[],unsigned char sdd[] ,char *filename); //读源文件

int huff_creat( int w[] ,int num, hufftree ht[]); //w[]为储存的权值,num为权值个数
void crthuff(unsigned char *hu[],hufftree ht[], int num); //*hu[]用来带回每个权值的编码，ht[]为传入的结构体数据，num为权值个数
int  writefile(unsigned char *hu[], int num, unsigned char sdd[]); //传入已存储编码的数组hu,
void Select(hufftree ht[],int num2, int *st, int *sd); //用来选择两个权值最小且父亲为0（即此两权值未结算过）

int compression( int num, char *filename, MODE mode[] );//压缩编码

int decom_readfile( int *num, MODE mode[], int w[] );//解压，读文件获取信息

int decompression( int num, MODE  mode[], hufftree ht[], char *filename);//编码解压

int compare(char *file1,char *file2);//文件比较
#endif
