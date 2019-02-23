#pragma once

#include "resource.h"
// CTransDlg 对话框

class CInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CInfoDlg)

public:
	CInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CInfoDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
private:
	//保存鼠标当前位置的RGB颜色值
	int m_rValue;
	int m_gValue;
	int m_bValue;
	//保存当前截屏区域的大小
	int m_nWidth;
	int m_nHeight;
	CRect m_rcClient;
	//保存位图句柄
	HBITMAP m_hBmp;
	//内存兼容DC句柄
	HDC m_hMemDC;
private:
	void DrawText(CDC* pDC);
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
private:
	void DrawBitmap(CDC* pDC);
	// 画出边界线
	void DrawLine(CDC* pDC);
public:
	///////////////////////////////////////////////////定义接口部分
	// 设置RGB颜色值接口
	void SetRGBValue(int r, int g, int b);
	// 设置截取区域大小接口
	void SetRectValue(int nWidth, int nHeight);
	// 设置当前鼠标所在区域位图句柄
	void SetBitmap(HBITMAP hBmp);
};
