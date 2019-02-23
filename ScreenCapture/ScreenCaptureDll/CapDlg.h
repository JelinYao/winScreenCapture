#pragma once
#include "ColorRect.h"
#include "TransDlg.h"
#include "CapToolBar.h"
#include <ATLIMAGE.H>
// CCapDlg �Ի���
#define WM_EXITCAP WM_USER+101
//������ID
#define IDC_TOOLBAR 1000
class CCapDlg : public CDialog
{
	DECLARE_DYNAMIC(CCapDlg)

public:
	CCapDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCapDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };
	//����״̬
	typedef enum
	{
		undo,
		doing,
		finish
	}STATE;
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//������水ť
	afx_msg void OnSave();
	afx_msg void OnExit();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	CColorRect m_rcTrack;
	//������������������
	CPoint m_ptStart;
	//�򸸴��ڷ����˳���Ϣ
	void SendExitMsg();
	//�ƶ���Ϣ��
	void MoveInfoDlg();
	// �ƶ�������
	void MoveToolBar();
	void DrawBrightBack(CDC* pDC);
	void DrawRectBack(CDC* pDC);
private://˽�г�Ա����
	//���ȴ����ľ��
	HBITMAP m_hBrightBmp;
	//�ڴ����DC
	HDC m_hMemDC1;
	HDC m_hMemDC2;
	STATE m_curState;
	//����ʱ���ָ��
	HCURSOR m_hCursor;
	//��ʾ���λ����Ϣ�ĶԻ���
	CInfoDlg* m_pInfoDlg;
	//������
	CCapToolBar* m_pToolBar;
public://��̬��Ա,�ӿڲ���
	// ����������Ļ�ľ��
	static HBITMAP m_hScreenBmp;
	//��Ļ����Ŀ���
	static HBITMAP m_hScreenBmpC;
	//��ʾ����С
	static int m_nxScreen;
	static int m_nyScreen;
	// ��ȡ����ͼƬ���浽�����
	static HBITMAP GetScreenBitmap(LPRECT rcDes);
	//��ȡ������ʾ��
	static HBITMAP GetScreenBitmap();
	//����λͼ���
	static HBITMAP CopyHBitmap(HBITMAP hSourceHbitmap);
	//����ͼƬ
	static void SaveBitmap(HBITMAP);
	// �ı�ͼ�������
	static void ChangeBitmapBright(HBITMAP hBmp);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
