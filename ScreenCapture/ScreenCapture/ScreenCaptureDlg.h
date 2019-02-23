
// ScreenCaptureDlg.h : 头文件
//

#pragma once
#define WM_EXITCAP WM_USER+101
//#pragma comment(lib,"..\\..\\Bin\\ScreenCaptureDll.lib")
//void InitCaptureDlg();
//void ReleaseCaptureDlg();
// CScreenCaptureDlg 对话框
class CScreenCaptureDlg : public CDialog
{
// 构造
public:
	CScreenCaptureDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SCREENCAPTURE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
