/////////////////////////////////////////////////////////////////////////
///@system 交易引擎系统
///@company 上海量投网络科技有限公司
///@file platform.cpp
///@brief实现跨平台需要的函数
///@history 
///20080603 huwp		创建，增加WIN32下的getopt函数
#include "platform.h"

#ifdef WIN32
char	*optarg;		// global argument pointer
int		optind = 0; 	// global argv index

int getopt(int argc, char *argv[], char *optstring)
{
	static char *next = NULL;
	if (optind == 0)
		next = NULL;

	optarg = NULL;

	if (next == NULL || *next == ('\0'))
	{
		if (optind == 0)
			optind++;

		if (optind >= argc || argv[optind][0] != ('-') || argv[optind][1] == ('\0'))
		{
			optarg = NULL;
			if (optind < argc)
				optarg = argv[optind];
			return EOF;
		}

		if (strcmp(argv[optind], ("--")) == 0)
		{
			optind++;
			optarg = NULL;
			if (optind < argc)
				optarg = argv[optind];
			return EOF;
		}

		next = argv[optind];
		next++;		// skip past -
		optind++;
	}

	char c = *next++;
	char *cp = strchr(optstring, c);

	if (cp == NULL || c == (':'))
		return ('?');

	cp++;
	if (*cp == (':'))
	{
		if (*next != ('\0'))
		{
			optarg = next;
			next = NULL;
		}
		else if (optind < argc)
		{
			optarg = argv[optind];
			optind++;
		}
		else
		{
			return ('?');
		}
	}

	return c;
}
#endif

