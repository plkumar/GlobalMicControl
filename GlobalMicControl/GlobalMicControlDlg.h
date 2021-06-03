
// GlobalMicControlDlg.h : header file
//

#pragma once


#include "TrayDialog.h"
#include "MicControl.h"
#include <string>

// CGlobalMicControlDlg dialog
class CGlobalMicControlDlg : public CTrayDialog
{
// Construction
public:
	CGlobalMicControlDlg(CWnd* pParent = nullptr);	// standard constructor
	~CGlobalMicControlDlg();
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GLOBALMICCONTROL_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual void OnTrayLButtonDown(CPoint pt);

	void ToggleMute();

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

	bool WriteRegStringValueWithKey(const LPTSTR valueName, CString& value, const LPCTSTR keyName) const;
	
	bool WriteRegStringValue(const LPTSTR valueName, CString& value) const;

	bool ReadRegStringValue(const LPTSTR valueName, CString& strDest) const;

	bool WriteRegWordValue(const LPTSTR valueName, DWORD value) const;

	bool ReadRegWordValue(const LPTSTR valueName, WORD& value) const;

	CString GetAppFullPath();

private:
	MicControl *micControl;
	HINSTANCE instanceHandle;
	LPCTSTR keyAppDefault = L"SOFTWARE\\GlobalMicControl";
	LPCTSTR keyRunAtLogin = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
	HKEY key_ = HKEY_CURRENT_USER;
public:
	CStatic lblSelectedDevice;
protected:
	CButton chkRunAtLogin;
};
