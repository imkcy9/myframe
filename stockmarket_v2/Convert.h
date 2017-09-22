#ifndef _CONVERT_H_
#define _CONVERT_H_

#include <string>

namespace Onv
{

enum CharsetType
{
	CHARSETTYPE_UNKNOW,
	CHARSETTYPE_UTF8,
	CHARSETTYPE_GBK,
	CHARSETTYPE_GB2312
};

// 编码格式和名称结构体
struct CharsetTypeName
{
	CharsetType type;
	char name[256];
};

class CConvert
{
public:
	CConvert();
	~CConvert();

	// 代码转换:从UTG-8编码转为GBK编码
	static std::string Utf82Gbk(char* pszBuf);
	// 代码转换:从UTG-8编码转为GBK编码
	static std::string Gbk2Utf8(char* pszBuf);

	// 代码转换:从一种编码转为另一种编码
	static std::string Convert(const CharsetType from, const CharsetType to, char* inbuf);
	// 代码转换:从一种编码转为另一种编码
	static std::string Convert(const CharsetType from, const CharsetType to, char* inbuf, const size_t inlen);
	// 代码转换:从一种编码转为另一种编码
	static bool Convert(const CharsetType from, const CharsetType to, char* inbuf, size_t inlen, char* outbuf, size_t& outlen);
	// 通过编码格式获取编码名称
	static const char* GetCharsetName(const CharsetType type);
private:
	// 编码格式和名称对应关系
	static CharsetTypeName s_CharsetTypeName[];
};

}

#endif
