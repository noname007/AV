
// mfc-1Dlg.h : 头文件
//

#pragma once


// Cmfc1Dlg 对话框
class Cmfc1Dlg : public CDialogEx
{
// 构造
public:
	Cmfc1Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MFC1_DIALOG };

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
