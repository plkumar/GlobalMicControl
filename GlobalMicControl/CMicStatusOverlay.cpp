#include "pch.h"
#include "CMicStatusOverlay.h"
#include "resource.h"
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
	ON_WM_ACTIVATE()
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	//ON_WM_PAINT()
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


void CMicStatusOverlay::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CCustomFrame::OnActivate(nState, pWndOther, bMinimized);

	if (nState == WA_ACTIVE)
	{
		SetWindowLong(this->m_hWnd, GWL_EXSTYLE, _defaultStyle);
		SetLayeredWindowAttributes(0, 255, LWA_ALPHA);
		UpdateWindow();
	}
	else if (nState == WA_INACTIVE)
	{
		int wl = GetWindowLong(this->m_hWnd, GWL_EXSTYLE);
		SetWindowLong(this->m_hWnd, GWL_EXSTYLE, wl | WS_EX_LAYERED | WS_EX_TRANSPARENT);
		SetLayeredWindowAttributes(0, 128, LWA_ALPHA);
		UpdateWindow();
	}
}

void CMicStatusOverlay::StayOnTop() const
{
	CRect rect;

	// get the current window size and position
	GetWindowRect(rect);

	// now change the size, position, and Z order 
	// of the window.
	::SetWindowPos(m_hWnd,       // handle to window
		HWND_TOPMOST,  // placement-order handle
		rect.left,     // horizontal position
		rect.top,      // vertical position
		rect.Width(),  // width
		rect.Height(), // height
		SWP_SHOWWINDOW // window-positioning options
);
}


void CMicStatusOverlay::OnDestroy()
{

	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);
	AfxGetApp()->WriteProfileBinary(L"MicStatusOverlay", L"WP", (LPBYTE)&wp, sizeof(wp));

	CCustomFrame::OnDestroy();
}


void CMicStatusOverlay::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CCustomFrame::OnShowWindow(bShow, nStatus);

	static bool bOnce = true;

	if (bShow && !IsWindowVisible() && bOnce)
	{
		bOnce = false;

		WINDOWPLACEMENT* lwp;
		UINT nl;

		if (AfxGetApp()->GetProfileBinary(L"MicStatusOverlay", L"WP", (LPBYTE*)&lwp, &nl))
		{
			SetWindowPlacement(lwp);
			delete[] lwp;
		}
	}
}


//void CMicStatusOverlay::OnPaint()
//{
//	CPaintDC dc(this); // device context for painting
//					   // TODO: Add your message handler code here
//					   // Do not call CCustomFrame::OnPaint() for painting messages
//	if (IsIconic())
//	{
//		CPaintDC dc(this); // device context for painting
//
//		SendMessage(WM_ICONERASEBKGND,
//			reinterpret_cast<WPARAM>(dc.GetSafeHdc()),
//			0);
//
//		// Center icon in client rectangle
//		int cxIcon = GetSystemMetrics(SM_CXICON);
//		int cyIcon = GetSystemMetrics(SM_CYICON);
//		CRect rect;
//		GetClientRect(&rect);
//		int x = (rect.Width() - cxIcon + 1) / 2;
//		int y = (rect.Height() - cyIcon + 1) / 2;
//
//		// Draw the icon
//		dc.DrawIcon(x, y, this->GetIcon(FALSE));
//	}
//	else
//	{
//		//Snippet 01: Get Client Coordinate of the Rectangle
//		CRect ClinetRect;
//		this->GetClientRect(&ClinetRect);
//
//		//Snippet 02: Declare Device Contexts and bitmap
//		CDC memoryDC;
//		CPaintDC DialogDC(this);
//		CBitmap tiledImage;
//
//		//Snippet 03: Load Image to Memory
//		memoryDC.CreateCompatibleDC(&DialogDC);
//		tiledImage.LoadBitmap(IDB_BITMAP1);
//		memoryDC.SelectObject(&tiledImage);
//
//		//Snippet 04: Copy memory pixels to dialog surface
//		DialogDC.BitBlt(
//			0,
//			0,
//			ClinetRect.Width(),
//			ClinetRect.Height(),
//			&memoryDC,
//			0,
//			0,
//			SRCCOPY);
//		//CCustomFrame::OnPaint();
//	}
//}
