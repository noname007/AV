
// mfc-av.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CmfcavApp:
// �йش����ʵ�֣������ mfc-av.cpp
//

class CmfcavApp : public CWinApp
{
public:
	CmfcavApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CmfcavApp theApp;