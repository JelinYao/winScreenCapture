// ScreenCaptureDll.h : ScreenCaptureDll DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CScreenCaptureDllApp
// �йش���ʵ�ֵ���Ϣ������� ScreenCaptureDll.cpp
//

class CScreenCaptureDllApp : public CWinApp
{
public:
	CScreenCaptureDllApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
