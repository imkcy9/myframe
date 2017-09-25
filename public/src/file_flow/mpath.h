/////////////////////////////////////////////////////////////////////////
///@system 交易引擎系统
///@company 上海量投网络科技有限公司
///@file mpath.h
///@brief 定义了跨平台的路径管理
///@history 
///20060303	huwp		创建该文件
/////////////////////////////////////////////////////////////////////////

#ifndef MPATH_H
#define MPATH_H

#ifdef WINDOWS
#define PATH_SPLIT '\\'
#endif

#ifdef UNIX
#define PATH_SPLIT '/'
#endif

#define ALL_SPLITS "\\/$"

#define MAX_PATH_LEN 200
	
///转换路径
///@param	target	目标
///@param	source	源
void convertPath(char *target, const char *source);
	
///替换标准的fopen函数
FILE *mfopen(const char *filename, const char *mode);

#endif
