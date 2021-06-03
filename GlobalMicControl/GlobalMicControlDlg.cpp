
// GlobalMicControlDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "GlobalMicControl.h"
#include "GlobalMicControlDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Message from the Systray Icon
#define MYWM_NOTIFYICON		(WM_USER+2)
#define ID_HOTKEY 1001
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnClose();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
//	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CGlobalMicControlDlg dialog



CGlobalMicControlDlg::CGlobalMicControlDlg(CWnd* pParent /*=nullptr*/)
	: CTrayDialog(IDD_GLOBALMICCONTROL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void CGlobalMicControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CTrayDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HOTKEY_MICTOGGLE, hkcMicToggle);
}

void CGlobalMicControlDlg::OnTrayLButtonDown(CPoint pt)
{
	AfxMessageBox(L"click");
}

BEGIN_MESSAGE_MAP(CGlobalMicControlDlg, CTrayDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_TRAYMENU_ABOUT, OnTrayMenuAbout)
	ON_COMMAND(ID_TRAYMENU_SETTINGS, OnTrayMenuSettings)
	ON_COMMAND(ID_TRAYMENU_SHOWOVERLAY, OnTrayMenuShowOverlay)
	ON_COMMAND(ID_TRAYMENU_EXIT, OnTrayMenuExit)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_MICTOGGLE_RESET, &CGlobalMicControlDlg::OnClickedBtnMicToggleReset)
	ON_BN_CLICKED(IDOK, &CGlobalMicControlDlg::OnBnClickedOk)
	ON_WM_HOTKEY()
	//ON_BN_CLICKED(IDCANCEL, &CGlobalMicControlDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CGlobalMicControlDlg message handlers

BOOL CGlobalMicControlDlg::OnInitDialog()
{
	CTrayDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu, strQuitMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		bNameValid = FALSE;
		bNameValid = strQuitMenu.LoadString(IDS_QUIT);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX+101, strQuitMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	TraySetIcon(IDI_MIC_UNMUTE);
	CString strTrayToolTip;
	ASSERT(strTrayToolTip.LoadString(IDS_SYSTRAY_TOOLTIP));
	TraySetToolTip(strTrayToolTip);
	TraySetMenu(IDR_MENU1);
	TraySetMinimizeToTray(TRUE);
	TrayShow();

	// Register hot key 
	/*WORD wKeyAndShift = static_cast<WORD>(hkcMicToggle.GetHotKey());
	this->SendMessage(WM_SETHOTKEY, wKeyAndShift);*/

	hkcMicToggle.SetHotKey('M', HOTKEYF_SHIFT + HOTKEYF_ALT);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGlobalMicControlDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		ShowAbout();
	}
	else
	{
		CTrayDialog::OnSysCommand(nID, lParam);
	}
}

void CGlobalMicControlDlg::ShowAbout()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGlobalMicControlDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CTrayDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGlobalMicControlDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CGlobalMicControlDlg::OnTrayMenuAbout()
{
	ShowAbout();
}

void CGlobalMicControlDlg::OnTrayMenuSettings()
{
	this->ShowWindow(SW_SHOW);
}

void CGlobalMicControlDlg::OnTrayMenuShowOverlay()
{
	AfxMessageBox(L"Here we'll toggle the transparent overlay");
}

void CGlobalMicControlDlg::OnTrayMenuExit()
{
	TrayHide();
	SendMessage(WM_CLOSE);
}

void CGlobalMicControlDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	if (TrayIsVisible())
		this->ShowWindow(SW_HIDE);
	else
	{
		UnregisterHotKey(this->m_hWnd, ID_HOTKEY);
		CTrayDialog::OnClose();
	}
}

void CGlobalMicControlDlg::OnClickedBtnMicToggleReset()
{
	hkcMicToggle.SetHotKey('A', HOTKEYF_ALT | HOTKEYF_SHIFT);
}


void CGlobalMicControlDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//WORD wKeyAndShift = static_cast<WORD>(hkcMicToggle.GetHotKey());
	//auto result = this->SendMessage(WM_SETHOTKEY, wKeyAndShift);
	//ASSERT(RegisterHotKey(this->m_hWnd, 100, MOD_ALT| MOD_SHIFT | MOD_NOREPEAT, 'M')==TRUE);
	WORD vk, modifiers;
	hkcMicToggle.GetHotKey(vk, modifiers);
	if (RegisterHotKey(this->m_hWnd, ID_HOTKEY, modifiers, vk) != TRUE)
	{
		AfxMessageBox(L"Error using current key combination, try a different combination.");
	}
	else {
		this->ShowWindow(SW_HIDE);
	}
		
	//CTrayDialog::OnOK();
}


void CGlobalMicControlDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	// TODO: Add your message handler code here and/or call default
	AfxMessageBox(L"hotkey pressed.");
	CTrayDialog::OnHotKey(nHotKeyId, nKey1, nKey2);
}

//
//void CGlobalMicControlDlg::OnBnClickedCancel()
//{
//	if (TrayIsVisible())
//		this->ShowWindow(SW_HIDE);
//	else
//		CTrayDialog::OnClose();
//}
