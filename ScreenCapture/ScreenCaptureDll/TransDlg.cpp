// TransDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TransDlg.h"
#include "ScreenCaptureDll.h"

// CTransDlg �Ի���

IMPLEMENT_DYNAMIC(CInfoDlg, CDialog)

CInfoDlg::CInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInfoDlg::IDD, pParent)
{
	m_rValue=m_gValue=m_bValue=0;
	m_nWidth=m_nHeight=0;
	m_hBmp=NULL;
	m_hMemDC=NULL;
}

CInfoDlg::~CInfoDlg()
{
	if(m_hMemDC)
	{
		DeleteDC(m_hMemDC);
		m_hMemDC=NULL;
	}
	if(m_hBmp)
	{
		DeleteObject(m_hBmp);
		m_hBmp=NULL;
	}
}

void CInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CInfoDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CTransDlg ��Ϣ�������

BOOL CInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
////////////////////////////////////////////////////////////////////////////
	//����͸���Ի���
	COLORREF maskColor = RGB(255,255,255);//RGB(236,233,216);
	SetWindowLong(m_hWnd,GWL_EXSTYLE,GetWindowLong(m_hWnd,GWL_EXSTYLE)|0x80000);
	HINSTANCE hInst=LoadLibrary(_T("User32.DLL"));
	if(hInst)
	{
		typedef BOOL (WINAPI * MYFUNC)(HWND , COLORREF,BYTE,DWORD);
		MYFUNC fun=NULL;
		fun=(MYFUNC)GetProcAddress(hInst,"SetLayeredWindowAttributes");
		if(fun) fun(m_hWnd,maskColor,128,LWA_COLORKEY);
		FreeLibrary(hInst);
	}
	m_rcClient=CRect(0,0,180,160);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CInfoDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	dc.FillSolidRect(&m_rcClient,RGB(255,255,255));
	dc.FrameRect(&m_rcClient,&CBrush(RGB(0,0,200)));
	DrawText(&dc);
	DrawBitmap(&dc);
	DrawLine(&dc);
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
}

void CInfoDlg::DrawText(CDC* pDC)
{
	CRect rcText;
	rcText.left=2;
	rcText.top=86;
	rcText.right=m_rcClient.Width();
	rcText.bottom=m_rcClient.Height();
	CFont ft,*pOldFont;
	ft.CreatePointFont(120,_T("����"));
	pOldFont=pDC->SelectObject(&ft);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(255,0,0));
	CString strRect,strColor;
	strRect.Format(_T("��ǰ�����С:\n         %d  ��  %d\n"),m_nWidth,m_nHeight);
	strColor.Format(_T("  RGB(%d,%d,%d)"),m_rValue,m_gValue,m_bValue);
	CString text=strRect+strColor+_T("\n˫�����������Ƶ�������");
	pDC->DrawText(text,&rcText,DT_LEFT);
	pDC->SelectObject(pOldFont);
	ft.DeleteObject();
}

BOOL CInfoDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//pDC->FillSolidRect(&client,RGB(155,0,200));
    return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}

void CInfoDlg::DrawBitmap(CDC* pDC)
{
	if(m_hBmp==NULL)
		return;
	HDC hDC=pDC->Detach();
	if(m_hMemDC==NULL)
		m_hMemDC=CreateCompatibleDC(hDC);//�����ڴ����DC
	HBITMAP hOldBmp=(HBITMAP)::SelectObject(m_hMemDC,m_hBmp);
	::StretchBlt(hDC,0,0,100,80,m_hMemDC,0,0,18,18,SRCCOPY);
	::SelectObject(m_hMemDC,hOldBmp);
	pDC->Attach(hDC);
}

// �����߽���
void CInfoDlg::DrawLine(CDC* pDC)
{
	CPen penBlack(PS_SOLID,1,RGB(155,0,200));
	CPen penRed(PS_SOLID,2,RGB(255,0,0)),*pOldPen;
	pOldPen=pDC->SelectObject(&penBlack);
	pDC->MoveTo(0,80);
	pDC->LineTo(100,80);
	pDC->LineTo(100,0);
	pDC->LineTo(0,0);
	pDC->LineTo(0,80);
	pDC->SelectObject(&penRed);
	pDC->MoveTo(0,40);
	pDC->LineTo(100,40);
	pDC->MoveTo(50,0);
	pDC->LineTo(50,80);
	pDC->SelectObject(pOldPen);
}

// ����RGB��ɫֵ�ӿ�
void CInfoDlg::SetRGBValue(int r, int g, int b)
{
	m_rValue=r;
	m_gValue=g;
	m_bValue=b;
}

// ���ý�ȡ�����С�ӿ�
void CInfoDlg::SetRectValue(int nWidth, int nHeight)
{
	m_nWidth=nWidth;
	m_nHeight=nHeight;
}

// ���õ�ǰ�����������λͼ���
void CInfoDlg::SetBitmap(HBITMAP hBmp)
{
	::DeleteObject(m_hBmp);
	m_hBmp=NULL;
	m_hBmp=hBmp;
}
