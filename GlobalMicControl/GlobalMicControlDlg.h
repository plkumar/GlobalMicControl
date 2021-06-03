
// GlobalMicControlDlg.h : header file
//

#pragma once


#include "TrayDialog.h"

// CGlobalMicControlDlg dialog
class CGlobalMicControlDlg : public CTrayDialog
{
// Construction
public:
	CGlobalMicControlDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GLOBALMICCONTROL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual void OnTrayLButtonDown(CPoint pt);

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	void ShowAbout();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTrayMenuAbout();
	afx_msg void OnTrayMenuSettings();
	afx_msg void OnTrayMenuShowOverlay();
	afx_msg void OnTrayMenuExit();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	afx_msg void OnClickedBtnMicToggleReset();
	CHotKeyCtrl hkcMicToggle;
	afx_msg void OnBnClickedOk();
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
	afx_msg void OnBnClickedCancel();
};
