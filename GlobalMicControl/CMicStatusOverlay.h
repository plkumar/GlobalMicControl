#pragma once
#include "CustomWindowFrm.h"
class CMicStatusOverlay :
	public CCustomFrame
{
	DECLARE_DYNCREATE(CMicStatusOverlay)
public:
	CMicStatusOverlay();           // protected constructor used by dynamic creation
	virtual ~CMicStatusOverlay();
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
public:
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

	void StayOnTop() const;

private:
	LONG _defaultStyle;
public:
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPaint();
	afx_msg void OnClose();
};

