// ffffff-1.cpp : �������̨Ӧ�ó������ڵ㡣
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

