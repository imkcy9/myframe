/////////////////////////////////////////////////////////////////////////
///@system 交易引擎系统
///@company 上海量投网络科技有限公司
///@file system.h
///@brief实现与网络通讯有关的系统函数，以及CByte、CWord、CInt这三个类
///@history 
///20020214	huwp		创建该文件
/////////////////////////////////////////////////////////////////////////

#include "platform.h"
#include "system.h"

int checkSystem(void)
{
	/*
	typedef struct
	{
		CByte a;
		CWord b;
		CInt c;
	}	testStruct;
	if (sizeof(CByte)!=1)
		return 0;
	if (sizeof(CWord)!=2)
		return 0;
	if (sizeof(CInt)!=4)
		return 0;
	if (sizeof(testStruct)!=7)
		return 0;
	if (getEndian()==_UNKNOWN_ENDIAN_)
		return 0;
	*/
	return 1;
}
/*
int getEndian(void)
{
	static int endian=_UNKNOWN_ENDIAN_;
	union
	{
		char a[4];
		int b;
	}	u;
	if (endian!=_UNKNOWN_ENDIAN_)
		return endian;
	u.a[0]=1;
	u.a[1]=2;
	u.a[2]=3;
	u.a[3]=4;
	if (u.b==0x01020304)
	{
		endian=_BIG_ENDIAN_;
	}
	else if (u.b==0x04030201)
	{
		endian=_SMALL_ENDIAN_;
	}
	return endian;
}

void CByte::setValue(int v)
{
	value=v;
}

int CByte::getValue(void)
{
	return value;
}

void CWord::setValue(int v)
{
	if (getEndian()==_SMALL_ENDIAN_)
	{
		value=	((v&0x000000ff)<<8)|
				((v&0x0000ff00)>>8);
	}
	else
	{
		value=v;
	}
}

int CWord::getValue(void)
{
	if (getEndian()==_SMALL_ENDIAN_)
	{
		return	((value&0x000000ff)<<8)|
				((value&0x0000ff00)>>8);
	}
	else
		return value;
}

void CInt::setValue(int v)
{
	if (getEndian()==_SMALL_ENDIAN_)
	{
		value=	((v&0x000000ff)<<24) |
				((v&0x0000ff00)<<8)  |
				((v&0x00ff0000)>>8)  |
				((v&0xff000000)>>24);
	}
	else
		value=v;
}

int CInt::getValue(void)
{
	if (getEndian()==_SMALL_ENDIAN_)
	{
		return	((value&0x000000ff)<<24) |
				((value&0x0000ff00)<<8)  |
				((value&0x00ff0000)>>8)  |
				((value&0xff000000)>>24);
	}
	else
		return value;
}
*/
