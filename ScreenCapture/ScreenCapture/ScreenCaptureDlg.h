
// ScreenCaptureDlg.h : ͷ�ļ�
//

#pragma once
#define WM_EXITCAP WM_USER+101
//#pragma comment(lib,"..\\..\\Bin\\ScreenCaptureDll.lib")
//void InitCaptureDlg();
//void ReleaseCaptureDlg();
// CScreenCaptureDlg �Ի���
class CScreenCaptureDlg : public CDialog
{
// ����
public:
	CScreenCaptureDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SCREENCAPTURE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBeginCapture();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
