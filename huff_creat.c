/*=====================================================================
* Author:       yexingkong         :)
* Email:        yexingkong@gmail.com
* Create time:  2012-12-07 08:48
* Last change:	2012-12-11 00:48
* Filename:		huff_creat.c
* Description:哈夫曼树的创建及其编码的实现函数
=====================================================================*/


#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>
#include"hufftree.h"


//自定义错误处理函数
void my_error(char *err_string, int line)
{
	fprintf(stderr,"line: %d\n",line);
	perror("err_string");
	printf("err_string: %s with error:  %d\n", strerror(errno),errno);

}


int readfile(int w[], unsigned char sdd[], char	*filename )
{
	FILE	*fp;
	unsigned char	ch;
	int 	num = 0;//记录字符种类数
	int de[N] = {0};//用来记录每种字符的出现频率
	int i= 0, t = 1;


	fp = fopen(filename, "r");
	if( fp == NULL)
	{
		my_error("readfile",__LINE__);
		exit(1);
	}
	ch = fgetc(fp);
	while( !feof(fp))	
	{
		de[ch]++; // 记录每种字符的出现频率
//		printf("ch=%d\n",ch);
//		sleep(1);
//		system("clear");
		ch = fgetc(fp);
	}
	for(i =0 ;i < N; i++)
	{
		if( de[i] != 0)
		{
			w[t] = de[i]; 
			sdd[t-1] = i;
			t++;
			num++;
		}
	}

		fclose(fp);

	fp = fopen("info.code","w");

	if( fp == NULL )
	{
		my_error("fopen",__LINE__);
		exit(1);
	}
	fprintf(fp,"%d\n",num);//字符种类数存入文件
//	printf("num=%d\n",num);
	for( i = 1; i < t; i++)
	{
		fprintf(fp,"%d\n",w[i]);//各种字符的频率存入文件
	//	printf(" sd= %d w=%d i= %d\n",sdd[i-1],w[i], i);
	}
	fclose(fp);
	

	return (num);
}




//哈夫曼编码的实现

void crthuff(unsigned char *hu[], hufftree ht[],int num ) //hu存放编码,ht是结构体，num初始结点数
{
	unsigned char *cd;
	int start, p, c, i ;
	cd  = (char *)malloc((num+1)*sizeof(char));
	cd[num-1] = '\0'; //在num-1空间就放字符结束符，是因为当只有一个结点时，@处不用复制，假如在num处放'\0'，那第一个字符将放在num-2处，num-1就会别空置，导致以后读取错误

	for( i = 1; i <= num; i++)
	{
		start = num - 1;
		c = i;
		p = ht[i].parent;
		while( p != 0)
		{
			start--;
			if( ht[p].lchild == c) //判断c是否为左孩子
				cd[start] = '0';
			else
				cd[start] = '1';
			c = p;  //第推，父亲作孩子
			p = ht[p].parent; //求父亲的父亲
		}
		hu[i] = (char *)malloc((num-start)*sizeof(char));
		strcpy(hu[i], &cd[start]); //@
	}

	free(cd);


/*	printf("权值及其对应的编码\n");
	for(i = 1; i <= num; i++)
	{
		printf("%d  %s\n", ht[i].weigth,hu[i]);
	}
*/
}

int writefile(unsigned char *hu[],int num ,unsigned char sdd[])//将哈夫曼编码存入文件
{
	int i, t;
	FILE	*fp;

	fp = fopen("info.code","a");

	if( fp == NULL )
	{
		my_error("fopen",__LINE__);
		exit(1);
	}

	for(i = 1; i <= num; i++)
	{
		t = 0;
		fprintf(fp,"%c ",sdd[i-1]);//将每种字符存入文件
		//		printf("w_sd=%c \n",sdd[i-1]);
		fprintf(fp,"%s",hu[i]);
		fputc('\n',fp);
	}
	
		fclose(fp);
		return 0;
}


//选择数组中两个最小的数并返回
void Select(hufftree ht[],int num2, int *st, int *sd )
{
	int i, j, p1[2] = {0}, k,  t= 0 ;

	for( i = 1; i <= num2; i++)
	{
		k = i;
		if((ht[k].parent == 0) && (k != p1[0]))
		{
		for( j = i+1; j <= num2 ; j++)
		{
			if( (j != p1[0]) && (ht[k].weigth > ht[j].weigth) &&(ht[j].parent == 0))
				k = j;
		}
		if( t == 2)
			break;
		p1[t++] = k; //记录ht[]中最小的两数的下标
		}
	}

	*st = p1[0];
	*sd = p1[1];
}


//根据叶子节点建立哈夫曼树
int huff_creat( int w[] ,int num ,hufftree ht[]) //传入num个权值，w[]保存权值
{
	int	i , j, m, n;
	int	st = 0, sd = 0;
	

	for( i = 1; i <= num; i++ ) //1~num号单元存放叶子节点的,初始化
	{
		ht[i].weigth = w[i];
		ht[i].parent = 0;
		ht[i].lchild = 0;
		ht[i].rchild = 0;
	}

	m = 2 * num - 1; //num+1 ~ m号单元存放非叶子节点

	for(j = num+1; j <= m; j++) //对num+1 ~ m号单元初始化
	{
		ht[j].weigth = 0;
		ht[j].parent = 0;
		ht[j].lchild = 0;
		ht[j].rchild = 0;
	}

	for(i = num +1; i <= m; i++) //再对ht[]的后 num+1 ~m号单元赋值
	{
		Select( ht, i - 1, &st, &sd ); //函数调用，选择从ht[1] ~ht[i-1]中两个最小值

		ht[i].weigth = ht[st].weigth + ht[sd].weigth;
		ht[st].parent = i;
		ht[sd].parent = i;
		ht[i].lchild = st;
		ht[i].rchild = sd;
//		printf("w=%d  p= %d  l=%d  r=%d \n", ht[i].weigth,ht[i].parent, ht[i].lchild, ht[i].rchild);
	}
//	printf("\n");
//	for(i = 1; i<= m; i++)
//		printf("w=%d  p= %d  l=%d  r=%d i=%d\n", ht[i].weigth,ht[i].parent, ht[i].lchild, ht[i].rchild, i);

		 return 0;
//creat end
}

//压缩编码
int compression( int num, char *filename,MODE mode[] )
{
	FILE	*fp, *fb;
	int i;
	char    t = 0, x = 0;
	unsigned char 	ch, b = 0;
	char  w = 0;
	
	
	fp = fopen("compression.tar","w");
	if( fp == NULL)
	{
		my_error("fopen",__LINE__);
		exit(1);
	}
	fclose(fp);

	fp = fopen(filename,"r");
	if( fp == NULL)
	{
		my_error("fopen",__LINE__);
		exit(1);
	}
	fb = fopen("compression.tar","w");
	if( fb == NULL)
	{
		my_error("fopen",__LINE__);
		exit(1);
	}

	ch = fgetc(fp);
	while( !feof(fp))//读文件中的字符
	{
		for( i = 0; i < num; i++)
		{
			if( ch == mode[i].zifu)
			{	//printf("mod_zi=%d  ch=%d\n",mode[i].zifu, ch);
				for( t = 0; t < strlen(mode[i].hu); t++)
				{//	printf("mod_m=%s\n",mode[i].hu);
					if(  mode[i].hu[t] != '0')
					{
						w<<=1; //位操作，以位存储::先移
						w +=1;//::后加
						x++;
					}
					else
					{
						w<<=1;
						x++;

					}
					if( x == 8 )
					{
					
						fprintf(fb,"%c",w);
						x = 0;
						w = 0;
					}
				}
				break;
			}
		}
		ch = fgetc(fp);
	}
		fclose(fp);
	 t= x;
	if( 0< x && x < 8)
	{
		while( x != 8)
		{
			w<<=1;
			x++;
		}
		fprintf(fb,"%c",w);
		b= 8 - t + 1;
		fprintf(fb,"%c",t);
	//	printf("x= %d\n",t);
	}
	else
		fprintf(fb,"%c",'0');
	
	fclose(fb);
	return 0;
}


//压缩-解压过程中读文件
int decom_readfile(int *num, MODE mode[], int w[])
{
	FILE 	*fp;
	int  i, t = 0;
	unsigned char  ch;

	fp = fopen("info.code","r");

	if(fp == NULL)
	{
		my_error("fopen",__LINE__);
		exit(1);
	}
	fscanf(fp,"%d",num);
	printf("de_num=%d\n",*num);
	fgetc(fp);
	for(i = 1; i <= *num; i++ )
	{
		fscanf(fp,"%d",&w[i]);
		fgetc(fp);
	}

	for(i = 0; i < *num; i++)
	{
		mode[i].zifu = fgetc(fp);//保存字符，图片中含有字符串，不是单个字符,所以不能读图片
	//	printf("rea=%d ",mode[i].zifu);
	    fgetc(fp);//读空格
		fscanf(fp,"%s",mode[i].hu); //读图片，错误
	//	printf("%s   i =%d\n",mode[i].hu, i);
		fgetc(fp);
	}	
	fclose(fp);
	return 0;
}


//编码解压
int decompression( int num, MODE mode[], hufftree ht[] ,char *filename)
{
		int 	i, j, t, F = 2 * num - 1 ;
		FILE   *fb;
		int 	p,l, e, wq = 0, fp;
	    unsigned char	w = 128, ch, m;
		char	cd[9]={0} , x;
	
	fb = fopen("decompression.souce","w");
	if( fb == NULL)
	{
		my_error("fopen",__LINE__);
		exit(1);
	}
	fclose(fb);

/*	for(i = 0; i < num; i++)
		printf("dec=%c\n",mode[i].zifu);
	for(i = 1; i<= 2*num-1; i++)
		printf("i= %d  w=%d  p= %d  l=%d  r=%d \n",i, ht[i].weigth,ht[i].parent, ht[i].lchild, ht[i].rchild);*/

		fp = open(filename,O_RDONLY );

		if( fp == -1 )
		{
			my_error("open",__LINE__);
			exit(1);
		}
	
		t = 0;
		e = F;
		j = 0;
		if((wq = lseek(fp, 0,SEEK_END)) == -1)
		{
			my_error("lseek",__LINE__);
			exit(1);
		}
			lseek(fp,-1,SEEK_END);
			read(fp,&x,1,1);//读取标记数
	//		printf("x=%d\n",x);
			lseek(fp,0,SEEK_SET);
			memset(cd,0,sizeof(char));	

			fb = fopen( "decompression.souce", "w");
			if(fb == NULL)
			{
				my_error("fopen",__LINE__);
				exit(1);
			}
	//printf(" wq=%d\n",wq);
		for( i = 0; i < wq - 1; i++)
		{
			read(fp,&ch,1,1);

			for(j = 0; j < 8; j++)
			{
				if( (ch & w) == 0) //获取一字节8位中的最高位数为0
				{
					m = 0;
					ch <<=1;
				}
				else if( (ch & w) == w)//最高位为1
				{
					m = 1;
					ch <<= 1;
				}

				if(m == 0)
				{
					p = ht[e].lchild;
					e = p;
				}
				else if( m == 1)
				{
					p = ht[e].rchild;
					e = p;
				}
				if( (ht[e].lchild == 0) || (ht[e].rchild == 0))
				{
					cd[t++] = mode[e-1].zifu;//找到字符
					e = F;
				}
				if( t == 8)
				{
			
					fwrite(cd,8,1,fb);
					memset(cd,0,sizeof(char)*9);
					t = 0;
				}

				if( ( i == wq - 2  ) && (j == x - 1) )
				{
					if( t != 8)
					{
						fwrite(cd,t,1,fb);
	//					printf("com=%s\n",cd);
					}

					fclose(fb);
					break;
				}
			}

		}

		close(fp);
		return 0;
}



int compare(char *file1,char *file2)
{
	FILE 	*fp,*fb;
	int i,j;
	char 	s1[11]={0}, s2[11]={0};
	char    ch, cd;

	fp = fopen(file1,"r");
	if( fp == NULL)
	{
		my_error("fopen",__LINE__);
		exit(1);
	}
	fb = fopen(file2,"r");
	if( fp == NULL)
	{
		my_error("fopen",__LINE__);
		exit(1);
	}
	while(((fgets(s1,10,fp)) != NULL) && ((fgets(s2,10,fb)) != NULL) )
	{
			if( strcmp(s1, s2) != 0)
			{
				printf("文件内容不同!\n");
				for( i=0; i < strlen(s1);i++)
				printf("file1: %d\n",s1[i]);
				for( j=0; j<strlen(s2);j++)
				printf("file2: %d\n",s2[j]);
				return 1;
			}
			memset(s1,0,sizeof(char)*11);
			memset(s2,0,sizeof(char)*11);
	}
	if( ((ch=fgetc(fp)) == EOF) && ((cd=fgetc(fb)) == EOF) )
	{
		printf("文件相同!\n");
		return 0;
	}
	else
	{
		printf("文件内容d不同！\n");;
		for( i=0; i < strlen(s1);i++)
			printf("file1: %c\n",s1[i]);
		for( j=0;j< strlen(s2);j++)
			printf("file2: %c\n",s2[j]);
/*		s1[strlen(s1)+1] = '\0';
		s2[strlen(s2)+1]= '\0';
		printf("file1: %s\n",s1);
		printf("file2: %s\n",s2);*/
		return 1;
	}
	fclose(fp);
	fclose(fb);
}
