
// mfc-avDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CmfcavDlg �Ի���
class CmfcavDlg : public CDialogEx
{
// ����
public:
	CmfcavDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MFCAV_DIALOG };

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
	afx_msg void OnBnClickedButton5();
	CEdit m_url;
	afx_msg void OnBnClickedOpenFile();
	afx_msg void OnBnClickedOk();
};
