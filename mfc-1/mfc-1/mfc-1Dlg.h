
// mfc-1Dlg.h : ͷ�ļ�
//

#pragma once


// Cmfc1Dlg �Ի���
class Cmfc1Dlg : public CDialogEx
{
// ����
public:
	Cmfc1Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MFC1_DIALOG };

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
	afx_msg void OnBnClickedPlay();
	afx_msg void OnBnClickedPause();
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedAbout();
	afx_msg void OnBnClickedAbout2();
	afx_msg void OnBnClickedClose();
	afx_msg void OnEnChangeUrl();
	CEdit m_url;
	afx_msg void OnBnClickedOpenFileDig();
};
