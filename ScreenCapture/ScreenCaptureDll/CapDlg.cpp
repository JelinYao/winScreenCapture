// CapDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ScreenCaptureDll.h"
#include "CapDlg.h"
#define MAKECURSOR(x) LoadCursor(NULL,x)
#define BRIGHT_VALUE 155>>8//���ȱ仯��
//���ȴ���
#define GET_BRIGHT_RVALUE(x) GetRValue(x)*BRIGHT_VALUE
#define GET_BRIGHT_GVALUE(x) GetGValue(x)*BRIGHT_VALUE
#define GET_BRIGHT_BVALUE(x) GetBValue(x)*BRIGHT_VALUE
//���Ȼ�ԭ����
#define RESET_RVALUE(x) GetRValue(x)<<8/155
#define RESET_GVALUE(x) GetGValue(x)<<8/155
#define RESET_BVALUE(x) GetBValue(x)<<8/155
////////////////////////////////////////////////////////////////////////////////////////////////////
//��̬������ʼ��
int CCapDlg::m_nxScreen=0;
int CCapDlg::m_nyScreen=0;
////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _DATA
{
	BYTE* pData;//������
	int nLen;//���ݳ���
}DATA;
//���ȴ����߳�
DWORD WINAPI ChangeBrightThread(LPVOID lpParam)
{
	ASSERT(lpParam);
	DATA* pData=(DATA*)lpParam;
	BYTE* pHead=pData->pData;
	BYTE* pTail=pHead+pData->nLen;
	DWORD dw1=0,dw2=0;//���ص��RGB��ɫֵ
	BYTE r1=0,g1=0,b1=0,r2=0,g2=0,b2=0;
	int nFlag=sizeof(DWORD);
	while(pTail>=pHead)
	{
		memcpy(&dw1,pHead,nFlag);
		memcpy(&dw2,pTail,nFlag);
		r1=GET_BRIGHT_RVALUE(dw1);
		g1=GET_BRIGHT_GVALUE(dw1);
		b1=GET_BRIGHT_BVALUE(dw1);
		r2=GET_BRIGHT_RVALUE(dw2);
		g2=GET_BRIGHT_GVALUE(dw2);
		b2=GET_BRIGHT_BVALUE(dw2);
		dw1=RGB(r1,g1,b1);
		dw2=RGB(r2,g2,b2);
		memcpy(pHead,&dw1,nFlag);
		memcpy(pTail,&dw2,nFlag);
		pHead+=nFlag;
		pTail-=nFlag;
	}
	delete pData;
	pData=NULL;
	return 1;
}
// CCapDlg �Ի���
HBITMAP CCapDlg::m_hScreenBmp=NULL;
HBITMAP CCapDlg::m_hScreenBmpC=NULL;
IMPLEMENT_DYNAMIC(CCapDlg, CDialog)

CCapDlg::CCapDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCapDlg::IDD, pParent)
	,m_curState(undo)
	,m_hMemDC1(NULL)
	,m_hMemDC2(NULL)
	,m_hCursor(NULL)
	,m_hBrightBmp(NULL)
{
	m_hCursor=AfxGetApp()->LoadCursor(IDC_CURSOR1);
}

CCapDlg::~CCapDlg()
{//�ͷ���Դ
	if(m_hMemDC1)
	{
		DeleteDC(m_hMemDC1);
		m_hMemDC1=NULL;
	}
	if(m_hMemDC2)
	{
		DeleteDC(m_hMemDC2);
		m_hMemDC2=NULL;
	}
	if(m_hScreenBmp)
	{
		DeleteObject(m_hScreenBmp);
		m_hScreenBmp=NULL;
	}
	if(m_hBrightBmp)
	{
		DeleteObject(m_hBrightBmp);
		m_hBrightBmp=NULL;
	}
	if(m_hCursor)
	{
		DeleteObject(m_hCursor);
		m_hCursor=NULL;
	}
	if(m_pInfoDlg)
	{
		m_pInfoDlg->DestroyWindow();
		delete m_pInfoDlg;
		m_pInfoDlg=NULL;
	}
	if(m_pToolBar)
	{
		m_pToolBar->DestroyWindow();
		delete m_pToolBar;
		m_pToolBar=NULL;
	}
	if(m_hScreenBmpC)
	{
		::DeleteObject(m_hScreenBmpC);
		m_hScreenBmpC=NULL;
	}
}

void CCapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCapDlg, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(CAPTOOLBAR_SAVE,&CCapDlg::OnSave)
	ON_COMMAND(CAPTOOLBAR_EXIT,&CCapDlg::OnExit)
END_MESSAGE_MAP()


// ��ȡ����ͼƬ���浽�����
HBITMAP CCapDlg::GetScreenBitmap(LPRECT rcDes)
{//��ȡm_hScreenBmp
	if(IsRectEmpty(rcDes))
		return NULL;
	//HBITMAP hScreenBmp=GetScreenBitmap();
	//ASSERT(hScreenBmp);
	int nWidth=rcDes->right-rcDes->left;
	int nHeight=rcDes->bottom-rcDes->top;
	TRACE(_T("RECT :%d  ,%d  ,%d  ,%d\n"),rcDes->left,rcDes->top,rcDes->right,rcDes->bottom);
	TRACE(_T("width:%d,height:%d\n"),nWidth,nHeight);
	HDC hDC=NULL,hMemDC=NULL;
	hDC=::CreateCompatibleDC(NULL);
	hMemDC=::CreateCompatibleDC(NULL);
	ASSERT(hDC&&hMemDC);
	HBITMAP hBmp=::CreateCompatibleBitmap(CClientDC(NULL),nWidth,nHeight);
	ASSERT(hBmp);
	HBITMAP hOldBmp1=(HBITMAP)::SelectObject(hMemDC,hBmp);
	HBITMAP hOldBmp2=(HBITMAP)::SelectObject(hDC,m_hScreenBmpC);
	::BitBlt(hMemDC,0,0,nWidth,nHeight,hDC,rcDes->left,rcDes->top,SRCCOPY);
	hBmp=(HBITMAP)::SelectObject(hMemDC,hOldBmp1);
	::SelectObject(hDC,hOldBmp2);
	::DeleteObject(hDC);
	::DeleteObject(hMemDC);
	//::DeleteObject(hScreenBmp);
	return hBmp;
}
HBITMAP CCapDlg::GetScreenBitmap()
{
	HDC hScreenDC,hMemDC;
	hScreenDC=::CreateDC(_T("DISPLAY"),NULL,NULL,NULL);
	hMemDC=::CreateCompatibleDC(hScreenDC);
	HBITMAP hBmp=NULL,hOldBmp=NULL;
	hBmp=CreateCompatibleBitmap(hScreenDC,m_nxScreen,m_nyScreen);
	hOldBmp=(HBITMAP)SelectObject(hMemDC,hBmp);
	BitBlt(hMemDC,0,0,m_nxScreen,m_nyScreen,hScreenDC,0,0,SRCCOPY);
	hBmp=(HBITMAP)SelectObject(hMemDC,hOldBmp);
	::DeleteDC(hScreenDC);
	::DeleteDC(hMemDC);
	return hBmp;
}
HBITMAP CCapDlg::CopyHBitmap(HBITMAP hSrcBmp)
{
	HDC hSrcDC=NULL,hDesDC=NULL;
	hSrcDC=::CreateCompatibleDC(NULL);
	hDesDC=::CreateCompatibleDC(NULL);
	BITMAP bm = {0};
	::GetObject(hSrcBmp, sizeof(bm), &bm);
	HBITMAP hRetBmp = ::CreateCompatibleBitmap(CClientDC(NULL), bm.bmWidth, bm.bmHeight);
	HBITMAP hSrcOld = (HBITMAP)::SelectObject(hSrcDC, hSrcBmp);
	HBITMAP hDesOld = (HBITMAP)::SelectObject(hDesDC, hRetBmp);
	::BitBlt(hDesDC,0, 0, bm.bmWidth, bm.bmHeight,hSrcDC, 0, 0, SRCCOPY );
	::SelectObject(hSrcDC, hSrcOld);
	hRetBmp=(HBITMAP)::SelectObject(hDesDC, hDesOld);
	::DeleteObject(hSrcDC);
	::DeleteObject(hDesDC);
	return hRetBmp;
}


BOOL CCapDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//��ʼ�������
	m_ptStart.x=0;
	m_ptStart.y=0;
	m_rcTrack.m_nStyle=CRectTracker::solidLine|CRectTracker::resizeOutside;
	m_rcTrack.m_rect.SetRect(-10,-10,-10,-10);
	HDC hSrc=::GetDC(NULL);
	m_nxScreen=GetDeviceCaps(hSrc,HORZRES);
	m_nyScreen=GetDeviceCaps(hSrc,VERTRES);
	::ReleaseDC(NULL,hSrc);
	//����͸����Ϣ�Ի���
	m_pInfoDlg=NULL;
	m_pInfoDlg=new CInfoDlg(this);
	m_pInfoDlg->Create(IDD_DIALOG2,this);
	//����������
	m_pToolBar=NULL;
	m_pToolBar=new CCapToolBar(this);
	m_pToolBar->ModifyStyleEx(0,WS_EX_TOPMOST);
	m_hScreenBmp=GetScreenBitmap();
	m_hBrightBmp=GetScreenBitmap();
	m_hScreenBmpC=CopyHBitmap(m_hScreenBmp);
	ChangeBitmapBright(m_hBrightBmp);
	MoveWindow(-1,-1,m_nxScreen+1,m_nyScreen+1);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
BOOL CCapDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	HDC hPaintDC=pDC->Detach();
	ASSERT(hPaintDC);
	if(m_hMemDC1==NULL)
		m_hMemDC1=CreateCompatibleDC(hPaintDC);
	if(m_hMemDC2==NULL)
	{
		m_hMemDC2=CreateCompatibleDC(m_hMemDC1);
		SelectObject(m_hMemDC2,m_hScreenBmp);
	}
	RECT rc=m_rcTrack.m_rect;
	int nWidth=rc.right-rc.left;
	int nHeight=rc.bottom-rc.top;
	HBITMAP hOldBmp1=NULL;
	if(m_curState!=undo)
	{
		HBITMAP hCopyBmp=CopyHBitmap(m_hBrightBmp);
		hOldBmp1=(HBITMAP)::SelectObject(m_hMemDC1,hCopyBmp);
		::BitBlt(m_hMemDC1,rc.left,rc.top,nWidth,nHeight,m_hMemDC2,rc.left,rc.top,SRCCOPY);
		DeleteObject(hCopyBmp);
	}
	else
		hOldBmp1=(HBITMAP)::SelectObject(m_hMemDC1,m_hBrightBmp);
	::BitBlt(hPaintDC,0,0,m_nxScreen,m_nyScreen,m_hMemDC1,0,0,SRCCOPY);
	::SelectObject(m_hMemDC1,hOldBmp1);
	pDC->Attach(hPaintDC);
	m_rcTrack.Draw(pDC);
	::ReleaseDC(m_hWnd,hPaintDC);
	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}
inline void CCapDlg::SendExitMsg()
{
	EndDialog(1);
}
LRESULT CCapDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
	switch(message)
	{
	case WM_RBUTTONDOWN:
		{
			if(m_curState==undo)//û�н�����ֱ���˳�
				SendExitMsg();
			else//������ɣ�ȡ������
			{
				m_rcTrack.m_rect.SetRect(-10,-10,-10,-10);
				m_curState=undo;
				Invalidate();
			}
			return 1;
		}
	case WM_KEYDOWN:
		{
			switch(wParam)
			{
			case VK_ESCAPE:
				{
					SendExitMsg();
					return 1;
				}
			case VK_RETURN:
				return 1;
			default:
				break;
			}
		}
	default:
		return CDialog::WindowProc(message, wParam, lParam);
	}
}

void CCapDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(m_curState==undo)
	{//��ʼ����
		m_curState=doing;
		m_ptStart=point;

	}
	else//������ɺ�
		m_rcTrack.Track(this,point,TRUE);
	CDialog::OnLButtonDown(nFlags, point);
}

void CCapDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(m_curState==doing)
	{
		m_rcTrack.m_rect.SetRect(m_ptStart,point);
		Invalidate();
	}
	else
	{
		CRect rc=m_rcTrack.m_rect;
		int nWidth=rc.Width();
		int nHeight=rc.Height();
		if(rc.left<0)
		{
			rc.left=0;
			rc.right=nWidth;
		}
		if(rc.top<0)
		{
			rc.top=0;
			rc.bottom=nHeight;
		}
		if(rc.right>m_nxScreen)
		{
			rc.left=m_nxScreen-nWidth;
			rc.right=m_nxScreen;
		}
		if(rc.bottom>m_nyScreen)
		{
			rc.top=m_nyScreen-nHeight;
			rc.bottom=m_nyScreen;
		}
		m_rcTrack.m_rect=rc;
		InvalidateRect(&rc);
	}
	MoveInfoDlg();
	if(nFlags==MK_LBUTTON)
	{
		m_pToolBar->ShowWindow(SW_HIDE);
		return;
	}
	MoveToolBar();
	CDialog::OnMouseMove(nFlags, point);
}

void CCapDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(m_curState==doing)
		m_curState=finish;
	CDialog::OnLButtonUp(nFlags, point);
}

// �ı�ͼ�������
void CCapDlg::ChangeBitmapBright(HBITMAP hBmp)
{
	ASSERT(hBmp);
	CBitmap bm;
	bm.Attach(hBmp);
	BITMAP bmMetric;
	bm.GetBitmap(&bmMetric);
	bm.Detach();
	DWORD dwCount=bmMetric.bmWidth*bmMetric.bmBitsPixel*bmMetric.bmHeight;//ͼ����ռ���ڴ������
	HGLOBAL hMem=GlobalAlloc(GHND,dwCount);
	BYTE* pBuffer=(BYTE*)GlobalLock(hMem);
	GetBitmapBits(hBmp,dwCount,pBuffer);
	int nFlag=sizeof(COLORREF);
	DWORD dwLen=dwCount/nFlag;//�������ص�ĸ���
	//ÿ����50,0000�����ص㿪һ���߳�
	int nThreadNum=(dwLen+49999)/500000;
	HANDLE* pThread=new HANDLE[nThreadNum];
	for(int i=0;i<nThreadNum;++i)
	{
		DATA* pData=new DATA;
		pData->pData=pBuffer+i*500000;
		pData->nLen=500000;
		if(i==nThreadNum-1)//���һ��ѭ��
		{
			int nRet=dwLen%500000;
			if(nRet)
				pData->nLen=nRet;
		}
		pThread[i]=CreateThread(NULL,0,ChangeBrightThread,pData,0,NULL);
	}
	::WaitForMultipleObjects(nThreadNum,pThread,TRUE,INFINITE);
	delete[] pThread;
	pThread=NULL;
	SetBitmapBits(hBmp,dwCount,pBuffer);
	GlobalUnlock(hMem);
	GlobalFree(hMem);
}

BOOL CCapDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CPoint pt;
	::GetCursorPos(&pt);
	int nHitFlag=m_rcTrack.HitTest(pt);
	if(nHitFlag<0)//�ⲿ
		::SetCursor(m_hCursor);
	else if(nHitFlag==0||nHitFlag==2)//���Ϻ�����
		::SetCursor(MAKECURSOR(IDC_SIZENWSE));
	else if(nHitFlag==1||nHitFlag==3)//���º�����
		::SetCursor(MAKECURSOR(IDC_SIZENESW));
	else if(nHitFlag==4||nHitFlag==6)//�����͵ײ�
		::SetCursor(MAKECURSOR(IDC_SIZENS));
	else if(nHitFlag==5||nHitFlag==7)//�󲿺��Ҳ�
		::SetCursor(MAKECURSOR(IDC_SIZEWE));
	else//�ڲ�
		::SetCursor(MAKECURSOR(IDC_SIZEALL));
	return TRUE;
	//return CDialog::OnSetCursor(pWnd, nHitTest, message);//���������Ĭ�ϵ����ָ��
}
void CCapDlg::MoveInfoDlg()
{
	CPoint pt;
	::GetCursorPos(&pt);
	CRect rcBm(pt.x-9,pt.y-9,pt.x+9,pt.y+9);
	m_pInfoDlg->SetBitmap(GetScreenBitmap(&rcBm));
	COLORREF color=::GetPixel(m_hMemDC2,pt.x,pt.y);
	m_pInfoDlg->SetRGBValue(RESET_RVALUE(color),RESET_GVALUE(color),RESET_BVALUE(color));
	m_pInfoDlg->SetRectValue(m_rcTrack.m_rect.Width(),m_rcTrack.m_rect.Height());
	m_pInfoDlg->Invalidate();
	CPoint point;
	::GetCursorPos(&point);
	int xPos,yPos;
	if(point.x>(m_nxScreen-183)&&point.y>(m_nyScreen-168))
	{
		xPos=point.x-183;
		yPos=point.y-168;
	}
	else if(point.x>(m_nxScreen-183))
	{
		xPos=point.x-183-10;
		yPos=point.y+5;
	}
	else if(point.y>(m_nyScreen-168-25))
	{
		xPos=point.x-5;
		yPos=point.y-168-10;
	}
	else
	{
		xPos=point.x+5;
		yPos=point.y+30;
	}
	m_pInfoDlg->MoveWindow(xPos,yPos,180,160,TRUE);
	if(IsWindowVisible())
		m_pInfoDlg->ShowWindow(SW_SHOW);
}

void CCapDlg::SaveBitmap(HBITMAP hBmp)
{
	SYSTEMTIME sm;
	::GetLocalTime(&sm);
	CString str;
	str.Format(_T("%d-%d-%d_%d_%d_%d"),sm.wYear,sm.wMonth,sm.wDay,sm.wHour,sm.wMinute,sm.wMinute);
	CFileDialog fd(FALSE,_T("bmp"),_T("MyCapture")+str,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("Bmp  File   (*.bmp)|*.bmp|Jpeg File    (*.jpg)|*.jpg|Png  File    (*.png)|*.png|Gif  File      (*.gif)|*.gif||"));
	if(IDOK==fd.DoModal())
	{
		CImage image;
		image.Attach(hBmp);
		CString strExtension;
		if(fd.m_ofn.nFileExtension!=0)
		{
			switch(fd.m_ofn.nFilterIndex)
			{
			case 1:strExtension=_T(".bmp");break;
			case 2:strExtension=_T(".jpg");break;
			case 3:strExtension=_T(".png");break;
			case 4:strExtension=_T(".gif");break;
			default:break;
			}
		}
		CString fullName=fd.m_ofn.lpstrFile+strExtension;
		HRESULT hRet=image.Save(fullName);
		hBmp=image.Detach();
		if(FAILED(hRet))
		{
			::MessageBox(NULL,_T("����ʧ��!"),_T("������:"),MB_OK|MB_ICONERROR);
			::DeleteObject(hBmp);
		}
	}
	::DeleteObject(hBmp);
}
// �ƶ�������
void CCapDlg::MoveToolBar()
{
	if(IsRectEmpty(m_rcTrack.m_rect))
	{
		m_pToolBar->ShowWindow(SW_HIDE);
		return;
	}
	CRect rcToolBar;
	m_pToolBar->GetClientRect(&rcToolBar);
	/*CString strMsg;
	strMsg.Format(_T("%d,%d"),rcToolBar.left,rcToolBar.top);
	MessageBox(strMsg);*/
	if(m_nxScreen-m_rcTrack.m_rect.right<50)
	{//���������ұ߷Ų��£��Ƶ���߷���
	/*	if((rcToolBar.left==m_rcTrack.m_rect.right-50)&&(rcToolBar.top==m_rcTrack.m_rect.bottom-25))
			return;*/
		rcToolBar.left=m_rcTrack.m_rect.right-50;
		rcToolBar.top=m_rcTrack.m_rect.bottom-25;
	}
	else//�ұ߷���
	{
		/*if((rcToolBar.top==m_rcTrack.m_rect.bottom-25))
			return;*/
		rcToolBar.left=m_rcTrack.m_rect.right;
		rcToolBar.top=m_rcTrack.m_rect.bottom-25;
	}
	rcToolBar.right=rcToolBar.left+50;
	rcToolBar.bottom=m_rcTrack.m_rect.bottom;
	m_pToolBar->MoveWindow(&rcToolBar);
	if(IsWindowVisible())
		m_pToolBar->ShowWindow(SW_SHOW);
}

void CCapDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(m_rcTrack.m_rect.PtInRect(point))
	{
		//�����������λͼ�����а�
		if(::OpenClipboard(this->GetSafeHwnd()))
		{
			if(::EmptyClipboard())
			{
				HBITMAP hBmp=GetScreenBitmap(&m_rcTrack.m_rect);
				::SetClipboardData(CF_BITMAP,hBmp);
				::DeleteObject(hBmp);
				::CloseClipboard();
			}
		}
		SendExitMsg();
	}
	CDialog::OnLButtonDblClk(nFlags, point);
}
void CCapDlg::OnSave()
{
	HBITMAP hSaveBmp=GetScreenBitmap(&m_rcTrack.m_rect);
	SaveBitmap(hSaveBmp);
	SendExitMsg();
}
void CCapDlg::OnExit()
{
	SendExitMsg();
}
BOOL CCapDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if(pMsg->message==WM_KEYDOWN)
	{
		CRect* pRect=&m_rcTrack.m_rect;
		switch(pMsg->wParam)
		{
		case VK_UP:
			{
				if(pRect->top>0)
				{
					pRect->top-=1;
					pRect->bottom-=1;
					Invalidate();
					MoveToolBar();
				}
				return 1;
			}
		case VK_DOWN:
			{
				if(pRect->bottom<m_nyScreen)
				{
					pRect->top+=1;
					pRect->bottom+=1;
					Invalidate();
					MoveToolBar();
				}
				return 1;
			}
		case VK_LEFT:
			{
				if (pRect->left>0)
				{
					pRect->left-=1;
					pRect->right-=1;
					Invalidate();
					MoveToolBar();
				}
				return 1;
			}
		case VK_RIGHT:
			{
				if(pRect->right<m_nxScreen)
				{
					pRect->left+=1;
					pRect->right+=1;
					Invalidate();
					MoveToolBar();
				}
				return 1;
			}
		default:
			break;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
