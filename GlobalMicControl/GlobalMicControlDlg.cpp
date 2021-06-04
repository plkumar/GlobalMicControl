
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
#include <string>
#include "CMicStatusOverlay.h"


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
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	micControl = new MicControl();
}

CGlobalMicControlDlg::~CGlobalMicControlDlg()
{
	if (statusOverlayForm != NULL)
	{
		statusOverlayForm->DestroyWindow();
		statusOverlayForm = NULL;
	}
	free(micControl);
	micControl = NULL;
}

void CGlobalMicControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CTrayDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HOTKEY_MICTOGGLE, hkcMicToggle);
	DDX_Control(pDX, IDC_LABEL_SELECTED_DEVICE, lblSelectedDevice);
	DDX_Control(pDX, IDC_CHECK_RUNATLOGIN, chkRunAtLogin);
	DDX_Control(pDX, IDC_MIC_IMAGE, picMicrophone);
}

void CGlobalMicControlDlg::OnTrayLButtonDown(CPoint pt)
{
	ToggleMute();
}

void CGlobalMicControlDlg::ToggleMute()
{
	MuteBehavior muteState = micControl->GetMuteState();
	if (muteState == MuteBehavior::MUTE) {
		micControl->SetMute(MuteBehavior::UNMUTE);
	}
	else {
		micControl->SetMute(MuteBehavior::MUTE);
	}
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

	TraySetIcon(IDI_MUTE);
	CString strTrayToolTip;
	ASSERT(strTrayToolTip.LoadString(IDS_SYSTRAY_TOOLTIP));
	TraySetToolTip(strTrayToolTip);
	TraySetMenu(IDR_MENU1);
	TraySetMinimizeToTray(TRUE);
	TrayShow();

	// Register hot key 
	/*WORD wKeyAndShift = static_cast<WORD>(hkcMicToggle.GetHotKey());
	this->SendMessage(WM_SETHOTKEY, wKeyAndShift);*/

	WORD vk=NULL, modifiers=NULL;
	WORD runAtLogin = NULL;
	if (ReadRegWordValue(L"VirtualKey", vk) && ReadRegWordValue(L"ModifierKey", modifiers))
	{
		hkcMicToggle.SetHotKey(vk, modifiers);
	}
	if (ReadRegWordValue(L"RunAtLogin", runAtLogin)) {
		chkRunAtLogin.SetCheck(runAtLogin);
	}

	HBITMAP bitmap = LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BITMAP1));
	picMicrophone.SetBitmap(bitmap);

	auto defaultDevice = micControl->GetDefaultDeviceName();
	lblSelectedDevice.SetWindowTextW(defaultDevice);

	CreateOverlayWindow();
	ShowOverlayWindow(SW_SHOW);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGlobalMicControlDlg::CreateOverlayWindow()
{
	statusOverlayForm = NULL;
	statusOverlayForm = new CMicStatusOverlay();
	if (statusOverlayForm != NULL)
	{
		// create and load the frame with its resources
		auto ret = statusOverlayForm->LoadFrame(IDR_MENU2, 0, NULL, NULL);
		if (!ret)   //Create failed.
		{
			TRACE(L"Error creating overlay window.");
		}
		statusOverlayForm->SetTitle(L"Mic Status");
	}
}

void CGlobalMicControlDlg::ShowOverlayWindow(int nID)
{
	if (statusOverlayForm != NULL )
	{
		statusOverlayForm->GetMenu()->Detach();
		statusOverlayForm->SetMenu(NULL);
		
		statusOverlayForm->ShowWindow(nID);
		statusOverlayForm->StayOnTop();
		statusOverlayForm->UpdateWindow();
	}
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
	//AfxMessageBox(L"Here we'll toggle the transparent overlay");
	ShowOverlayWindow(SW_SHOW);
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
		if (statusOverlayForm != NULL)
			statusOverlayForm->CloseWindow();
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
	WORD vk, modifiers;
	hkcMicToggle.GetHotKey(vk, modifiers);
	if (RegisterHotKey(this->m_hWnd, ID_HOTKEY, modifiers, vk) != TRUE)
	{
		AfxMessageBox(L"Error using current key combination, try a different combination.");
	}
	else {
		// TODO Move to AfxGetApp()->WriteProfile??? methods
		WriteRegWordValue(L"VirtualKey", vk);
		WriteRegWordValue(L"ModifierKey", modifiers);
		WriteRegWordValue(L"RunAtLogin", chkRunAtLogin.GetCheck());
		if (chkRunAtLogin.GetCheck() != 0)
		{
			WriteRegStringValueWithKey(L"GlobalMicControl", GetAppFullPath(), keyRunAtLogin);
		}
		this->ShowWindow(SW_HIDE);
	}

	//CTrayDialog::OnOK();
}


void CGlobalMicControlDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	if (nHotKeyId == ID_HOTKEY) {
		//AfxMessageBox(L"hotkey pressed.");
		ToggleMute();
	}
	else
		CTrayDialog::OnHotKey(nHotKeyId, nKey1, nKey2);
}

bool CGlobalMicControlDlg::WriteRegStringValueWithKey(const LPTSTR valueName, CString& value, const LPCTSTR keyName) const
{
	ATL::CRegKey regKey;
	if (ERROR_SUCCESS != regKey.Open(key_, keyName, KEY_WRITE)) {
		if (ERROR_SUCCESS != regKey.Create(key_, keyName))
		{
			regKey.Close();
			return false;
		}
	}

	return (ERROR_SUCCESS == regKey.Create(key_, keyName, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE)) && ERROR_SUCCESS == regKey.SetStringValue(valueName, value);
	return false;
}

bool CGlobalMicControlDlg::WriteRegStringValue(const LPTSTR valueName, CString& value) const
{
	return WriteRegStringValueWithKey(valueName, value, keyAppDefault);
}

bool CGlobalMicControlDlg::ReadRegStringValue(const LPTSTR valueName, CString& strDest) const
{
	ATL::CRegKey regKey;
	int nError = ERROR_SUCCESS;
	TCHAR szStringValue[LF_FACESIZE] = { 0 };
	ULONG cchMaxLen = _countof(szStringValue);
	if (ERROR_SUCCESS == regKey.Open(key_, keyAppDefault, KEY_READ | KEY_QUERY_VALUE)) {
		nError = regKey.QueryStringValue(valueName, szStringValue, &cchMaxLen);
		if (nError == ERROR_SUCCESS)
		{
			strDest = szStringValue;
			return (true);
		}
		else
		{
			return (false);
		}
	}

	return false;
}

bool CGlobalMicControlDlg::WriteRegWordValue(const LPTSTR valueName, DWORD value) const
{
	ATL::CRegKey regKey;
	if (ERROR_SUCCESS != regKey.Open(key_, keyAppDefault, KEY_WRITE)) {
		if (ERROR_SUCCESS != regKey.Create(key_, keyAppDefault))
		{
			return false;
		}
	}

	return (ERROR_SUCCESS == regKey.Create(key_, keyAppDefault, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE)) && ERROR_SUCCESS == regKey.SetDWORDValue(valueName, value);
}


bool CGlobalMicControlDlg::ReadRegWordValue(const LPTSTR valueName, WORD& value) const
{
	ATL::CRegKey regKey;
	DWORD refWordValue;
	if (ERROR_SUCCESS == regKey.Open(key_, keyAppDefault, KEY_READ | KEY_QUERY_VALUE)) {
		if (ERROR_SUCCESS == regKey.QueryDWORDValue(valueName, refWordValue))
		{
			value = (WORD)refWordValue;
			regKey.Close();
			return true;
		}
	}

	return false;
}

CString CGlobalMicControlDlg::GetAppFullPath()
{
	CString executableName = AfxGetApp()->m_pszExeName; // Get the "GlobalMicControl" portion executable.
	executableName.Append(L".exe"); // Now has "GlobalMicControl.exe" 
	HMODULE hmod = GetModuleHandle(executableName);
	CString fullPath;
	DWORD pathLen = ::GetModuleFileName(hmod, fullPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH); // stripoff zeros
	fullPath.ReleaseBuffer(pathLen);
	return fullPath;
}