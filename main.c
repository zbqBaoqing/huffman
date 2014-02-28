/*=====================================================================
* Author:       yexingkong         :)
* Email:        yexingkong@gmail.com
* Create time:  2012-12-09 18:29
* Last change:	2012-12-09 18:30
* Filename:		main.c
* Description:
=====================================================================*/
//说明：哈夫曼树的创建及其编码实现
//编程时间：2012.10.31

#include <stdio.h>
#include<string.h>
#include "hufftree.h"

//文件压缩
int main(int argc, char **argv)
{
	int num = 0;//记录字符的种类数
	int w[N] = {0}, n = 0;
	unsigned char sdd[N]={0};
	char *file1="file.souce",*file2= "decompression.souce";
	MODE	mode[N]={0};
	hufftree ht[M+1]= {0};

	if(argc != 3 )
	{
		printf("stander:./tar -[C/D] filename\n");
		exit(1);
	}
	if( !strcmp(*(argv+1), "-C\0"))
	{
		num = readfile( w, sdd, *(argv+2) );//读文件求各字符的频率
		unsigned char *hu[num+1];//用来保存各字符的编码

		 huff_creat( w , num, ht );//哈夫曼树构建

		crthuff(hu, ht, num );//编码构建

		writefile( hu, num, sdd );//将各种字符编码保存文件

		decom_readfile(&n, mode, w) ;//读文件获取信息
		
		if(compression( num ,*(argv+2), mode ) == 0)//压缩函数
		{
			printf("%s 文件压缩成功(~:compression.tar)！\n",*(argv+2));
		}
		else
			printf("文件压缩失败!\n");
	}
	if( !strcmp(*(argv+1) ,"-D\0"))
	{
		decom_readfile(&num, mode, w) ;//解压，读文件获取信息

		huff_creat( w, num,ht );

		if(decompression( num, mode, ht, *(argv+2))==0)//解压函数
		{
			printf("compression.tar解压成功(~:decompression.souce)！\n");
		}
		else
			printf("文件解压失败！\n");
		 compare(file1,file2);
			
	}
	return 0;
}

