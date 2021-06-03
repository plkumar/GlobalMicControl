#include "pch.h"
#include "CMicStatusOverlay.h"

// CCustomFrameTest

IMPLEMENT_DYNCREATE(CMicStatusOverlay, CCustomFrame)

CMicStatusOverlay::CMicStatusOverlay()
{

}

CMicStatusOverlay::~CMicStatusOverlay()
{
}


BEGIN_MESSAGE_MAP(CMicStatusOverlay, CCustomFrame)
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()


// CMicStatusOverlay message handlers
BOOL CMicStatusOverlay::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	return CCustomFrame::OnCreateClient(lpcs, pContext);
}

BOOL CMicStatusOverlay::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	return CCustomFrame::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMicStatusOverlay::OnSetFocus(CWnd* pOldWnd)
{
	CCustomFrame::OnSetFocus(pOldWnd);
}
