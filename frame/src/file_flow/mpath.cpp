/////////////////////////////////////////////////////////////////////////
///@system 交易引擎系统
///@company 上海量投网络科技有限公司
///@file mpath.h
///@brief 实现了跨平台的路径管理
///@history 
///20140105	hwp		创建该文件
/////////////////////////////////////////////////////////////////////////

#include "platform.h"

void convertPath(char *target, const char *source)
{
	const char *s;
	char *t;
	for (s=source, t=target; ((s-source)<MAX_PATH_LEN) && (*s!='\0'); s++, t++)
	{
		if (strchr(ALL_SPLITS,*s)!=NULL)
		{
			*t=PATH_SPLIT;
		}
		else
		{
			*t=*s;
		}
	}
	*t='\0';
}
	
FILE *mfopen(const char *filename, const char *mode)
{
	char actualName[MAX_PATH_LEN+1];
	convertPath(actualName,filename);
	return fopen(actualName,mode);
}
