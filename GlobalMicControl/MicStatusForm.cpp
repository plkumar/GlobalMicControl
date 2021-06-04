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
		SetWindowLong(this->m_hWnd, GWL_EXSTYLE, _defaultStyle);
		SetLayeredWindowAttributes(0, 255, LWA_ALPHA);
		ModifyStyle(0, WS_CAPTION); // to show titlebar
		DrawMicStatus(TRUE);
		//UpdateWindow();
	}
	else if (nState == WA_INACTIVE)
	{
		//_defaultStyle = GetWindowLong(this->m_hWnd, GWL_EXSTYLE);
		SetWindowLong(this->m_hWnd, _defaultStyle | GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT);
		ModifyStyle(WS_CAPTION, 0); // to hide titlebar
		//ModifyStyleEx(WS_EX_CLIENTEDGE | WS_EX_WINDOWEDGE, 0);
		SetLayeredWindowAttributes(0, _alphaChannel, LWA_ALPHA);

		RECT rect;
		GetClientRect(&rect);
		CRgn m_CustomRgn;
		m_CustomRgn.CreateRoundRectRgn(rect.left,
			rect.top
			, rect.right, rect.bottom, 90, 90);
		VERIFY(SetWindowRgn(m_CustomRgn, TRUE));

		DrawMicStatus(FALSE);
		//UpdateWindow();
	}
}

void CMicStatusForm::DrawMicStatus(BOOL isActive)
{
	RECT clientRect;
	GetClientRect(&clientRect);

	 //optional step - see below
	/*if (isActive) {
		clientRect.top += 40;
		clientRect.bottom -= 40;
	}*/

	static bool isCreated = false;
	//imgMicStatus.DestroyWindow();
	if (!isCreated && imgMicStatus.Create(L"", WS_CHILD | WS_BORDER | WS_VISIBLE | SS_BITMAP | SS_REALSIZECONTROL | SS_CENTERIMAGE, clientRect, this) == TRUE)
	//if (!isCreated && imgMicStatus.Create(L"", WS_CHILD | WS_BORDER | WS_VISIBLE | SS_ICON | SS_REALSIZECONTROL | SS_CENTERIMAGE, clientRect, this) == TRUE)
	{
		isCreated = true;
		//HICON micIcon = LoadIcon(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_MUTE));
		//imgMicStatus.SetIcon(micIcon);
		HBITMAP bitmap = LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BITMAP1));
		
		imgMicStatus.SetBitmap(bitmap);	
		imgMicStatus.RedrawWindow(&clientRect);
		imgMicStatus.ShowWindow(SW_SHOW);
	}
	else {
		//imgMicStatus.RedrawWindow(&clientRect);
		//imgMicStatus.GetWindowRect(&clientRect);
		imgMicStatus.ScreenToClient(&clientRect);
		imgMicStatus.ShowWindow(SW_SHOW);
	}
}

void CMicStatusForm::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);
	AfxGetApp()->WriteProfileBinary(L"", L"WP", (LPBYTE)&wp, sizeof(wp));
	ShowWindow(SW_HIDE);
	//CFrameWnd::OnClose();
}


int CMicStatusForm::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	this->SetWindowPos((const CWnd*)NULL, (int)0, (int)0,
		200,
		200,
		SWP_NOZORDER | SWP_NOMOVE );

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	_defaultStyle = GetWindowLongW(this->m_hWnd, GWL_EXSTYLE);

	return 0;
}

void CMicStatusForm::UpdateOpacity(BYTE alpha)
{
	this->_alphaChannel = alpha;
	UpdateWindow();
}

void CMicStatusForm::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFrameWnd::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here
	DrawMicStatus(TRUE);

	//static bool bOnce = true;

	if (bShow && !IsWindowVisible()) // && bOnce)
	{
		//bOnce = false;

		WINDOWPLACEMENT* lwp;
		UINT nl;

		if (AfxGetApp()->GetProfileBinary(L"", L"WP", (LPBYTE*)&lwp, &nl))
		{
			SetWindowPlacement(lwp);
			delete[] lwp;
		}
	}
}


BOOL CMicStatusForm::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CFrameWnd::OnCreateClient(lpcs, pContext);
}
