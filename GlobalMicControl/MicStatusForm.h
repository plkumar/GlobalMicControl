#pragma once
#include "Layered.h"

// CMicStatusForm frame

class CMicStatusForm : public CFrameWnd, CLayered<CMicStatusForm>
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
	void DrawWindowRegion(WINDOWPLACEMENT* lwp);
	afx_msg void OnClose();
	void MakeWindowTransparent(BOOL isTransparent);
	void DrawMicStatus(BYTE isMuted);
	void SaveWindowPlacement();

private:
	BYTE _alphaChannel = 128; //default to 50% opacity
	//CTransparentStatic imgMicStatus;
	CStatic imgMicStatus;
	HICON m_hIcon;
	HICON m_muteIcon;
	HICON m_unmuteIcon;
	BYTE _micStatus;
	int _windowWidth=200;
	int _windowHeight=200;
	CWnd* p_parentController;
public:
	void UpdateOpacity(BYTE alpha);
	void UpdateMicStatus(BYTE status);
	void SetOverlaySize(int size);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	afx_msg void OnExitSizeMove();
	void SetParentController(CWnd* parent);
};


