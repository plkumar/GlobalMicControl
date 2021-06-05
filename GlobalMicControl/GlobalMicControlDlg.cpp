
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
	m_pmicControl = new MicControl();
}

CGlobalMicControlDlg::~CGlobalMicControlDlg()
{
	if (frmMicStatusOverlay != NULL)
	{
		//frmMicStatusOverlay->DestroyWindow();
		frmMicStatusOverlay = NULL;
	}
	free(m_pmicControl);
	m_pmicControl = NULL;
}

void CGlobalMicControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CTrayDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HOTKEY_MICTOGGLE, hkcMicToggle);
	DDX_Control(pDX, IDC_LABEL_SELECTED_DEVICE, lblSelectedDevice);
	DDX_Control(pDX, IDC_CHECK_RUNATLOGIN, chkRunAtLogin);
	//DDX_Control(pDX, IDC_MIC_IMAGE, picMicrophone);
	DDX_Control(pDX, IDC_CHECK_ENABLEOVERLAY, chkEnableMicStatus);
	DDX_Control(pDX, IDC_STATUSOVERLAYGROUP, pnlMicStatusOverlay);
	DDX_Control(pDX, IDC_ALPHASLIDER, sldrTransparencyAlpha);
	DDX_Control(pDX, IDC_COMBO_OVERLAYSIZE, comboOverLaySize);
}

void CGlobalMicControlDlg::OnTrayLButtonDown(CPoint pt)
{
	ToggleMute();
}

void CGlobalMicControlDlg::ToggleMute()
{
	MuteBehavior muteState = m_pmicControl->GetMuteState();
	if (muteState == MuteBehavior::MUTE) {
		m_pmicControl->SetMute(MuteBehavior::UNMUTE);
	}
	else {
		m_pmicControl->SetMute(MuteBehavior::MUTE);
	}

	UpdateMuteState();
}

void CGlobalMicControlDlg::UpdateMuteState()
{
	MuteBehavior muteState = m_pmicControl->GetMuteState();
	frmMicStatusOverlay->UpdateMicStatus((BYTE)muteState);
	auto icon = muteState == MuteBehavior::MUTE ? IDI_MUTE : IDI_UNMUTE;
	TraySetIcon(icon);
	TrayUpdate();
}

BEGIN_MESSAGE_MAP(CGlobalMicControlDlg, CTrayDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_COMMAND(ID_TRAYMENU_ABOUT, OnTrayMenuAbout)
	ON_COMMAND(ID_TRAYMENU_SETTINGS, OnTrayMenuSettings)
	ON_COMMAND(ID_TRAYMENU_SHOWOVERLAY, OnTrayMenuShowOverlay)
	ON_COMMAND(ID_TRAYMENU_EXIT, OnTrayMenuExit)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_MICTOGGLE_RESET, &CGlobalMicControlDlg::OnClickedBtnMicToggleReset)
	ON_BN_CLICKED(IDOK, &CGlobalMicControlDlg::OnBnClickedOk)
	ON_WM_HOTKEY()
	ON_BN_CLICKED(IDC_CHECK_ENABLEOVERLAY, &CGlobalMicControlDlg::OnClickedCheckEnableOverlay)
	ON_WM_DESTROY()
	ON_WM_ACTIVATE()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CGlobalMicControlDlg message handlers

BOOL CGlobalMicControlDlg::OnInitDialog()
{
	CTrayDialog::OnInitDialog();
	this->ShowWindow(SW_HIDE);
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
	if(m_pmicControl->GetMuteState()==MuteBehavior::MUTE)
		TraySetIcon(IDI_MUTE);
	else
		TraySetIcon(IDI_UNMUTE);
	CString strTrayToolTip;
	ASSERT(strTrayToolTip.LoadString(IDS_SYSTRAY_TOOLTIP));
	TraySetToolTip(strTrayToolTip);
	TraySetMenu(IDR_MENU1);
	TraySetMinimizeToTray(TRUE);
	TrayShow();

	chkEnableMicStatus.SetCheck(AfxGetApp()->GetProfileIntW(L"", REG_ENABLEMIC_STATUS, 0));
	sldrTransparencyAlpha.SetRange(10, 200, TRUE);
	sldrTransparencyAlpha.SetPos(AfxGetApp()->GetProfileIntW(L"", REG_ALPHACHANNEL, 128));
	sldrTransparencyAlpha.EnableWindow(chkEnableMicStatus.GetCheck());

	WORD vk=NULL, modifiers=NULL;
	vk = AfxGetApp()->GetProfileIntW(L"", REG_VIRTUAL_KEY, 0);
	modifiers = AfxGetApp()->GetProfileIntW(L"", REG_MODIFIER_KEY, 0);
	
	if (vk!=0 || modifiers != 0 )
	{
		hkcMicToggle.SetHotKey(vk, modifiers);
		if (RegisterHotKey(this->m_hWnd, ID_HOTKEY, modifiers, vk) != TRUE)
		{
			TRACE("Unable to register hot key.");
		}
	}

	chkRunAtLogin.SetCheck(AfxGetApp()->GetProfileIntW(L"", REG_RUNAT_LAUNCH, 0));

	PopulateSizeDropdown();
	
	/*auto oldStyle = GetWindowLong(picMicrophone.m_hWnd, GWL_STYLE);
	SetWindowLong(picMicrophone.m_hWnd, GWL_STYLE, oldStyle | SS_REALSIZECONTROL);

	HBITMAP bitmap = LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BITMAP1));
	picMicrophone.SetBitmap(bitmap);*/

	auto defaultDevice = m_pmicControl->GetDefaultDeviceName();
	lblSelectedDevice.SetWindowTextW(defaultDevice);

	if (chkEnableMicStatus.GetCheck() == TRUE)
	{
		TraySetMenuItemChecked(ID_TRAYMENU_SHOWOVERLAY, TRUE);
		ShowOverlayWindow(SW_SHOW);
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGlobalMicControlDlg::PopulateSizeDropdown()
{
	auto selectedSize = AfxGetApp()->GetProfileIntW(L"", REG_OVERLAY_SIZE, _overlaySize);
	for (int i = 50; i <= 400; i += 50)
	{
		CString strItem;
		strItem.Format(L"%dx%d", i, i);
		comboOverLaySize.AddString(strItem);
		if (i == selectedSize)
		{
			comboOverLaySize.SelectString(0, strItem);
		}
	}
}

void CGlobalMicControlDlg::CreateOverlayWindow()
{
	if (frmMicStatusOverlay != NULL && ::IsWindow(frmMicStatusOverlay->m_hWnd))
	{
		frmMicStatusOverlay->CloseWindow();
		frmMicStatusOverlay->DestroyWindow();
		frmMicStatusOverlay = NULL;
	}
	//frmMicStatusOverlay = new CMicStatusOverlay();
	_overlaySize = AfxGetApp()->GetProfileIntW(L"", REG_OVERLAY_SIZE, _overlaySize);
	frmMicStatusOverlay = new CMicStatusForm();
	frmMicStatusOverlay->SetOverlaySize(_overlaySize);
	if (frmMicStatusOverlay != NULL)
	{
		// create and load the frame with its resources
		auto ret = frmMicStatusOverlay->LoadFrame(IDR_MENU2, NULL, NULL, NULL);
		if (!ret)   //Create failed.
		{
			TRACE(L"Error creating overlay window.");
		}
		frmMicStatusOverlay->SetTitle(L"Mic Status");
		frmMicStatusOverlay->GetMenu()->Detach();
		frmMicStatusOverlay->SetMenu(NULL);
		frmMicStatusOverlay->UpdateMicStatus((BYTE)m_pmicControl->GetMuteState());
	}
}

void CGlobalMicControlDlg::ShowOverlayWindow(int nID=SW_SHOW)
{
	if(frmMicStatusOverlay == NULL || !::IsWindow(frmMicStatusOverlay->m_hWnd))
		CreateOverlayWindow();

	if (frmMicStatusOverlay != NULL && ::IsWindow(frmMicStatusOverlay->m_hWnd))
	{
		if (nID == SW_SHOW) {
			isOverLayVisible = TRUE;
		}else {
			isOverLayVisible = FALSE;
		}
		TraySetMenuItemChecked(ID_TRAYMENU_SHOWOVERLAY, nID == SW_SHOW);
		frmMicStatusOverlay->StayOnTop();
		frmMicStatusOverlay->ShowWindow(nID);
		//frmMicStatusOverlay->UpdateWindow();
	}
}

void CGlobalMicControlDlg::CloseOverlayWindow()
{
	TRY
		if (frmMicStatusOverlay != NULL && IsWindow(frmMicStatusOverlay->m_hWnd) )
		{
			//frmMicStatusOverlay->CloseWindow(); // this doesn't seem to trigger OnClose() CMicStatusForm
			SendMessageA(frmMicStatusOverlay->m_hWnd, WM_CLOSE, NULL, NULL);
			//frmMicStatusOverlay->DestroyWindow();
		}
	CATCH_ALL(e)
		//ignore.
	END_CATCH_ALL
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
	if (isOverLayVisible == FALSE)
	{
		ShowOverlayWindow(SW_SHOW);
		isOverLayVisible = TRUE;
	}else
	{
		ShowOverlayWindow(SW_HIDE);
		isOverLayVisible = FALSE;
	}
}

void CGlobalMicControlDlg::OnTrayMenuExit()
{
	CloseOverlayWindow();
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
	WORD vk, modifiers;
	hkcMicToggle.GetHotKey(vk, modifiers);
	if (UnregisterHotKey(this->m_hWnd, ID_HOTKEY) != TRUE) {
		TRACE(L"Error, Unregistering hotkey");
	}
	if (RegisterHotKey(this->m_hWnd, ID_HOTKEY, modifiers, vk) != TRUE)
	{
		AfxMessageBox(L"Error using current key combination, try a different combination.");
	}
	else {
		AfxGetApp()->WriteProfileInt(L"", REG_VIRTUAL_KEY, vk);
		AfxGetApp()->WriteProfileInt(L"", REG_MODIFIER_KEY, modifiers);
		AfxGetApp()->WriteProfileInt(L"", REG_RUNAT_LAUNCH, chkRunAtLogin.GetCheck());
		if (chkRunAtLogin.GetCheck() != 0)
		{
			WriteRegStringValueWithKey(REG_GLOBALMICCONTROL, GetAppFullPath(), keyRunAtLogin);
		}
		this->ShowWindow(SW_HIDE);
	}

	AfxGetApp()->WriteProfileInt(L"", REG_ENABLEMIC_STATUS, chkEnableMicStatus.GetCheck());
	AfxGetApp()->WriteProfileInt(L"", REG_ALPHACHANNEL, sldrTransparencyAlpha.GetPos());
	
	CString selectedItem;
	comboOverLaySize.GetLBText(comboOverLaySize.GetCurSel(), selectedItem);
	int xpos = selectedItem.Find(L"x");
	selectedItem = selectedItem.Left(xpos);
	_overlaySize = std::stoi({ selectedItem.GetString(), static_cast<size_t>(selectedItem.GetLength()) });
	AfxGetApp()->WriteProfileInt(L"", REG_OVERLAY_SIZE, _overlaySize);

	if(frmMicStatusOverlay!=NULL && frmMicStatusOverlay->IsWindowVisible())
		frmMicStatusOverlay->UpdateOpacity(sldrTransparencyAlpha.GetPos());

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

void CGlobalMicControlDlg::OnClickedCheckEnableOverlay()
{
	// TODO: Add your control notification handler code here
	pnlMicStatusOverlay.EnableWindow(chkEnableMicStatus.GetCheck());
	sldrTransparencyAlpha.EnableWindow(chkEnableMicStatus.GetCheck());
}


void CGlobalMicControlDlg::OnDestroy()
{
	TrayHide();

	CTrayDialog::OnDestroy();

	// TODO: Add your message handler code here
}


void CGlobalMicControlDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CTrayDialog::OnActivate(nState, pWndOther, bMinimized);

	// TODO: Add your message handler code here
	//if (nState== WA_ACTIVE)
	//{
	//	static bool isFirstActivation = true;
	//	if (isFirstActivation) {
	//		isFirstActivation = false;
	//		::PostMessage(this->GetSafeHwnd(), WM_COMMAND, IDOK, 0); //end dialog with idok
	//		//::PostMessage(this->GetSafeHwnd(), WM_CLOSE, 0, 0); //or, close dialog
	//	}
	//}
}


void CGlobalMicControlDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CTrayDialog::OnShowWindow(bShow, nStatus);
	int initialLaunch = AfxGetApp()->GetProfileIntW(L"", REG_INITIAL_LAUNCH, TRUE);
	// TODO: Add your message handler code here
	static bool isFirstLaunch = true;
	if (isFirstLaunch && bShow == TRUE && initialLaunch!= TRUE)
	{
		isFirstLaunch = false;
		::PostMessage(this->GetSafeHwnd(), WM_COMMAND, IDOK, 0); //end dialog with idok
		//::PostMessage(this->GetSafeHwnd(), WM_CLOSE, 0, 0); //or, close dialog
	}

	if (initialLaunch == TRUE)
	{
		AfxGetApp()->WriteProfileInt(L"", REG_INITIAL_LAUNCH, FALSE);
	}
}

