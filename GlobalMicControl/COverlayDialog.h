#pragma once


// COverlayDialog dialog

class COverlayDialog : public CDialogEx
{
	DECLARE_DYNAMIC(COverlayDialog)

public:
	COverlayDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~COverlayDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COverlayDialog };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

private:
	LONG _defaultStyle;
};
