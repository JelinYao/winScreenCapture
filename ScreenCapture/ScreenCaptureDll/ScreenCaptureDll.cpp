// ScreenCaptureDll.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "ScreenCaptureDll.h"
#include "CapDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CScreenCaptureDllApp

BEGIN_MESSAGE_MAP(CScreenCaptureDllApp, CWinApp)
END_MESSAGE_MAP()


// CScreenCaptureDllApp ����

CScreenCaptureDllApp::CScreenCaptureDllApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CScreenCaptureDllApp ����

CScreenCaptureDllApp theApp;
//CCapDlg* pDlg=NULL;

// CScreenCaptureDllApp ��ʼ��

BOOL CScreenCaptureDllApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
/*inline void DeleteDlg()
{
	if(pDlg)
	{
		pDlg->DestroyWindow();
		delete pDlg;
		pDlg=NULL;
	}
}*/
/////////////////////////////////////////////////////////////////////////////////
//����ģ��ӿ�
//��ʼ����������
void InitCaptureDlg()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CCapDlg dlg;
	dlg.DoModal();
}
//����λͼ���
HBITMAP CopyBitmapHandle(HBITMAP hBmp)
{
	return CCapDlg::CopyHBitmap(hBmp);
}
//������Ļλͼ���,��ȡ��һ��Ҫ�ǵ��ͷ�
HBITMAP GetScreenBitmapHandle()
{
	return CCapDlg::GetScreenBitmap();
}
HBITMAP GetRectBitmapHandle(CRect& rc)
{
	return CCapDlg::GetScreenBitmap(rc);
}
