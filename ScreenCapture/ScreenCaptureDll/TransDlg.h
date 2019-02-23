#pragma once

#include "resource.h"
// CTransDlg �Ի���

class CInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CInfoDlg)

public:
	CInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CInfoDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
private:
	//������굱ǰλ�õ�RGB��ɫֵ
	int m_rValue;
	int m_gValue;
	int m_bValue;
	//���浱ǰ��������Ĵ�С
	int m_nWidth;
	int m_nHeight;
	CRect m_rcClient;
	//����λͼ���
	HBITMAP m_hBmp;
	//�ڴ����DC���
	HDC m_hMemDC;
private:
	void DrawText(CDC* pDC);
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
private:
	void DrawBitmap(CDC* pDC);
	// �����߽���
	void DrawLine(CDC* pDC);
public:
	///////////////////////////////////////////////////����ӿڲ���
	// ����RGB��ɫֵ�ӿ�
	void SetRGBValue(int r, int g, int b);
	// ���ý�ȡ�����С�ӿ�
	void SetRectValue(int nWidth, int nHeight);
	// ���õ�ǰ�����������λͼ���
	void SetBitmap(HBITMAP hBmp);
};
