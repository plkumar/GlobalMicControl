
// GlobalMicControlDlg.h : header file
//

#pragma once


#include "TrayDialog.h"
#include "MicControl.h"
#include <string>
#include "MicStatusForm.h"

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

	void UpdateMuteState();

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	void CreateOverlayWindow();
	void ShowOverlayWindow(int nID);
	void CloseOverlayWindow();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	void ShowAbout();
	afx_msg void OnTrayMenuAbout();
	afx_msg void OnTrayMenuSettings();
	afx_msg void OnTrayMenuShowOverlay();
	afx_msg void OnTrayMenuExit();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	afx_msg void OnClickedBtnMicToggleReset();
	afx_msg void OnBnClickedOk();
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);

	bool WriteRegStringValueWithKey(const LPTSTR valueName, CString& value, const LPCTSTR keyName) const;
	
	CString GetAppFullPath();

private:
	MicControl *m_pmicControl;
	HINSTANCE instanceHandle;
	LPCTSTR keyAppDefault = L"SOFTWARE\\GlobalMicControl";
	LPCTSTR keyRunAtLogin = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
	HKEY key_ = HKEY_CURRENT_USER;
	//CMicStatusOverlay* frmMicStatusOverlay;
	CMicStatusForm* frmMicStatusOverlay;
	BOOL isOverLayVisible = FALSE;
public:
	CHotKeyCtrl hkcMicToggle;
	CStatic lblSelectedDevice;
	CButton chkRunAtLogin;
	//CStatic picMicrophone;
	CButton chkEnableMicStatus;
	CStatic pnlMicStatusOverlay;
	CSliderCtrl sldrTransparencyAlpha;
	afx_msg void OnClickedCheckEnableOverlay();
	afx_msg void OnDestroy();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	CComboBox comboOverLaySize;
};
