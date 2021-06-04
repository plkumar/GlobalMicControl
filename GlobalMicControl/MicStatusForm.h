#pragma once


// CMicStatusForm frame

class CMicStatusForm : public CFrameWnd
{
	DECLARE_DYNCREATE(CMicStatusForm)
public:
	CMicStatusForm();           // protected constructor used by dynamic creation
	virtual ~CMicStatusForm();

	void StayOnTop() const;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

	void DrawMicStatus(BOOL isActive);

private:
	BYTE _alphaChannel = 128; //default to 50% opacity
	LONG _defaultStyle;
	CStatic imgMicStatus;
	HICON m_hIcon;
public:
	afx_msg void OnClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void UpdateOpacity(BYTE alpha);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
};


