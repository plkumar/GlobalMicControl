// MicStatusForm.cpp : implementation file
//

#include "pch.h"
#include "GlobalMicControl.h"
#include "MicStatusForm.h"


// CMicStatusForm

IMPLEMENT_DYNCREATE(CMicStatusForm, CFrameWnd)

CMicStatusForm::CMicStatusForm()
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

CMicStatusForm::~CMicStatusForm()
{
}


BEGIN_MESSAGE_MAP(CMicStatusForm, CFrameWnd)
	ON_WM_ACTIVATE()
	ON_WM_CLOSE()
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_WM_DESTROY()
	ON_WM_EXITSIZEMOVE()
END_MESSAGE_MAP()


// CMicStatusForm message handlers
void CMicStatusForm::StayOnTop() const
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

void CMicStatusForm::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CFrameWnd::OnActivate(nState, pWndOther, bMinimized);

	if (nState == WA_ACTIVE)
	{
		MakeWindowTransparent(FALSE);
		DrawMicStatus(_micStatus);
		//UpdateWindow();
	}
	else if (nState == WA_INACTIVE)
	{
		MakeWindowTransparent(TRUE);

		/*RECT rect;
		GetClientRect(&rect);
		CRgn m_CustomRgn;
		m_CustomRgn.CreateRoundRectRgn(rect.left,
			rect.top
			, rect.right, rect.bottom, 90, 90);
		VERIFY(SetWindowRgn(m_CustomRgn, TRUE));*/

		DrawMicStatus(_micStatus);
		//UpdateWindow();
	}
}

void CMicStatusForm::MakeWindowTransparent(BOOL bTransparent)
{
	if (bTransparent == TRUE)
	{
		SetLayered();
		SetClickThru();
		MakeTransparent(_alphaChannel);
	}
	else {
		RemoveLayered();
		RemoveClickThru();
		MakeTransparent(255);
		ModifyStyle(WS_CAPTION, 0);
	}
}

void CMicStatusForm::DrawMicStatus(BYTE isMuted)
{
	RECT clientRect;
	GetClientRect(&clientRect);

	HBITMAP bitmap = isMuted ==TRUE? LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BITMAP2)) : LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BITMAP3));

	imgMicStatus.DestroyWindow();
	//if (imgMicStatus.Create(L"", WS_CHILD | WS_BORDER | WS_VISIBLE | SS_BITMAP | SS_REALSIZECONTROL | SS_CENTERIMAGE, clientRect, this) == TRUE)
	if (imgMicStatus.Create(L"", WS_CHILD | WS_BORDER | WS_VISIBLE | SS_BITMAP | SS_REALSIZECONTROL, clientRect, this) == TRUE)
	{
		imgMicStatus.SetWindowPos(this, 0, 0, clientRect.right, clientRect.bottom, 0);
		imgMicStatus.ModifyStyle(WS_BORDER, 0);
		imgMicStatus.SetBitmap(bitmap);	
		imgMicStatus.RedrawWindow(&clientRect);
		imgMicStatus.ShowWindow(SW_SHOW);
	}
}

void CMicStatusForm::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	SaveWindowPlacement();
	//CFrameWnd::OnClose();
}

void CMicStatusForm::SaveWindowPlacement()
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);
	AfxGetApp()->WriteProfileBinary(L"", L"WP", (LPBYTE)&wp, sizeof(wp));
}


int CMicStatusForm::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	WINDOWPLACEMENT* lwp;
	UINT nl;

	if (AfxGetApp()->GetProfileBinary(L"", L"WP", (LPBYTE*)&lwp, &nl))
	{
		SetWindowPlacement(lwp);
	}
	else {

		auto screenX = GetSystemMetrics(SM_CXSCREEN);
		auto screenY = GetSystemMetrics(SM_CYSCREEN);

		screenX = screenX - (screenX / 6);

		screenY = screenY - (screenY / 4);

		this->SetWindowPos((const CWnd*)NULL, screenX, screenY,
			200,
			200, SWP_SHOWWINDOW);
	}

	m_muteIcon = AfxGetApp()->LoadIcon(IDI_MUTE);
	m_unmuteIcon = AfxGetApp()->LoadIcon(IDI_UNMUTE);

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	//SetWindowText(L"Mic Status");

	if (lwp != NULL)
	{
		VERIFY(SetWindowPos(NULL, lwp->rcNormalPosition.left, lwp->rcNormalPosition.top, 200, 200, SWP_SHOWWINDOW | SWP_NOOWNERZORDER));
		HRGN region;
		region = CreateEllipticRgn(2, 2, 198, 198);
		SetWindowRgn(region, TRUE);
	}

	delete[] lwp;

	return 0;
}

void CMicStatusForm::UpdateOpacity(BYTE alpha)
{
	this->_alphaChannel = alpha;
	UpdateWindow();
}

void CMicStatusForm::UpdateMicStatus(BYTE status)
{
	_micStatus = status;

	if (status == 1) // MUTE
	{
		SetWindowText(L"MUTED");
		SetIcon(m_muteIcon, FALSE);
		DrawMicStatus(status);
	}
	else if (status == 2) // UNMUTE
	{
		SetWindowText(L"UNMUTED");
		SetIcon(m_unmuteIcon, FALSE);
		DrawMicStatus(status);
	}
	else
	{
		//TODO  implementation for unknown state.
	}
}

void CMicStatusForm::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFrameWnd::OnShowWindow(bShow, nStatus);

	//DrawMicStatus(TRUE);

	//if (bShow && !IsWindowVisible()) // && bOnce)
	//{
	//	WINDOWPLACEMENT* lwp;
	//	UINT nl;

	//	if (AfxGetApp()->GetProfileBinary(L"", L"WP", (LPBYTE*)&lwp, &nl))
	//	{
	//		SetWindowPlacement(lwp);
	//		delete[] lwp;
	//	}
	//} else if (!bShow) {
	//	SaveWindowPlacement();
	//}
}



BOOL CMicStatusForm::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_MOUSEMOVE && (pMsg->wParam & MK_LBUTTON))
	{
		CPoint p = pMsg->pt;
		ScreenToClient(&p);
		CRect r(50, 50, 200, 200);
		if (r.PtInRect(p))
		{
			ReleaseCapture();
			SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, 0);
			SendMessage(WM_NCLBUTTONUP, HTCAPTION, 0);
			return 1;
		}
	}

	return CFrameWnd::PreTranslateMessage(pMsg);
}


void CMicStatusForm::OnDestroy()
{
	//SaveWindowPlacement();
	CFrameWnd::OnDestroy();

	// TODO: Add your message handler code here
}


void CMicStatusForm::OnExitSizeMove()
{
	// TODO: Add your message handler code here and/or call default
	SaveWindowPlacement();
	__super::OnExitSizeMove();
}
