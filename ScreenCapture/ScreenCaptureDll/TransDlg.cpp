// TransDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TransDlg.h"
#include "ScreenCaptureDll.h"

// CTransDlg 对话框

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


// CTransDlg 消息处理程序

BOOL CInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
////////////////////////////////////////////////////////////////////////////
	//设置透明对话框
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
	// 异常: OCX 属性页应返回 FALSE
}

void CInfoDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	dc.FillSolidRect(&m_rcClient,RGB(255,255,255));
	dc.FrameRect(&m_rcClient,&CBrush(RGB(0,0,200)));
	DrawText(&dc);
	DrawBitmap(&dc);
	DrawLine(&dc);
	// 不为绘图消息调用 CDialog::OnPaint()
}

void CInfoDlg::DrawText(CDC* pDC)
{
	CRect rcText;
	rcText.left=2;
	rcText.top=86;
	rcText.right=m_rcClient.Width();
	rcText.bottom=m_rcClient.Height();
	CFont ft,*pOldFont;
	ft.CreatePointFont(120,_T("宋体"));
	pOldFont=pDC->SelectObject(&ft);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(255,0,0));
	CString strRect,strColor;
	strRect.Format(_T("当前区域大小:\n         %d  ×  %d\n"),m_nWidth,m_nHeight);
	strColor.Format(_T("  RGB(%d,%d,%d)"),m_rValue,m_gValue,m_bValue);
	CString text=strRect+strColor+_T("\n双击截屏区复制到剪贴板");
	pDC->DrawText(text,&rcText,DT_LEFT);
	pDC->SelectObject(pOldFont);
	ft.DeleteObject();
}

BOOL CInfoDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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
		m_hMemDC=CreateCompatibleDC(hDC);//保存内存兼容DC
	HBITMAP hOldBmp=(HBITMAP)::SelectObject(m_hMemDC,m_hBmp);
	::StretchBlt(hDC,0,0,100,80,m_hMemDC,0,0,18,18,SRCCOPY);
	::SelectObject(m_hMemDC,hOldBmp);
	pDC->Attach(hDC);
}

// 画出边界线
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

// 设置RGB颜色值接口
void CInfoDlg::SetRGBValue(int r, int g, int b)
{
	m_rValue=r;
	m_gValue=g;
	m_bValue=b;
}

// 设置截取区域大小接口
void CInfoDlg::SetRectValue(int nWidth, int nHeight)
{
	m_nWidth=nWidth;
	m_nHeight=nHeight;
}

// 设置当前鼠标所在区域位图句柄
void CInfoDlg::SetBitmap(HBITMAP hBmp)
{
	::DeleteObject(m_hBmp);
	m_hBmp=NULL;
	m_hBmp=hBmp;
}
