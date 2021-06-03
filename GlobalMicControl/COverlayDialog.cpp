// COverlayDialog.cpp : implementation file
//

#include "pch.h"
#include "GlobalMicControl.h"
#include "COverlayDialog.h"
#include "afxdialogex.h"


// COverlayDialog dialog

IMPLEMENT_DYNAMIC(COverlayDialog, CDialogEx)

COverlayDialog::COverlayDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_COverlayDialog, pParent)
{

}

COverlayDialog::~COverlayDialog()
{
}

void COverlayDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COverlayDialog, CDialogEx)
	ON_WM_ACTIVATE()
//	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()


// COverlayDialog message handlers


BOOL COverlayDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	_defaultStyle = GetWindowLong(this->m_hWnd, GWL_EXSTYLE);
	SetWindowLong(this->m_hWnd, GWL_EXSTYLE, _defaultStyle | WS_EX_LAYERED | WS_EX_TRANSPARENT);
	SetLayeredWindowAttributes(0, 128, LWA_ALPHA);
	return TRUE;
}


void COverlayDialog::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	if (nState == WA_ACTIVE)
	{
		CDialogEx::OnActivate(nState, pWndOther, bMinimized);
		SetWindowLong(this->m_hWnd, GWL_EXSTYLE, _defaultStyle);
		SetLayeredWindowAttributes(0, 255, LWA_ALPHA);
	}
	else if (nState == WA_INACTIVE)
	{
		//this.FormBorderStyle = FormBorderStyle.None;
		//Click through
		int wl = GetWindowLong(this->m_hWnd, GWL_EXSTYLE);
		SetWindowLong(this->m_hWnd, GWL_EXSTYLE, wl | WS_EX_LAYERED | WS_EX_TRANSPARENT);
		//Change alpha
		//User32_SetLayeredWindowAttributes(this.Handle, (TransparencyKey.B << 16) + (TransparencyKey.G << 8) + TransparencyKey.R, _alpha, LayeredWindowAttributes.LWA_COLORKEY | LayeredWindowAttributes.LWA_ALPHA);
		SetLayeredWindowAttributes(0, 128, LWA_ALPHA);

		//Region = System.Drawing.Region.FromHrgn(CreateRoundRectRgn(0, 0, Width, Height, 50, 50));
		//RECT winSize;
		//GetClientRect(&winSize);

		//auto rectRgn = CreateRoundRectRgn(0, 0, winSize.left- winSize.right, winSize.top - winSize.bottom, 50, 50);
		//SetWindowRgn(rectRgn, TRUE);

		//CRect rect;
		//GetClientRect(rect);
		////Create the custom region for the window
		//m_CustomRgn.CreateRoundRectRgn(rect.left + winStats.frm_clp_wdth,
		//	rect.top + winStats.frm_clp_wdth,
		//	rect.right - winStats.frm_clp_wdth,
		//	rect.bottom - winStats.frm_clp_wdth,
		//	20, 20);
		////Set the window region
		//VERIFY(SetWindowRgn(m_CustomRgn, TRUE));
	}
}

