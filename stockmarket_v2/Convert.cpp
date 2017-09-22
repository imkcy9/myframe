
#include <iconv.h>
#include <string.h>
#include <limits.h>
#include <assert.h>

#include "Convert.h"

namespace Onv
{

// 编码格式和名称对应关系
CharsetTypeName CConvert::s_CharsetTypeName[] = {
		{CHARSETTYPE_UNKNOW, "unknow" },
		{CHARSETTYPE_UTF8,   "utf-8"  },
		{CHARSETTYPE_GBK,    "gbk"    },
		{CHARSETTYPE_GB2312, "gb2312" }
};

CConvert::CConvert()
{

}

CConvert::~CConvert()
{

}

// 代码转换:从UTG-8编码转为GBK编码
std::string CConvert::Utf82Gbk(char* pszBuf)
{
	return CConvert::Convert(CHARSETTYPE_UTF8, CHARSETTYPE_GBK, pszBuf);
}

// 代码转换:从UTG-8编码转为GBK编码
std::string CConvert::Gbk2Utf8(char* pszBuf)
{
	return CConvert::Convert(CHARSETTYPE_GBK, CHARSETTYPE_UTF8, pszBuf);
}

// 代码转换:从一种编码转为另一种编码
std::string CConvert::Convert(const CharsetType from, const CharsetType to, char* inbuf)
{
	assert(NULL != inbuf);
	return CConvert::Convert(from, to, inbuf, strlen(inbuf));
}

// 代码转换:从一种编码转为另一种编码
std::string CConvert::Convert(const CharsetType from, const CharsetType to, char* inbuf, const size_t inlen)
{
	assert(NULL != inbuf);
	char szCharsetTo[USHRT_MAX] = {'\0'};
	size_t len = sizeof(szCharsetTo);
	CConvert::Convert(from, to, inbuf, inlen, szCharsetTo, len);
	return szCharsetTo;
}

// 代码转换:从一种编码转为另一种编码
bool CConvert::Convert(const CharsetType from, const CharsetType to, char* inbuf, size_t inlen, char* outbuf, size_t& outlen)
{
	assert(NULL != inbuf);
	iconv_t cd = iconv_open(CConvert::GetCharsetName(to), CConvert::GetCharsetName(from));
	if (0 == cd)
	{
		return false;
	}
	memset(outbuf, 0, outlen);
	if ((size_t)-1 == iconv(cd, &inbuf, &inlen, &outbuf, &outlen))
	{
		iconv_close(cd);
		return false;
	}
	else
	{
		iconv_close(cd);
		return true;
	}
}

// 通过编码格式获取编码名称
const char* CConvert::GetCharsetName(const CharsetType type)
{
	const char* pszCharsetName = NULL;

	const unsigned int uiCount = sizeof(CConvert::s_CharsetTypeName)/sizeof(CharsetTypeName);
	for (unsigned int uiIndex=0; uiIndex<uiCount; uiIndex++)
	{
		if (type == CConvert::s_CharsetTypeName[uiIndex].type)
		{
			pszCharsetName = CConvert::s_CharsetTypeName[uiIndex].name;
			break;
		}
		else if (CHARSETTYPE_UNKNOW == CConvert::s_CharsetTypeName[uiIndex].type)
		{
			pszCharsetName = CConvert::s_CharsetTypeName[uiIndex].name;
		}
	}
	return pszCharsetName;
}

}

