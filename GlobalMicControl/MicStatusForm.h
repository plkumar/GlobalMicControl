#pragma once
#include "TransparentStatic.h"


// CMicStatusForm frame

class CMicStatusForm : public CFrameWnd
{
	DECLARE_DYNCREATE(CMicStatusForm)
public:
	CMicStatusForm();
	virtual ~CMicStatusForm();

	void StayOnTop() const;

protected:
	DECLARE_MESSAGE_MAP()

private:
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	void MakeTransparent(BOOL isTransparent);
	void DrawMicStatus(BYTE isMuted);
	void SaveWindowPlacement();

private:
	BYTE _alphaChannel = 128; //default to 50% opacity
	LONG _defaultStyle;
	CTransparentStatic imgMicStatus;
	HICON m_hIcon;
	HICON m_muteIcon;
	HICON m_unmuteIcon;
	BYTE _micStatus;
public:
	void UpdateOpacity(BYTE alpha);
	void UpdateMicStatus(BYTE status);
	
};


