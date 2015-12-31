// ffffff-1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
//#include "libavcodec/avcodec.h"
extern "C"{
	#include "libavcodec/avcodec.h"
};

int _tmain(int argc, _TCHAR* argv[])
{
	printf("%s",avcodec_configuration());
	system("pause");
	return 0;
}

